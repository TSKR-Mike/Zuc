/**
 * @file common_concepts.hpp
 * @author {TSKR-Mike-CYX@github.com}
 * @brief A file aiming to define common concepts
 * @date 2026-07-15
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#pragma once
#include <concepts>
#include <string>

namespace zuc {

/**
 * @concept Stringable
 * @brief Concept that checks if a type can be converted to a string
 * 
 * A type satisfies Stringable if it can be:
 * - Directly constructed into a std::string, OR
 * - Convertible using std::to_string()
 * 
 * This concept is used throughout the library to enable generic string operations
 * and formatting for various types including built-in types and user-defined types
 * that support string conversion.
 * 
 * @tparam T The type to check
 * 
 * @example
 * Stringable auto value = 42; // int is Stringable (std::to_string)
 * Stringable auto text = "hello"; // const char* is Stringable (constructible)
 * Stringable auto number = 3.14; // double is Stringable (std::to_string)
 */
template <typename T>
concept Stringable = std::constructible_from<std::string, T> ||
                     requires(T&& v) { std::to_string(std::forward<T>(v)); };


/**
 * @concept OneOf
 * @brief Concept that checks if a type T is one of the specified Types
 *
 * A type satisfies OneOf if it matches any of the provided types in the
 * parameter pack. This is useful for type constraints and template
 * specialization when you need to restrict a template parameter to a
 * specific set of allowed types.
 *
 * @tparam T The type to check
 * @tparam Types Variadic template parameter pack of possible types
 *
 * @example
 * template<OneOf<int, float, double> T>
 * void process(T value) { // ... }
 *
 * process(42);      // OK - int is in {int, float, double}
 * process(3.14f);   // OK - float is in {int, float, double}
 * process("hello"); // ERROR - const char* is not in {int, float, double}
 */
template<typename T, typename... Types>
concept OneOf = (std::is_same_v<T, Types> || ...);

/**
 * @concept UnaryPred
 * @brief Concept for unary predicates that take a const reference to T
 * 
 * A type F satisfies UnaryPred if it can be called with a single argument
 * of type const T& and returns a value convertible to bool.
 * 
 * @tparam F The callable type
 * @tparam T The argument type
 * 
 * @example
 * auto is_positive = [](const int& x) { return x > 0; };
 * static_assert(UnaryPred<decltype(is_positive), int>);
 */
template <typename F, typename T>
concept UnaryPred = std::predicate<F, const T&>;

/**
 * @concept BinaryPred
 * @brief Concept for binary predicates that take const references to T1 and T2
 * 
 * A type F satisfies BinaryPred if it can be called with two arguments
 * of types const T1& and const T2& and returns a value convertible to bool.
 * 
 * @tparam F The callable type
 * @tparam T1 The first argument type
 * @tparam T2 The second argument type
 * 
 * @example
 * auto are_equal = [](const int& a, const int& b) { return a == b; };
 * static_assert(BinaryPred<decltype(are_equal), int, int>);
 */
template <typename F, typename T1, typename T2>
concept BinaryPred = std::predicate<F, const T1&, const T2&>;

/**
 * @concept UnaryFunc
 * @brief Concept for unary functions that take a value of type T
 * 
 * A type F satisfies UnaryFunc if it can be called with a single argument
 * of type T. The return type is not constrained.
 * 
 * @tparam F The callable type
 * @tparam T The argument type
 * 
 * @example
 * auto square = [](int x) { return x * x; };
 * static_assert(UnaryFunc<decltype(square), int>);
 */
template <typename F, typename T>
concept UnaryFunc = std::invocable<F, T>;

/**
 * @concept BinaryFunc
 * @brief Concept for binary functions that take values of types T1 and T2
 * 
 * A type F satisfies BinaryFunc if it can be called with two arguments
 * of types T1 and T2. The return type is not constrained.
 * 
 * @tparam F The callable type
 * @tparam T1 The first argument type
 * @tparam T2 The second argument type
 * 
 * @example
 * auto add = [](int a, int b) { return a + b; };
 * static_assert(BinaryFunc<decltype(add), int, int>);
 */
template <typename F, typename T1, typename T2>
concept BinaryFunc = std::invocable<F, T1, T2>;

/**
 * @concept InvocableReturns
 * @brief Concept for callables that return a specific type
 * 
 * A type F satisfies InvocableReturns if it can be called with arguments
 * of types Args... and returns a value convertible to ReturnType.
 * 
 * @tparam F The callable type
 * @tparam ReturnType The required return type
 * @tparam Args The argument types
 * 
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
 * 
 * A type F satisfies InvocableReturnsVoid if it can be called with arguments
 * of types Args... and returns exactly void.
 * 
 * @tparam F The callable type
 * @tparam Args The argument types
 * 
 * @example
 * auto print = [](int x) { std::cout << x << std::endl; };
 * static_assert(InvocableReturnsVoid<decltype(print), int>);
 */
template <typename F, typename... Args>
concept InvocableReturnsVoid = 
    std::invocable<F, Args...> && 
    std::same_as<std::invoke_result_t<F, Args...>, void>;

}