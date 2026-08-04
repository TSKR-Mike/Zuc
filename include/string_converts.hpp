/**
 * @file string_converts.hpp
 * @brief String conversion utilities for C++20
 * @date 2026-07-30
 * @copyright Copyright (c) 2026
 * @note Provides utilities for converting between strings, ranges, bytes, and hex representations
 *       Includes safe numeric parsing and container-to-string conversion functions
 */

#pragma once

#include <charconv>
#include <string_view>
#include <system_error>
#include <cstddef>
#include <iterator>
#include <map>
#include <optional>
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

/**
 * @brief Converts a range (container) to its string representation
 * @tparam Range Range type to convert
 * @tparam ElemType Element type (deduced if not specified)
 * @param r The range to convert
 * @return String representation of the range with elements separated by commas
 * @note Uses [] for sequence containers and {} for set-like containers.
 *       Each element is converted using convert_stringable_to_string().
 *       Returns "[]" or "{}" for empty ranges.
 * @example
 * std::vector<int> vec = {1, 2, 3};
 * std::string str = convert_range_to_string(vec); // "[1, 2, 3]"
 * 
 * std::set<std::string> set = {"a", "b"};
 * std::string str2 = convert_range_to_string(set); // "{a, b}"
 */
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

/**
 * @brief Converts a map-like container to its string representation
 * @tparam Range Map type to convert
 * @tparam KeyType Key type (deduced from Range::key_type)
 * @tparam ValueType Value type (deduced from Range::mapped_type)
 * @param r The map to convert
 * @return String representation of the map with key-value pairs
 * @note Uses {} as delimiters and ": " to separate keys from values.
 *       Each key and value is converted using convert_stringable_to_string().
 *       Returns "{}" for empty maps.
 * @example
 * std::map<std::string, int> map = {{"a", 1}, {"b", 2}};
 * std::string str = convert_range_to_string(map); // "{a: 1, b: 2}"
 */
template <typename Range>
    requires is_map_like_v<Range> &&
             requires { typename Range::key_type; typename Range::mapped_type; } &&
             Stringable<typename Range::key_type> &&
             Stringable<typename Range::mapped_type>
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

/**
 * @brief Converts a trivially copyable object to a byte span
 * @tparam T Type of object to convert (must be trivially copyable)
 * @param t The object to convert
 * @return Span of bytes representing the object's memory layout
 * @note Provides a view into the object's memory without copying.
 *       The returned span is only valid as long as the object is alive.
 * @example
 * int value = 42;
 * auto bytes = convert_to_bytes(value);
 * // bytes points to the memory representation of value
 */
// To-bytes functions
template <typename T>
    requires std::is_trivially_copyable_v<T>
std::span<const std::byte> convert_to_bytes(const T& t) {
    return std::as_bytes(std::span<const T>(&t, 1));
}

/**
 * @brief Converts a span of trivially copyable objects to a byte span
 * @tparam T Element type (must be trivially copyable)
 * @param span The span to convert
 * @return Span of bytes representing the span's memory layout
 * @note Provides a view into the span's memory without copying.
 *       The returned span is only valid as long as the original span is valid.
 * @example
 * std::vector<int> vec = {1, 2, 3};
 * auto bytes = convert_to_bytes(std::span(vec));
 * // bytes points to the memory representation of the vector elements
 */
template <typename T>
    requires std::is_trivially_copyable_v<T> &&
             IsNotOneOf<T, std::string, std::string_view>
std::span<const std::byte> convert_to_bytes(std::span<const T> span) {
    return std::as_bytes(span);
}

/**
 * @brief Converts a string_view to a byte span
 * @param s The string_view to convert
 * @return Span of bytes representing the string's characters
 * @note Provides a view into the string's memory without copying.
 *       The returned span is only valid as long as the string_view is valid.
 * @example
 * std::string_view text = "Hello";
 * auto bytes = convert_to_bytes(text);
 * // bytes points to the memory representation of the string
 */
inline std::span<const std::byte> convert_to_bytes(std::string_view s) {
    return std::span<const std::byte>(
        reinterpret_cast<const std::byte*>(s.data()), s.size());
}

/**
 * @brief Converts a string to a byte span
 * @param s The string to convert
 * @return Span of bytes representing the string's characters
 * @note Provides a view into the string's memory without copying.
 *       The returned span is only valid as long as the string is alive.
 * @example
 * std::string text = "Hello";
 * auto bytes = convert_to_bytes(text);
 * // bytes points to the memory representation of the string
 */
inline std::span<const std::byte> convert_to_bytes(const std::string& s) {
    return convert_to_bytes(std::string_view(s));
}

/**
 * @brief Converts a single byte to its hexadecimal string representation
 * @param b The byte to convert
 * @return Two-character string containing hexadecimal representation
 * @note Converts the byte to 2 hexadecimal characters using uppercase letters.
 *       Each nibble (4 bits) is converted to one hex character.
 * @example
 * auto hex = convert_to_hex(std::byte{0xAB}); // Returns "AB"
 */
// To-hex functions
thread_local inline const char hex_chars[] = "0123456789ABCDEF";

inline std::string convert_to_hex(std::byte b) {
    unsigned char val = static_cast<unsigned char>(b);
    std::string result;
    result.reserve(2);
    result.push_back(hex_chars[val >> 4]);      // High nibble
    result.push_back(hex_chars[val & 0x0F]);    // Low nibble
    return result;
}
/**
 * @brief Converts a byte span to its hexadecimal string representation
 * @param bytes The byte span to convert
 * @return String containing hexadecimal representation of all bytes
 * @note Each byte is converted to 2 hexadecimal characters using uppercase letters.
 *       The result has exactly bytes.size() * 2 characters.
 * @example
 * std::vector<std::byte> data = {std::byte{0xAB}, std::byte{0xCD}};
 * std::string hex = convert_to_hex(std::span(data)); // "ABCD"
 */
inline std::string convert_to_hex(std::span<const std::byte> bytes) {
    std::string result;
    result.reserve(bytes.size() * 2);
    for (auto byte : bytes) {
        result.append(convert_to_hex(byte));
    }
    return result;
}

/**
 * @brief Converts a trivially copyable value to its hexadecimal string representation
 * @tparam T Type of value to convert (must be trivially copyable)
 * @param val The value to convert
 * @return String containing hexadecimal representation of the value's bytes
 * @note Converts the memory representation of the value to hexadecimal.
 *       Useful for debugging and binary data inspection.
 * @example
 * int value = 0x12345678;
 * std::string hex = convert_to_hex(value);
 * // Returns hexadecimal representation of the int's bytes
 */
template <typename T>
    requires std::is_trivially_copyable_v<T> &&
             IsNotOneOf<T, std::string, std::string_view>
inline std::string convert_to_hex(T& val) {
    return convert_to_hex(convert_to_bytes(val));
}

/**
 * @brief Converts a string_view to its hexadecimal string representation
 * @param sv The string_view to convert
 * @return String containing hexadecimal representation of the string's bytes
 * @note Each character in the string is converted to 2 hexadecimal characters.
 * @example
 * std::string_view text = "Hi";
 * std::string hex = convert_to_hex(text); // "4869" (ASCII for 'H' and 'i')
 */
inline std::string convert_to_hex(std::string_view sv) {
    return convert_to_hex(convert_to_bytes(sv));
}

/**
 * @brief Converts a string to its hexadecimal string representation
 * @param sv The string to convert
 * @return String containing hexadecimal representation of the string's bytes
 * @note Each character in the string is converted to 2 hexadecimal characters.
 * @example
 * std::string text = "Hi";
 * std::string hex = convert_to_hex(text); // "4869" (ASCII for 'H' and 'i')
 */
inline std::string convert_to_hex(const std::string& sv) {
    return convert_to_hex(convert_to_bytes(sv));
}



/**
 * @brief Attempts to convert a string to a numeric value
 * @tparam T Numeric type to convert to (integral or floating point)
 * @param sv String view containing the numeric representation
 * @return std::optional<T> containing the converted value, or nullopt if conversion fails
 * @note Uses std::from_chars for fast, locale-independent conversion.
 *       The entire string must be consumed for successful conversion.
 *       Returns nullopt for empty strings or invalid formats.
 * @example
 * auto result1 = try_convert_string_to_numerics<int>("123");   // 123
 * auto result2 = try_convert_string_to_numerics<double>("3.14"); // 3.14
 * auto result3 = try_convert_string_to_numerics<int>("abc");   // nullopt
 */
template <typename T>
    requires NumericType<T>
std::optional<T> try_convert_string_to_numerics(std::string_view sv) {
    if (sv.empty()) {
        return std::nullopt;
    }
    T value;
    auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), value);
    if (ec == std::errc() && ptr == sv.data() + sv.size()) {
        return value;
    } else {
        return std::nullopt;
    }
}

/**
 * @brief Checks if a string represents a valid numeric value
 * @param sv String view to check
 * @return true if the string can be converted to a number, false otherwise
 * @note Attempts to convert the string to double to determine if it's numeric.
 *       Returns false for empty strings or invalid numeric formats.
 * @example
 * bool result1 = is_numeric("123");    // true
 * bool result2 = is_numeric("3.14");   // true
 * bool result3 = is_numeric("hello");  // false
 * bool result4 = is_numeric("");       // false
 */
inline bool is_numeric(std::string_view sv) {
    return try_convert_string_to_numerics<double>(sv).has_value();
}


}  // namespace zuc