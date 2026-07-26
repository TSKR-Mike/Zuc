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

template <typename Range, typename U>
    requires std::equality_comparable_with<get_rangelike_value_type<Range>,
                                           U> &&
             RangeLike<Range>
constexpr bool contains(const Range& obj, const U& contain_obj) {
    if (std::empty(obj)) return false;
    return std::find(std::begin(obj), std::end(obj), contain_obj) !=
           std::end(obj);
}

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

template <typename Range, typename ElemType = get_rangelike_value_type<Range>>
constexpr bool contains_if(const Range& obj, UnaryPred<ElemType> auto pred) {
    if (std::empty(obj)) {
        return false;
    }
    return std::find_if(std::begin(obj), std::end(obj), pred) != std::end(obj);
}

template <typename T>
std::optional<T> pop_back_value(std::vector<T>& vec) {
    if (vec.empty()) {
        return std::nullopt;
    }
    T ret = std::move(vec.back());
    vec.pop_back();
    return std::move(ret);
}

template <typename KeyType, typename ValueType, typename... Args>
    requires std::constructible_from<ValueType, Args...>
ValueType& get_or_insert(std::unordered_map<KeyType, ValueType>& map,
                         const KeyType& key, Args&&... args) {
    return map.try_emplace(key, std::forward<Args>(args)...).first->second;
}

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
