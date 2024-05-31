#pragma once

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <memory>
#include <mutex>
#include <optional>
#include <string_view>
#include <thread>
#include <vector>

#include "xcb/xcb.hpp"

namespace clipboardxx {

using std::string_view_literals::operator""sv;

inline constexpr std::chrono::duration kHandleEventsForEverDelay =
    std::chrono::milliseconds(20);
inline constexpr std::chrono::duration kWaitForPasteDataTimeout =
    std::chrono::milliseconds(300);
inline constexpr std::array kSupportedTextFormats = {
    "UTF8_STRING"sv,
    "text/plain;charset=utf-8"sv,
    "text/plain;charset=UTF-8"sv,
    "GTK_TEXT_BUFFER_CONTENTS"sv,
    "STRING"sv,
    "TEXT"sv,
    "text/plain"sv,
};

struct EssentialAtoms {
    std::vector<xcb::Atom> supported_text_formats;
    xcb::Atom clipboard;
    xcb::Atom targets;
    xcb::Atom atom;
    xcb::Atom buffer;
};

class X11EventHandler {
public:
    X11EventHandler(std::shared_ptr<xcb::Xcb> xcb)
        : m_xcb(std::move(xcb)),
          m_atoms(create_essential_atoms()),
          m_targets(generate_targets_atom_array(
              m_atoms.targets, m_atoms.supported_text_formats)),
          m_stop_event_thread(false) {
        m_event_thread =
            std::thread(&X11EventHandler::handle_events_for_ever, this);
    }

    ~X11EventHandler() {
        m_stop_event_thread = true;
        m_event_thread.join();
    }

    void set_copy_data(const std::string& data) {
        std::lock_guard<std::mutex> lock_guard(m_lock);
        m_copy_data = std::optional<std::string>(data);
    }

    std::string get_paste_data() {
        {
            std::lock_guard<std::mutex> lock_guard(m_lock);
            if (do_we_own_clipoard())
                return m_copy_data.value();
            else
                m_xcb->request_selection_data(
                    m_atoms.clipboard,
                    m_atoms.supported_text_formats.at(0), m_atoms.buffer);
        }

        wait_for_paste_data_with_timeout(kWaitForPasteDataTimeout);
        std::lock_guard<std::mutex> lock_guard(m_lock);
        std::string result = m_paste_data.value_or(std::string(""));
        m_paste_data.reset();
        return result;
    }

private:
    EssentialAtoms create_essential_atoms() const {
        EssentialAtoms atoms;
        atoms.clipboard = m_xcb->create_atom("CLIPBOARD");
        atoms.buffer    = m_xcb->create_atom("BUFFER");
        atoms.targets   = m_xcb->create_atom("TARGETS");
        atoms.atom      = m_xcb->create_atom("ATOM");

        atoms.supported_text_formats =
            std::vector<xcb_atom_t>(kSupportedTextFormats.size());
        std::transform(kSupportedTextFormats.begin(),
                       kSupportedTextFormats.end(),
                       atoms.supported_text_formats.begin(),
                       [&xcb = m_xcb](std::string_view name) {
                           return xcb->create_atom(name);
                       });
        return atoms;
    }

    static std::vector<xcb::Atom> generate_targets_atom_array(
        xcb::Atom target, const std::vector<xcb::Atom>& atoms) {
        std::vector<xcb::Atom> targets(atoms.size() + 1);
        targets[0] = target;
        std::copy(atoms.begin(), atoms.end(), targets.begin() + 1);
        return targets;
    }

    bool do_we_own_clipoard() const {
        return m_copy_data.has_value();
    }

    void wait_for_paste_data_with_timeout(
        std::chrono::milliseconds timeout) {
        m_paste_data.reset();
        std::chrono::system_clock::time_point start_time =
            std::chrono::system_clock::now();

        while (true) {
            if (std::chrono::system_clock::now() - start_time > timeout)
                break;

            std::this_thread::sleep_for(
                std::chrono::milliseconds(kHandleEventsForEverDelay));
            std::lock_guard<std::mutex> lock_guard(m_lock);

            if (m_paste_data.has_value())
                break;
        }
    }

    void handle_events_for_ever() noexcept {
        while (!m_stop_event_thread) {
            std::this_thread::sleep_for(kHandleEventsForEverDelay);
            std::lock_guard<std::mutex> lock_guard(m_lock);
            std::optional<std::unique_ptr<xcb::Event>> event =
                m_xcb->get_latest_event();

            if (event.has_value())
                handle_event(std::move(event.value()));
        }
    }

    void handle_event(std::unique_ptr<xcb::Event> event) {
        switch (event->get_type()) {
            case xcb::Event::Type::kRequestSelection:
                handle_request_selection_event(
                    reinterpret_cast<xcb::RequestSelectionEvent*>(
                        event.get()));
                break;
            case xcb::Event::Type::kSelectionClear:
                m_copy_data = std::nullopt;
                break;
            case xcb::Event::Type::kSelectionNotify:
                handle_selection_notify_event(
                    reinterpret_cast<xcb::SelectionNotifyEvent*>(
                        event.get()));
                break;
            case xcb::Event::Type::kNone:
                return;
        }
    }

    void handle_request_selection_event(
        const xcb::RequestSelectionEvent* event) {
        if (event->m_selection != m_atoms.clipboard ||
            !m_copy_data.has_value())
            return;

        bool found_format =
            std::find(m_atoms.supported_text_formats.begin(),
                      m_atoms.supported_text_formats.end(),
                      event->m_target) !=
            m_atoms.supported_text_formats.end();
        if (event->m_target == m_atoms.targets) {
            m_xcb->write_on_window_property(event->m_requestor,
                                            event->m_property,
                                            m_atoms.atom, m_targets);
            m_xcb->notify_window_property_change(
                event->m_requestor, event->m_property, m_atoms.atom,
                event->m_selection);
        } else if (found_format) {
            m_xcb->write_on_window_property(
                event->m_requestor, event->m_property, event->m_target,
                m_copy_data.value());
            m_xcb->notify_window_property_change(
                event->m_requestor, event->m_property, event->m_target,
                event->m_selection);
        } else {
            m_xcb->notify_window_property_change(event->m_requestor, 0,
                                                 event->m_target,
                                                 event->m_selection);
        }
    }

    void handle_selection_notify_event(
        const xcb::SelectionNotifyEvent* event) {
        if (event->m_selection != m_atoms.clipboard ||
            m_paste_data.has_value())
            return;
        m_paste_data = m_xcb->get_our_property_value(m_atoms.buffer);
    }

    const std::shared_ptr<xcb::Xcb> m_xcb;
    const EssentialAtoms m_atoms;
    const std::vector<xcb_atom_t> m_targets;
    std::optional<std::string> m_copy_data;
    std::optional<std::string> m_paste_data;
    std::mutex m_lock;
    std::thread m_event_thread;
    std::atomic<bool> m_stop_event_thread;
};

}  // namespace clipboardxx
