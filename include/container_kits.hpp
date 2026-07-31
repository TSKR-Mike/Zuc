/**
 * @file container_kits.hpp
 * @brief Container utilities and operations for C++20
 * @date 2026-07-30
 * @copyright Copyright (c) 2026
 * @note Provides generic container operations including element search, safe removal,
 *       map operations, and container slicing. Works with any range-like container.
 */

#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <concepts>
#include <cstring>
#include <deque>
#include <iterator>
#include <list>
#include <map>
#include <optional>
#include <span>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "common_concepts.hpp"

namespace zuc {

/**
 * @brief Checks if a range contains a specific element
 * @tparam Range Range type to search in
 * @tparam U Type of element to search for
 * @param obj The range to search in
 * @param contain_obj The element to look for
 * @return true if the element is found, false otherwise
 * @note Performs a linear search using std::find. Works with any range-like container.
 *       Returns false for empty ranges.
 * @example
 * std::vector<int> vec = {1, 2, 3, 4, 5};
 * bool found = contains(vec, 3); // true
 * bool not_found = contains(vec, 6); // false
 */
template <typename Range, typename U>
    requires std::equality_comparable_with<get_rangelike_value_type<Range>,
                                           U> &&
             RangeLike<Range>
constexpr bool contains(const Range& obj, const U& contain_obj) {
    if (std::empty(obj)) return false;
    return std::find(std::begin(obj), std::end(obj), contain_obj) !=
           std::end(obj);
}

/**
 * @brief Checks if a range contains any of the specified elements
 * @tparam Range1 Type of the range to search in
 * @tparam Range2 Type of the range containing elements to search for
 * @param obj The range to search in
 * @param contain_objs Range of elements to look for
 * @return true if any element from contain_objs is found in obj, false otherwise
 * @note Returns false if either range is empty. Performs linear search for each element.
 * @example
 * std::vector<int> vec = {1, 2, 3, 4, 5};
 * std::array<int, 3> search = {2, 6, 8};
 * bool found = contains_any(vec, search); // true (2 is in vec)
 */
template <typename Range1, typename Range2>
    requires std::equality_comparable_with<get_rangelike_value_type<Range1>,
                                           get_rangelike_value_type<Range2>> &&
             RangeLike<Range1> && RangeLike<Range2>
constexpr bool contains_any(const Range1& obj, const Range2& contain_objs) {
    if (std::empty(contain_objs) || std::empty(obj)) return false;
    for (const auto& x : contain_objs) {
        if (contains(obj, x)) return true;
    }
    return false;
}

/**
 * @brief Checks if a range contains any element satisfying a predicate
 * @tparam Range Range type to search in
 * @tparam ElemType Type of elements in the range (deduced if not specified)
 * @param obj The range to search in
 * @param pred Unary predicate function to test each element
 * @return true if any element satisfies the predicate, false otherwise
 * @note Returns false for empty ranges. Uses std::find_if for efficient searching.
 * @example
 * std::vector<int> vec = {1, 2, 3, 4, 5};
 * bool has_even = contains_if(vec, [](int x) { return x % 2 == 0; }); // true
 * bool has_negative = contains_if(vec, [](int x) { return x < 0; }); // false
 */
template <typename Range, typename ElemType = get_rangelike_value_type<Range>>
constexpr bool contains_if(const Range& obj, UnaryPred<ElemType> auto pred) {
    if (std::empty(obj)) {
        return false;
    }
    return std::find_if(std::begin(obj), std::end(obj), pred) != std::end(obj);
}

/**
 * @brief Safely removes and returns the last element from a vector
 * @tparam T Type of elements in the vector
 * @param vec The vector to pop from
 * @return std::optional<T> containing the removed element, or nullopt if vector is empty
 * @note Uses move semantics for efficient element extraction. Returns empty optional for empty vectors.
 * @example
 * std::vector<int> vec = {1, 2, 3};
 * auto result = pop_back_value(vec); // 3, vec is now {1, 2}
 * auto empty = pop_back_value(std::vector<int>{}); // nullopt
 */
template <typename T>
std::optional<T> pop_back_value(std::vector<T>& vec) {
    if (vec.empty()) {
        return std::nullopt;
    }
    T ret = std::move(vec.back());
    vec.pop_back();
    return std::move(ret);
}

/**
 * @brief Gets or inserts a value in an unordered_map with perfect forwarding
 * @tparam KeyType Type of the map keys
 * @tparam ValueType Type of the map values
 * @tparam Args Types of arguments for value construction
 * @param map The unordered_map to operate on
 * @param key The key to look up or insert
 * @param args Arguments to forward to ValueType constructor if insertion is needed
 * @return Reference to the value associated with the key
 * @note Uses try_emplace for efficient insertion that only constructs value if needed.
 *       Perfect forwards arguments to the value constructor.
 * @example
 * std::unordered_map<std::string, std::string> map;
 * auto& value = get_or_insert(map, "key", "default_value");
 * // value is "default_value", map now contains {"key": "default_value"}
 */
template <typename KeyType, typename ValueType, typename... Args>
    requires std::constructible_from<ValueType, Args...>
ValueType& get_or_insert(std::unordered_map<KeyType, ValueType>& map,
                         const KeyType& key, Args&&... args) {
    return map.try_emplace(key, std::forward<Args>(args)...).first->second;
}

/**
 * @brief Gets or inserts a value in an unordered_map with a default value
 * @tparam KeyType Type of the map keys
 * @tparam ValueType Type of the map values
 * @tparam InsertElemType Type of the default value to insert
 * @tparam InsertKeyType Type of the key to insert
 * @param map The unordered_map to operate on
 * @param key The key to look up or insert
 * @param default_value The default value to use if insertion is needed
 * @return Reference to the value associated with the key
 * @note If the key doesn't exist, inserts the key with the default value.
 *       Supports type conversion between key and value types.
 * @example
 * std::unordered_map<int, std::string> map;
 * auto& value = get_or_insert_default(map, 1, "default");
 * // value is "default", map now contains {1: "default"}
 */
template <typename KeyType, typename ValueType, typename InsertElemType,
          typename InsertKeyType>
    requires std::constructible_from<ValueType, InsertElemType> &&
             std::constructible_from<KeyType, InsertKeyType>
ValueType& get_or_insert_default(std::unordered_map<KeyType, ValueType>& map,
                                 const InsertKeyType& key,
                                 const InsertElemType& default_value) {
    auto [it, inserted] = map.try_emplace(
        static_cast<KeyType>(key), static_cast<ValueType>(default_value));
    return it->second;
}

template <typename Range, typename ElemType = get_rangelike_value_type<Range>>
    requires RangeLike<Range> && IsNotOneOf<Range, std::string, std::string_view>
constexpr std::vector<ElemType> container_slice(const Range& obj, size_t start,
                                                size_t end, size_t step = 1) {
    size_t total_size = std::size(obj);
    if (total_size == 0) {
        return {};
    }
    assert(start < total_size && end <= total_size &&
           "start or end out of range");
    assert(end >= start && "end should be greater than start");
    if (end - start < step) {
        return {};
    }

    std::vector<ElemType> result;
    for (size_t i = start; i < end; i += step) {
        result.push_back(obj[i]);
    }
    return result;
}

template <typename ContainerType,
          typename ElemType = typename ContainerType::value_type>
    requires OneOf<ContainerType, std::list<ElemType>, std::deque<ElemType>>
std::optional<ElemType> pop_front_value(ContainerType& container) {
    if (container.empty()) {
        return std::nullopt;
    }
    ElemType ret = std::move(container.front());
    container.pop_front();
    return std::move(ret);
}

template <typename Range, typename ElemType = get_rangelike_value_type<Range>>
    requires RangeLike<Range>
constexpr void erase_if(Range& obj, UnaryPred<ElemType> auto pred) {
    if (std::empty(obj)) {
        return;
    }
    for (auto it = std::begin(obj); it != std::end(obj);) {
        if (pred(*it)) {
            it = obj.erase(it);
        } else {
            ++it;
        }
    }
}

template <typename TransformType, typename Range, typename F,
          typename ElemType = get_rangelike_value_type<Range>>
    requires InvocableReturns<F, TransformType, ElemType> && RangeLike<Range>
constexpr std::vector<TransformType> transform_all_to_vector(
    const Range& range,
    F transform_func) {
    if (std::size(range) == 0) {
        return {};
    }
    std::vector<TransformType> result;
    for (auto it = std::begin(range); it != std::end(range); ++it) {
        result.push_back(transform_func(*it));
    }
    return result;
}

template <typename MapType, typename KeyType = MapType::key_type>
    requires is_map_like_v<MapType>
std::unordered_set<KeyType> get_map_keys(const MapType& m) {
    if (std::size(m) == 0) {
        return {};
    }
    std::unordered_set<KeyType> result;
    result.reserve(m.size());
    for (const auto& [k, v] : m) {
        result.insert(k);
    }
    return result;
}

template <typename MapType, typename ValueType = MapType::mapped_type>
    requires is_map_like_v<MapType>
std::unordered_set<ValueType> get_map_values(const MapType& m) {
    if (std::size(m) == 0) {
        return {};
    }
    std::unordered_set<ValueType> result;
    result.reserve(m.size());
    for (const auto& [k, v] : m) {
        result.insert(v);
    }
    return result;
}

template <typename Range, typename ElemType = get_rangelike_value_type<Range>>
    requires RangeLike<Range> && Insertable<Range, ElemType>
std::decay_t<Range> merge(const Range& r1, const Range& r2) {
    if (std::size(r1) == 0) {
        return r2;
    }
    if (std::size(r2) == 0) {
        return r1;
    }
    std::decay_t<Range> result;
    if constexpr (requires { result.reserve(std::size(r1) + std::size(r2)); }) {
        result.reserve(std::size(r1) + std::size(r2));
    }
    for (auto& it : r1) {
        result.insert(std::end(result), it);
    }
    for (auto& it : r2) {
        result.insert(std::end(result), it);
    }
    return result;
}

}  // namespace zuc