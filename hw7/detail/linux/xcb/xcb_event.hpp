#pragma once

#include <xcb/xcb.h>

namespace clipboardxx {
namespace xcb {

using Atom   = xcb_atom_t;
using Window = xcb_window_t;

class Event {
public:
    enum class Type {
        kNone = 0,
        kRequestSelection,
        kSelectionClear,
        kSelectionNotify
    };

    constexpr Event(Type type) noexcept : m_type(type) {}

    constexpr Type get_type() const noexcept {
        return m_type;
    }

    virtual ~Event() = default;

private:
    const Type m_type;
};

class RequestSelectionEvent final : public Event {
public:
    constexpr RequestSelectionEvent(Window requestor, Window owner,
                                    Atom selection, Atom target,
                                    Atom property) noexcept
        : Event(Type::kRequestSelection),
          m_requestor(requestor),
          m_owner(owner),
          m_selection(selection),
          m_target(target),
          m_property(property) {}

    const Window m_requestor;
    const Window m_owner;
    const Atom m_selection;
    const Atom m_target;
    const Atom m_property;
};

class SelectionNotifyEvent final : public Event {
public:
    constexpr SelectionNotifyEvent(Window requestor, Atom selection,
                                   Atom target, Atom property) noexcept
        : Event(Type::kSelectionNotify),
          m_requestor(requestor),
          m_selection(selection),
          m_target(target),
          m_property(property) {}

    const Window m_requestor;
    const Atom m_selection;
    const Atom m_target;
    const Atom m_property;
};

class SelectionClearEvent final : public Event {
public:
    SelectionClearEvent(Atom selection)
        : Event(Type::kSelectionClear), m_selection(selection) {}

    const Atom m_selection;
};

}  // namespace xcb
}  // namespace clipboardxx
