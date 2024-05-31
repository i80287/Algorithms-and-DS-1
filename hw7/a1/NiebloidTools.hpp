#pragma once

#include <cstdint>
#include <source_location>
#include <string_view>

namespace niebloid_tools {
namespace detail {

template <class AlgorithmNiebloidType>
consteval std::string_view get_algorithm_name_impl(
    const std::source_location location = std::source_location::current()) {
    using namespace std::string_view_literals;
    using std::string_view;

    const string_view s        = location.function_name();
    constexpr auto type_id_str = "AlgorithmNiebloidType = "sv;
    auto lpos            = s.find(type_id_str);
    if (lpos == string_view::npos) {
        return "";
    }
    lpos += type_id_str.size();
    const auto rpos                = std::min(s.find(';', lpos), s.find(']', lpos));
    string_view algo_niebloid_name = s.substr(lpos, rpos - lpos);
    constexpr auto kNiebloidSuffix = "iebloid"sv;
    if (algo_niebloid_name.ends_with(kNiebloidSuffix)) {
        algo_niebloid_name.remove_suffix(kNiebloidSuffix.size());
        if (algo_niebloid_name.ends_with('N') || algo_niebloid_name.ends_with('n')) {
            algo_niebloid_name.remove_suffix(1);
        }
        while (algo_niebloid_name.ends_with('_')) {
            algo_niebloid_name.remove_suffix(1);
        }
    }

    constexpr auto kScopeId = "::"sv;
    const auto scope_op_pos = algo_niebloid_name.rfind(kScopeId);
    if (scope_op_pos != string_view::npos) {
        algo_niebloid_name.remove_prefix(scope_op_pos + kScopeId.size());
    }

    return algo_niebloid_name;
}

}  // namespace detail

template <class AlgorithmNiebloidType>
consteval std::string_view get_algorithm_name() {
    return detail::get_algorithm_name_impl<AlgorithmNiebloidType>();
}

}  // namespace niebloid_tools
