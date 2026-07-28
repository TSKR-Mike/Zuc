#pragma once

#include <cstddef>
#include <iterator>
#include <map>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "common_concepts.hpp"
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
    result.reserve(
        2 +
        r.size() *
            32);  // 32 is a rough estimate of the average length of a string
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
    requires is_map_like_v<Range> && Stringable<KeyType> &&
             Stringable<ValueType>
inline std::string convert_range_to_string(const Range& r) {
    if (r.empty()) {
        return "{}";
    }

    std::string result{'{'};
    result.reserve(
        2 +
        r.size() *
            32);  // 32 is a rough estimate of the average length of a string
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

// To-bytes functions
template <typename T>
    requires std::is_trivially_copyable_v<T>
std::span<const std::byte> convert_to_bytes(const T& t) {
    return std::as_bytes(std::span<const T>(&t, 1));
}

template <typename T>
    requires std::is_trivially_copyable_v<T> &&
             IsNotOneOf<T, std::string, std::string_view>
std::span<const std::byte> convert_to_bytes(std::span<const T> span) {
    return std::as_bytes(span);
}

inline std::span<const std::byte> convert_to_bytes(std::string_view s) {
    return std::span<const std::byte>(
        reinterpret_cast<const std::byte*>(s.data()), s.size());
}

inline std::span<const std::byte> convert_to_bytes(const std::string& s) {
    return convert_to_bytes(s);
}

// To-hex functions
thread_local inline const char hex_chars[] = "0123456789ABCDEF";

inline auto convert_to_hex(std::byte b) -> char {
    unsigned char val = static_cast<unsigned char>(b);
    return hex_chars[val >> 4] + hex_chars[val & 0xF];
}
inline std::string convert_to_hex(std::span<const std::byte> bytes) {
    std::string result;
    result.reserve(bytes.size() * 2);
    for (auto byte : bytes) {
        result.append(2, convert_to_hex(byte));
    }
    return result;
}

template <typename T>
    requires std::is_trivially_copyable_v<T> &&
             IsNotOneOf<T, std::string, std::string_view>
inline std::string convert_to_hex(T& val) {
    return convert_bytes_to_hex(convert_to_bytes(val));
}

inline std::string convert_to_hex(std::string_view sv) {
    return convert_to_hex(convert_to_bytes(sv));
}

inline std::string convert_to_hex(std::string sv) {
    return convert_to_hex(convert_to_bytes(sv));
}

}  // namespace zuc