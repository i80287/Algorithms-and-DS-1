#pragma once

#include <string>

namespace clipboardxx {

template <class ClipboardProviderInterface, class ClipboardProviderType>
concept LinuxClipboardProviderConcept =
    std::derived_from<ClipboardProviderType, ClipboardProviderInterface> &&
    requires(ClipboardProviderType clp, std::string text) {
        { clp.copy(text) } -> std::same_as<void>;
        { clp.paste() } -> std::same_as<std::string>;
    };

template <class DerivedClipboardProvider>
class LinuxClipboardProvider {
public:
    void copy(const std::string &text)
        requires LinuxClipboardProviderConcept<LinuxClipboardProvider,
                                               DerivedClipboardProvider>
    {
        static_cast<DerivedClipboardProvider *>(this)->copy(text);
    }
    std::string paste()
        requires LinuxClipboardProviderConcept<LinuxClipboardProvider,
                                               DerivedClipboardProvider>
    {
        return static_cast<DerivedClipboardProvider *>(this)->paste();
    }
};

}  // namespace clipboardxx
