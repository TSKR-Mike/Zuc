/**
 * @file common_concepts.hpp
 * @brief Common concepts for C++20
 * @date 2026-07-15
 * @copyright Copyright (c) 2026
 */
#pragma once
#include <concepts>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace zuc {

/**
 * @concept Stringable
 * @brief Concept that checks if a type can be converted to a string
 * @tparam T The type to check
 * @note A type satisfies Stringable if it can be constructed into std::string
 * or if it's a numeric type that can be converted using std::to_string()
 * @example
 * Stringable auto value = 42; // int is Stringable (std::to_string)
 * Stringable auto text = "hello" // const char* is Stringable (constructible)
 */
template <typename T>
concept Stringable = std::constructible_from<std::string, T> ||
                     ((std::integral<T> || std::floating_point<T>) &&
                      requires(T&& v) { std::to_string(std::forward<T>(v)); });
/**
 * @concept OneOf
 * @brief Concept that checks if a type T is one of the specified Types
 * @tparam T The type to check
 * @tparam Types Variadic template parameter pack of possible types
 * @example
 * template<OneOf<int, float, double> T>
 * void process(T value) { // ... }
 * process(42);      // OK - int is in {int, float, double}
 * process("hello"); // ERROR - const char* is not in {int, float, double}
 */
template <typename T, typename... Types>
concept OneOf = (std::same_as<T, Types> || ...);

template <typename T, typename... Types>
concept IsNotOneOf = !(std::same_as<T, Types> || ...);

/**
 * @concept UnaryPred
 * @brief Concept for unary predicates that take a const reference to T
 * @tparam F The callable type
 * @tparam T The argument type
 * @example
 * auto is_positive = [](const int& x) { return x > 0; };
 * static_assert(UnaryPred<decltype(is_positive), int>);
 */
template <typename F, typename T>
concept UnaryPred = std::predicate<F, const T&>;

/**
 * @concept BinaryPred
 * @brief Concept for binary predicates that take const references to T1 and T2
 * @tparam F The callable type
 * @tparam T1 The first argument type
 * @tparam T2 The second argument type
 * @example
 * auto are_equal = [](const int& a, const int& b) { return a == b; };
 * static_assert(BinaryPred<decltype(are_equal), int, int>);
 */
template <typename F, typename T1, typename T2>
concept BinaryPred = std::predicate<F, const T1&, const T2&>;

/**
 * @concept UnaryFunc
 * @brief Concept for unary functions that take a value of type T
 * @tparam F The callable type
 * @tparam T The argument type
 * @example
 * auto square = [](int x) { return x * x; };
 * static_assert(UnaryFunc<decltype(square), int>);
 */
template <typename F, typename T>
concept UnaryFunc = std::invocable<F, T>;

/**
 * @concept BinaryFunc
 * @brief Concept for binary functions that take values of types T1 and T2
 * @tparam F The callable type
 * @tparam T1 The first argument type
 * @tparam T2 The second argument type
 * @example
 * auto add = [](int a, int b) { return a + b; };
 * static_assert(BinaryFunc<decltype(add), int, int>);
 */
template <typename F, typename T1, typename T2>
concept BinaryFunc = std::invocable<F, T1, T2>;

/**
 * @concept InvocableReturns
 * @brief Concept for callables that return a specific type
 * @tparam F The callable type
 * @tparam ReturnType The required return type
 * @tparam Args The argument types
 * @example
 * auto get_length = [](const std::string& s) { return s.length(); };
 * static_assert(InvocableReturns<decltype(get_length), size_t, std::string>);
 */
template <typename F, typename ReturnType, typename... Args>
concept InvocableReturns =
    std::invocable<F, Args...> &&
    std::convertible_to<std::invoke_result_t<F, Args...>, ReturnType>;

/**
 * @concept InvocableReturnsVoid
 * @brief Concept for callables that return void
 * @tparam F The callable type
 * @tparam Args The argument types
 * @example
 * auto print = [](int x) { std::cout << x << std::endl; };
 * static_assert(InvocableReturnsVoid<decltype(print), int>);
 */
template <typename F, typename... Args>
concept InvocableReturnsVoid =
    std::invocable<F, Args...> &&
    std::same_as<std::invoke_result_t<F, Args...>, void>;

template <typename T>
concept RangeLike = requires(T& t) {
    std::begin(t);
    std::end(t);
};

template <typename T, typename ValueType>
concept RangeLikeElemTypeSpecified = requires(T& t) {
    RangeLike<T>;
    requires std::same_as<
        typename std::iterator_traits<decltype(std::begin(t))>::value_type,
        ValueType>;
};

template <typename T, typename ValueType>
concept RangeLikeElemTypeConvertibleTo = requires(T& t) {
    RangeLike<T>;
    requires std::convertible_to<
        typename std::iterator_traits<decltype(std::begin(t))>::value_type,
        ValueType>;
};

template <typename Range>
    requires RangeLike<Range>
using get_rangelike_value_type =
    std::remove_cv_t<typename std::iterator_traits<decltype(std::begin(
        std::declval<Range&>()))>::value_type>;

template <typename Range>
    requires RangeLike<Range>
using get_rangelike_iterator_type =
    decltype(std::begin(std::declval<Range&>()));

template <typename Range>
concept SupportsStdSize =
    requires(Range& r) { std::size(r) && RangeLike<Range>; };

template <typename Range>
    requires RangeLike<Range> && SupportsStdSize<Range>
inline size_t get_rangelike_size(Range& r) {
    return std::size(r);
}

// Fallback
template <typename Range>
    requires RangeLike<Range> && (!SupportsStdSize<Range>)
inline size_t get_rangelike_size(Range& r) {
    return std::distance(std::begin(r), std::end(r));
}

// Map like containers
template <typename T>
struct is_map_like : std::false_type {};

template <typename Key, typename Value, typename CompareFunc,
          typename Allocator>
struct is_map_like<std::map<Key, Value, CompareFunc, Allocator>>
    : std::true_type {};

template <typename Key, typename Value, typename CompareFunc,
          typename Allocator>
struct is_map_like<std::multimap<Key, Value, CompareFunc, Allocator>>
    : std::true_type {};

template <typename Key, typename Value, typename HashFunc,
          typename KeyEqualFunc, typename Allocator>
struct is_map_like<
    std::unordered_map<Key, Value, HashFunc, KeyEqualFunc, Allocator>>
    : std::true_type {};

template <typename Key, typename Value, typename HashFunc,
          typename KeyEqualFunc, typename Allocator>
struct is_map_like<
    std::unordered_multimap<Key, Value, HashFunc, KeyEqualFunc, Allocator>>
    : std::true_type {};

template <typename T>
inline constexpr bool is_map_like_v = is_map_like<T>::value;

// Set like containers
template <typename>
struct is_set_like : std::false_type {};
template <typename T, typename C, typename A>
struct is_set_like<std::set<T, C, A>> : std::true_type {};

template <typename T, typename C, typename A>
struct is_set_like<std::multiset<T, C, A>> : std::true_type {};

template <typename T, typename C, typename A>
struct is_set_like<std::unordered_set<T, C, A>> : std::true_type {};

template <typename T, typename C, typename A>
struct is_set_like<std::unordered_multiset<T, C, A>> : std::true_type {};

template <typename T>
inline constexpr bool is_set_like_v = is_set_like<T>::value;

template <typename Range, typename ElemType = get_rangelike_value_type<Range>>
concept Insertable =
    RangeLike<Range> &&
    requires(Range r, typename Range::const_iterator it, const ElemType& e) {
        { r.insert(it, e) } -> std::same_as<typename Range::iterator>;
    };

template <typename T>
concept NumericType = std::integral<T> || std::floating_point<T>;

template <typename From, typename To>
concept CastableTo = requires(From&& from) {
    { static_cast<To>(std::forward<From>(from)) };
};

template <typename From, typename To>
    requires CastableTo<From, To>
constexpr decltype(auto) optimal_cast(From&& from) noexcept(
    noexcept(static_cast<To>(std::forward<From>(from)))) {
    return static_cast<To>(std::forward<From>(from));
}

}  // namespace zuc