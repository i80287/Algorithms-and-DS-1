#pragma once

#include <algorithm>
#include <iostream>
#include <source_location>
#include <string_view>
#include <utility>

namespace detail {

namespace {

template <bool ReturnEmptyIfFails = false>
consteval std::string_view get_typename_impl(std::source_location location) noexcept(
    ReturnEmptyIfFails) {
    const std::string_view s               = location.function_name();
    constexpr std::string_view type_id_str = "T = ";
    std::size_t lpos                       = s.find(type_id_str);
    if constexpr (ReturnEmptyIfFails) {
        if (lpos == std::string_view::npos) {
            return "";
        }
    }
    lpos += type_id_str.size();
    std::size_t rpos = std::min(s.find(';', lpos), s.find(']', lpos));
    if constexpr (ReturnEmptyIfFails) {
        if (rpos == std::string_view::npos) {
            return "";
        }
    }
    return s.substr(lpos, rpos - lpos);
}
}  // namespace

}  // namespace detail

void log(const std::source_location location = std::source_location::current()) {
    std::clog << "file " << location.file_name() << ", line " << location.line() << ':'
              << location.column() << ' ' << location.function_name() << '\n';
}

template <class T, bool ReturnEmptyIfFails = false>
consteval std::string_view get_typename() noexcept(
    noexcept(detail::get_typename_impl<ReturnEmptyIfFails>(std::source_location::current()))) {
    return detail::get_typename_impl<ReturnEmptyIfFails>(std::source_location::current());
}

template <class T, bool ReturnEmptyIfFails = false>
consteval std::string_view get_typename(T&&) noexcept(
    noexcept(detail::get_typename_impl<ReturnEmptyIfFails>(std::source_location::current()))) {
    // Should we remove reference by std::remove_reference_t<T>?
    return get_typename<T, ReturnEmptyIfFails>();
}
