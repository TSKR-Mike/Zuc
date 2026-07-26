/**
 * @file string_kits.hpp
 * @brief Efficient string utilities for C++20
 * @version 0.3
 * @date 2026-07-12
 * @copyright Copyright (c) 2026
 * @note Functions returning std::string may throw std::bad_alloc.
 *       String views are only valid while the underlying data remains alive.
 */

#pragma once

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <iterator>
#include <map>
#include <set>
#include <span>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "common_concepts.hpp"

namespace zuc {

template <Stringable T>
std::string convert_stringable_to_string(const T& v) {
    if constexpr (std::constructible_from<std::string, T>) {
        return std::string(v);
    } else {
        return std::to_string(v);
    }
}

// Substring / slicing (non‑allocating views)

/**
 * @brief Returns a view of a substring starting at pos, taking at most count chars
 * @param s     Input string view
 * @param pos   Start index (must be <= s.size())
 * @param count Maximum number of characters to take (default 1)
 * @return      View of the substring; empty if pos == s.size() or count == 0
 * @note        Truncates if count exceeds remaining length. No allocation.
 */
inline std::string_view string_slice(std::string_view s, size_t pos = 0,
                                     size_t count = 1) noexcept {
    if (s.empty()) {
        return s;
    }
    assert(pos <= s.size() && "slice: pos out of range");
    return s.substr(pos, count);
}

/**
 * @brief Returns a view of the first n characters
 * @param s Input view
 * @param n Number of chars (must be <= s.size())
 * @return  Prefix view
 */
inline std::string_view prefix(std::string_view s, size_t n) noexcept {
    if (s.empty()) {
        return s;
    }
    assert(n <= s.size() && "prefix: n out of range");
    return string_slice(s, 0, n);
}

/**
 * @brief Returns a view of the last n characters
 * @param s Input view
 * @param n Number of chars (must be <= s.size())
 * @return  Suffix view
 */
inline std::string_view suffix(std::string_view s, size_t n) noexcept {
    if (s.empty()) {
        return s;
    }
    assert(n <= s.size() && "suffix: n out of range");
    return string_slice(s, s.size() - n, n);
}

/**
 * @brief Removes the first n characters and returns a view of the remainder
 * @param s Input view
 * @param n Number of chars to remove (must be <= s.size())
 * @return  View starting at index n
 */
inline std::string_view remove_prefix(std::string_view s, size_t n) noexcept {
    if (s.empty()) {
        return s;
    }
    assert(n <= s.size() && "remove_prefix: n out of range");
    return s.substr(n);
}

/**
 * @brief Removes the last n characters and returns a view of the remaining part
 * @param s Input view
 * @param n Number of chars to remove (must be <= s.size())
 * @return  View without the last n chars
 */
inline std::string_view remove_suffix(std::string_view s, size_t n) noexcept {
    if (s.empty()) {
        return s;
    }
    assert(n <= s.size() && "remove_suffix: n out of range");
    return string_slice(s, 0, s.size() - n);
}

// Copy‑based trimming (returns a new std::string)

/**
 * @brief Returns a copy with leading whitespace removed
 * @param s Input string
 * @return  Trimmed copy; empty if all whitespace
 */
inline std::string get_trimmed_left(const std::string& s) {
    if (s.empty()) {
        return {};
    }
    auto it = std::find_if(s.begin(), s.end(), [](char c) {
        return !std::isspace(static_cast<unsigned char>(c));
    });
    return (it == s.end()) ? std::string{} : std::string(it, s.end());
}

/**
 * @brief Returns a copy with trailing whitespace removed
 * @param s Input string
 * @return  Trimmed copy; empty if all whitespace
 */
inline std::string get_trimmed_right(const std::string& s) {
    if (s.empty()) {
        return {};
    }
    auto it = std::find_if(s.rbegin(), s.rend(), [](char c) {
        return !std::isspace(static_cast<unsigned char>(c));
    });
    return (it == s.rend()) ? std::string{} : std::string(s.begin(), it.base());
}

/**
 * @brief Returns a copy with both leading and trailing whitespace removed
 * @param s Input string
 * @return  Trimmed copy; empty if all whitespace
 */
inline std::string get_trimmed(const std::string& s) {
    if (s.empty()) {
        return {};
    }
    auto front = std::find_if(s.begin(), s.end(), [](char c) {
        return !std::isspace(static_cast<unsigned char>(c));
    });
    if (front == s.end()) {
        return {};
    }
    auto back = std::find_if(s.rbegin(), s.rend(), [](char c) {
        return !std::isspace(static_cast<unsigned char>(c));
    });
    return std::string(front, back.base());
}

// Splitting (view‑based, no allocation for views)

/**
 * @brief Splits a string view by a single delimiter char into a vector of views
 * @param s     Input view
 * @param delim Delimiter character (if '\0', returns empty)
 * @return      Vector of views; empty parts preserved. Returns empty if s.empty() or delim == '\0'
 */
inline std::vector<std::string_view> split(std::string_view s,
                                           char delim) noexcept {
    if (s.empty() || delim == '\0') {
        return {};
    }
    std::vector<std::string_view> parts;
    size_t start = 0;
    size_t pos;
    while ((pos = s.find(delim, start)) != std::string_view::npos) {
        parts.emplace_back(s.substr(start, pos - start));
        start = pos + 1;
    }
    parts.emplace_back(s.substr(start));
    return parts;
}

/**
 * @brief Splits a string view by a multi‑character delimiter into a vector of views
 * @param s     Input view
 * @param delim Delimiter string (must not be empty)
 * @return      Vector of views; empty parts preserved. Returns empty if s.empty() or delim.empty()
 */
inline std::vector<std::string_view> split(std::string_view s,
                                           std::string_view delim) noexcept {
    if (s.empty() || delim.empty()) {
        return {};
    }
    std::vector<std::string_view> parts;
    size_t start = 0;
    size_t pos;
    while ((pos = s.find(delim, start)) != std::string_view::npos) {
        parts.emplace_back(s.substr(start, pos - start));
        start = pos + delim.size();
    }
    parts.emplace_back(s.substr(start));
    return parts;
}

/**
 * @brief Splits a string by any of the characters in delims, returning std::string copies
 * @param s      Input view
 * @param delims Set of delimiter characters (can be empty → returns whole string)
 * @return       Vector of strings; empty parts preserved. Returns empty if s.empty()
 */
inline std::vector<std::string> split_by_any(std::string_view s,
                                             std::string_view delims) {
    if (s.empty()) {
        return {};
    }
    std::vector<std::string> result;
    size_t curr = 0;
    while (curr <= s.size()) {
        size_t found = s.find_first_of(delims, curr);
        result.emplace_back(s.substr(curr, found - curr));
        if (found == std::string_view::npos) {
            break;
        }
        curr = found + 1;
    }
    return result;
}

/**
 * @brief Splits by a single char delimiter and returns std::string copies
 * @param s     Input view
 * @param delim Delimiter char
 * @return      Vector of strings; empty if s.empty()
 */
inline std::vector<std::string> split_to_string(std::string_view s,
                                                char delim) {
    auto views = split(s, delim);
    return {views.begin(), views.end()};
}

/**
 * @brief Splits by a string delimiter and returns std::string copies
 * @param s     Input view
 * @param delim Delimiter string
 * @return      Vector of strings; empty if s.empty()
 */
inline std::vector<std::string> split_to_string(std::string_view s,
                                                std::string_view delim) {
    auto views = split(s, delim);
    return {views.begin(), views.end()};
}

// Joining (allocates result)

/**
 * @brief Joins a span of string views with a delimiter
 * @param strings   Span of views
 * @param delimiter String to insert between elements (default empty)
 * @return          Concatenated result; empty if strings is empty
 * @note            Memory is pre‑reserved to avoid reallocations
 */
inline std::string join(std::span<const std::string_view> strings,
                        std::string_view delimiter = "") {
    if (strings.empty()) {
        return {};
    }
    size_t total = 0;
    for (const auto& sv : strings) {
        total += sv.size();
    }
    total += (strings.size() - 1) * delimiter.size();

    std::string result;
    result.reserve(total);

    for (size_t i = 0; i < strings.size(); ++i) {
        result.append(strings[i].data(), strings[i].size());
        if (i + 1 < strings.size()) {
            result.append(delimiter.data(), delimiter.size());
        }
    }
    return result;
}

// In‑place trimming (modifies the original)

/**
 * @brief Removes leading and trailing whitespace in‑place
 * @param s String to trim
 * @return  Reference to s
 */
inline std::string& trim(std::string& s) {
    if (s.empty()) {
        return s;
    }
    auto back = std::find_if(s.rbegin(), s.rend(), [](char c) {
        return !std::isspace(static_cast<unsigned char>(c));
    });
    s.erase(back.base(), s.end());

    auto front = std::find_if(s.begin(), s.end(), [](char c) {
        return !std::isspace(static_cast<unsigned char>(c));
    });
    s.erase(s.begin(), front);
    return s;
}

/**
 * @brief Removes leading whitespace in‑place
 * @param s String to trim
 * @return  Reference to s
 */
inline std::string& trim_left(std::string& s) {
    if (s.empty()) {
        return s;
    }
    auto front = std::find_if(s.begin(), s.end(), [](char c) {
        return !std::isspace(static_cast<unsigned char>(c));
    });
    s.erase(s.begin(), front);
    return s;
}

/**
 * @brief Removes trailing whitespace in‑place
 * @param s String to trim
 * @return  Reference to s
 */
inline std::string& trim_right(std::string& s) {
    if (s.empty()) {
        return s;
    }
    auto back = std::find_if(s.rbegin(), s.rend(), [](char c) {
        return !std::isspace(static_cast<unsigned char>(c));
    });
    s.erase(back.base(), s.end());
    return s;
}

// View‑based trimming (non‑allocating)

/**
 * @brief Returns a view with leading whitespace removed
 * @param s Input view
 * @return  View without leading spaces; empty if all whitespace
 */
inline std::string_view trim_left_view(std::string_view s) noexcept {
    if (s.empty()) {
        return s;
    }
    auto it = std::find_if(s.begin(), s.end(), [](char c) {
        return !std::isspace(static_cast<unsigned char>(c));
    });
    return s.substr(it - s.begin());
}

/**
 * @brief Returns a view with trailing whitespace removed
 * @param s Input view
 * @return  View without trailing spaces; empty if all whitespace
 */
inline std::string_view trim_right_view(std::string_view s) noexcept {
    if (s.empty()) {
        return s;
    }
    auto it = std::find_if(s.rbegin(), s.rend(), [](char c) {
        return !std::isspace(static_cast<unsigned char>(c));
    });
    return s.substr(0, it.base() - s.begin());
}

/**
 * @brief Returns a view with both leading and trailing whitespace removed
 * @param s Input view
 * @return  View trimmed on both ends; empty if all whitespace
 */
inline std::string_view trim_view(std::string_view s) noexcept {
    if (s.empty()) {
        return s;
    }
    return trim_right_view(trim_left_view(s));
}

// Content checks

/**
 * @brief Check if a string contains a substring
 * @param s String to search in
 * @param contain_str Substring to look for
 * @return true if substring is found, false otherwise
 * @note Returns false if the substring is empty
 */
inline bool contains(std::string_view s,
                     std::string_view contain_str) noexcept {
    if (contain_str.empty()) {
        return false;
    }
    return s.find(contain_str) != std::string_view::npos;
}

/**
 * @brief Checks if a string contains any of the given substrings
 * @param s            String to search in
 * @param contain_strs Span of substrings
 * @return             true if at least one is found; false if s empty or span empty
 */
inline bool contains_any(
    std::string_view s,
    std::span<const std::string_view> contain_strs) noexcept {
    if (s.empty() || contain_strs.empty()) {
        return false;
    }
    for (const auto& sub : contain_strs) {
        if (contains(s, sub)) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Checks if a string contains all given substrings
 * @param s            String to search in
 * @param contain_strs Span of substrings
 * @return             true if all found; false if any missing or inputs empty
 */

inline bool contains_all(
    std::string_view s,
    std::span<const std::string_view> contain_strs) noexcept {
    if (s.empty() || contain_strs.empty()) {
        return false;
    }
    for (const auto& sub : contain_strs) {
        if (!contains(s, sub)) {
            return false;
        }
    }
    return true;
}

// Replace / Remove (in‑place and copy versions)

/**
 * @brief Replaces all occurrences of old_str with new_str in‑place
 * @param s       String to modify
 * @param old_str Substring to replace (must not be empty)
 * @param new_str Replacement string
 * @note          Does nothing if s.empty() or old_str.empty(). Does not recursively replace inserted text
 */
inline void replace_all(std::string& s, std::string_view old_str,
                        std::string_view new_str) {
    if (s.empty() || old_str.empty()) {
        return;
    }
    size_t pos = 0;
    while ((pos = s.find(old_str, pos)) != std::string::npos) {
        s.replace(pos, old_str.length(), new_str);
        pos +=
            old_str.length();  // skip the replaced part to avoid infinite loop
    }
}

/**
 * @brief Returns a copy of s with all occurrences of old_str replaced by new_str
 * @param s       Original string
 * @param old_str Substring to replace (must not be empty)
 * @param new_str Replacement string
 * @return        New string with replacements applied
 */
inline std::string get_all_replaced(std::string s, std::string_view old_str,
                                    std::string_view new_str) {
    replace_all(s, old_str, new_str);
    return s;
}

/**
 * @brief Removes all occurrences of a substring from a string in‑place
 * @param s                String to modify
 * @param str_to_be_removed Substring to remove (must not be empty)
 * @return                 Reference to the modified string
 */
inline std::string& remove_all(std::string& s,
                               std::string_view str_to_be_removed) {
    if (s.empty() || str_to_be_removed.empty()) {
        return s;
    }
    size_t pos = 0;
    while ((pos = s.find(str_to_be_removed, pos)) != std::string::npos) {
        s.erase(pos, str_to_be_removed.length());
    }
    return s;
}

/**
 * @brief Removes all occurrences of multiple substrings from a string in‑place
 * @param s                   String to modify
 * @param strs_to_be_removed  Span of substrings to remove
 * @return                    Reference to the modified string
 * @note                      Removal order is sequential; overlapping patterns may interact
 */
inline std::string& remove_all(
    std::string& s, std::span<const std::string_view> strs_to_be_removed) {
    if (s.empty() || strs_to_be_removed.empty()) {
        return s;
    }
    for (const auto& pat : strs_to_be_removed) {
        remove_all(s, pat);
    }
    return s;
}

/**
 * @brief Returns a copy of s with all listed substrings removed
 * @param s                   Original string
 * @param strs_to_be_removed  Span of substrings to remove
 * @return                    New string with removals applied
 */
inline std::string get_all_removed(
    std::string s, std::span<const std::string_view> strs_to_be_removed) {
    remove_all(s, strs_to_be_removed);
    return s;
}

/**
 * @brief Returns a copy of s with all occurrences of a substring removed
 * @param s                   Original string
 * @param str_to_be_removed   Substring to remove
 * @return                    New string with removals applied
 */
inline std::string get_all_removed(std::string s,
                                   std::string_view str_to_be_removed) {
    remove_all(s, str_to_be_removed);
    return s;
}
/**
 * @brief Returns a copy of s that repeats according to the given number
 * @param s      The string to be repeated
 * @param times  How many times should the string repeat
 * @return std::string
 */
inline std::string repeat(std::string& s, size_t times) {
    if (s.empty() || times == 0) {
        return "";
    }
    return join(std::vector<std::string_view>(times, s));
}

template <Stringable T>
inline bool match_any(const T& string_obj,
                      std::span<const std::string_view> all_targets) noexcept {
    if (all_targets.empty()) {
        return false;
    }
    std::string str = convert_stringable_to_string(string_obj);
    for (const auto& target : all_targets) {
        if (str == target) {
            return true;
        }
    }
    return false;
}

template <Stringable T>
inline bool match_any(
    const T& string_obj,
    std::initializer_list<std::string_view> all_targets) noexcept {
    return match_any(string_obj,
                     std::span<const std::string_view>(all_targets));
}

inline std::string_view to_string_view(std::span<const char> s) noexcept {
    return std::string_view(s.data(), s.size());
}


}  // namespace zuc