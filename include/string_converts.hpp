#pragma once

#include <iterator>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "common_concepts.hpp"
#include "container_kits.hpp"
#include "string_kits.hpp"

namespace zuc {

template <RangeLike Range, typename ElemType = get_rangelike_value_type<Range>>
    requires Stringable<ElemType> &&
             IsNotOneOf<Range, std::string, std::string_view> &&
             (!is_map_like_v<Range>)
inline std::string convert_range_to_string(const Range& r) {
    std::string brackets;
    if constexpr (is_set_like_v<Range>) {
        brackets = "{}";
    } else {
        brackets = "[]";
    }
    if (std::size(r) == 0) {
        return brackets;
    }
    std::string result{brackets[0]};
    result.reserve(2 + r.size() * 32); // 32 is a rough estimate of the average length of a string
    bool first = true;
    for (const auto& item : r) {
        if (!first) result.append(", ");
        first = false;
        result.append(convert_stringable_to_string(item));
    }
    result.push_back(brackets[1]);
    result.shrink_to_fit();
    return result;
}
    

template <RangeLike Range, typename KeyType = typename Range::key_type, 
          typename ValueType = typename Range::mapped_type>
    requires is_map_like_v<Range> && Stringable<KeyType> && Stringable<ValueType>
inline std::string convert_range_to_string(const Range& r) {
    if (r.empty()) {
        return "{}";
    }

    std::string result{'{'};
    result.reserve(2 + r.size() * 32); // 32 is a rough estimate of the average length of a string
    bool first = true;
    for (const auto& item : r) {
        if (!first) result.append(", ");
        first = false;
        result.append(convert_stringable_to_string(item.first));
        result.append(": ");
        result.append(convert_stringable_to_string(item.second));
    }
    result.push_back('}');
    result.shrink_to_fit();
    return result;
    
}



}  // namespace zuc