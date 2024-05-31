#pragma once

#include <concepts>
#include <string>

namespace clipboardxx {

template <class ClipboardInterface, class ClipboardType>
concept ClipboardConcept =
    std::derived_from<ClipboardType, ClipboardInterface> &&
    requires(const ClipboardType clp, std::string text) {
        { clp.copy(text) } -> std::same_as<void>;
        { clp.paste() } -> std::same_as<std::string>;
    };

template <class DerivedClipboard>
class ClipboardInterface {
public:
    void copy(const std::string &text) const
        requires ClipboardConcept<ClipboardInterface, DerivedClipboard>
    {
        static_cast<DerivedClipboard *>(this)->copy(text);
    }
    std::string paste() const
        requires ClipboardConcept<ClipboardInterface, DerivedClipboard>
    {
        return static_cast<DerivedClipboard *>(this)->paste();
    }
};

}  // namespace clipboardxx
