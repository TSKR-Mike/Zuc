/**
 * @file numerics_kits.hpp
 * @brief Safe numeric operations and type selection utilities for C++20
 * @date 2026-07-30
 * @copyright Copyright (c) 2026
 * @note Provides overflow-checked arithmetic operations and compile-time type selection
 *       based on value ranges and maximum values
 */

#pragma once
#include <optional>


namespace zuc {

/**
 * @struct select_by_max_unsigned
 * @brief Compile-time type selector based on maximum unsigned value
 * @tparam Value The maximum value the type needs to hold
 * @note Selects the smallest unsigned integer type that can hold the specified value
 *       Selection order: uint8_t → uint16_t → uint32_t → uint64_t
 * @example
 * using Type1 = select_by_max_unsigned_t<255>;   // uint8_t
 * using Type2 = select_by_max_unsigned_t<256>;   // uint16_t
 * using Type3 = select_by_max_unsigned_t<65535>; // uint16_t
 * using Type4 = select_by_max_unsigned_t<65536>; // uint32_t
 */
template <unsigned long long Value>
struct select_by_max_unsigned {
    static consteval unsigned long long get_value() { return Value; }

    using type = std::conditional_t<
        Value <= 0xFF, std::uint8_t,
        std::conditional_t<Value <= 0xFFFF, std::uint16_t,
                           std::conditional_t<Value <= 0xFFFFFFFF,
                                              std::uint32_t, std::uint64_t>>>;
};

template <unsigned long long Value>
using select_by_max_unsigned_t = typename select_by_max_unsigned<Value>::type;

/**
 * @struct select_by_range
 * @brief Compile-time type selector based on value range
 * @tparam Min Minimum value the type needs to hold
 * @tparam Max Maximum value the type needs to hold
 * @note Selects the smallest integer type (signed or unsigned) that can hold the specified range
 *       For signed ranges: checks both Min and Max bounds
 *       For unsigned ranges: only checks Max bound (Min is assumed to be >= 0)
 * @example
 * using SignedType = select_by_range_t<-100, 100>;    // int8_t
 * using UnsignedType = select_by_range_t<0, 255>;     // uint8_t
 * using LargeType = select_by_range_t<0, 100000>;    // uint32_t
 */
template <long long Min, long long Max>
struct select_by_range {
    static_assert(Min <= Max, "Min must be <= Max");
    static constexpr bool is_signed = Min < 0;

    using type = std::conditional_t<
        is_signed,
        std::conditional_t<
            (Min >= INT8_MIN && Max <= INT8_MAX), int8_t,
            std::conditional_t<
                (Min >= INT16_MIN && Max <= INT16_MAX), int16_t,
                std::conditional_t<(Min >= INT32_MIN && Max <= INT32_MAX),
                                   int32_t, int64_t>>>,
        std::conditional_t<
            (Max <= UINT8_MAX), uint8_t,
            std::conditional_t<
                (Max <= UINT16_MAX), uint16_t,
                std::conditional_t<(Max <= UINT32_MAX), uint32_t, uint64_t>>>>;
};

template <long long Min, long long Max>
using select_by_range_t = typename select_by_range<Min, Max>::type;

template <long long Value>
using select_by_value_t = typename select_by_range<Value, Value>::type;

#ifdef _MSC_VER
#include <intrin.h>
#endif

namespace numerics {

/**
 * @concept is_64bit
 * @brief Concept that identifies 64-bit integer types
 * @tparam T The type to check
 * @note Satisfied by int64_t and uint64_t types
 * @example
 * static_assert(is_64bit<int64_t>);
 * static_assert(is_64bit<uint64_t>);
 * static_assert(!is_64bit<int32_t>);
 */
template <typename T>
concept is_64bit = std::is_same_v<T, int64_t> || std::is_same_v<T, uint64_t>;

/**
 * @brief Performs overflow-checked addition
 * @tparam T Integral type for the operation
 * @param v1 First operand
 * @param v2 Second operand  
 * @return std::optional<T> containing the result, or nullopt if overflow occurs
 * @note Uses compiler intrinsics when available (_addcarry_u64 for MSVC, __builtin_add_overflow for GCC/Clang)
 *       For smaller types, promotes to larger types for safe calculation
 * @example
 * auto result1 = checked_add<uint8_t>(200, 50);  // nullopt (overflow)
 * auto result2 = checked_add<uint8_t>(200, 55);  // 255
 * auto result3 = checked_add<int>(-100, 50);     // -50
 */
template <typename T>
    requires std::integral<T>
std::optional<T> checked_add(T v1, T v2) {
#if defined(_MSC_VER) && !defined(__clang__)  // MSVC only
    if constexpr (std::is_unsigned_v<T>) {
        if constexpr (std::is_same_v<T, uint64_t>) {
            unsigned __int64 result;
            unsigned char carry = _addcarry_u64(0, v1, v2, &result);
            if (carry) return std::nullopt;
            return static_cast<T>(result);
        } else {
            // under small types(uint32_t and below) promote to uint64_t
            uint64_t result =
                static_cast<uint64_t>(v1) + static_cast<uint64_t>(v2);
            if (result > std::numeric_limits<T>::max()) return std::nullopt;
            return static_cast<T>(result);
        }
    } else {
        // signed number required
        if constexpr (std::is_same_v<T, int64_t>) {
            // Check for overflow without converting to unsigned
            if ((v2 > 0 && v1 > std::numeric_limits<T>::max() - v2) ||
                (v2 < 0 && v1 < std::numeric_limits<T>::min() - v2)) {
                return std::nullopt;
            }
            return v1 + v2;
        } else {
            // under small types(int32_t and below) promote to int64_t
            int64_t result =
                static_cast<int64_t>(v1) + static_cast<int64_t>(v2);
            if (result > std::numeric_limits<T>::max() ||
                result < std::numeric_limits<T>::min()) {
                return std::nullopt;
            }
            return static_cast<T>(result);
        }
    }
#else
    // GCC / Clang
    T result;
    if (__builtin_add_overflow(v1, v2, &result)) return std::nullopt;
    return result;
#endif
}

/**
 * @brief Performs overflow-checked subtraction
 * @tparam T Integral type for the operation
 * @param v1 First operand (minuend)
 * @param v2 Second operand (subtrahend)
 * @return std::optional<T> containing the result, or nullopt if overflow occurs
 * @note Uses compiler intrinsics when available (_subborrow_u64 for MSVC, __builtin_sub_overflow for GCC/Clang)
 *       For unsigned types, returns nullopt if v1 < v2
 *       For smaller types, promotes to larger types for safe calculation
 * @example
 * auto result1 = checked_sub<uint8_t>(100, 150); // nullopt (underflow)
 * auto result2 = checked_sub<uint8_t>(200, 50);  // 150
 * auto result3 = checked_sub<int>(-100, 50);     // -150
 */
template <typename T>
    requires std::integral<T>
std::optional<T> checked_sub(T v1, T v2) {
#if defined(_MSC_VER) && !defined(__clang__)  // MSVC only
    // unsigned number required
    if constexpr (std::is_unsigned_v<T>) {
        if constexpr (std::is_same_v<T, uint64_t>) {
            unsigned __int64 result;
            unsigned char borrow = _subborrow_u64(0, v1, v2, &result);
            if (borrow) return std::nullopt;  // v1 < v2
            return static_cast<T>(result);
        } else {
            if (v1 < v2) return std::nullopt;
            return static_cast<T>(v1 - v2);
        }
    } else {
        if constexpr (std::is_same_v<T, int64_t>) {
            // Check for overflow conditions in subtraction
            if ((v2 > 0 && v1 < std::numeric_limits<T>::min() + v2) ||
                (v2 < 0 && v1 > std::numeric_limits<T>::max() + v2)) {
                return std::nullopt;
            }
            return v1 - v2;
        } else {
            int64_t result =
                static_cast<int64_t>(v1) - static_cast<int64_t>(v2);
            if (result > std::numeric_limits<T>::max() ||
                result < std::numeric_limits<T>::min()) {
                return std::nullopt;
            }
            return static_cast<T>(result);
        }
    }
#else
    // GCC / Clang
    T result;
    if (__builtin_sub_overflow(v1, v2, &result)) return std::nullopt;
    return result;
#endif
}

/**
 * @brief Performs overflow-checked multiplication
 * @tparam T Integral type for the operation
 * @param v1 First operand
 * @param v2 Second operand
 * @return std::optional<T> containing the result, or nullopt if overflow occurs
 * @note Uses compiler intrinsics when available (_umul128 for MSVC, __builtin_mul_overflow for GCC/Clang)
 *       For signed multiplication, performs comprehensive overflow checking across all sign combinations
 *       For smaller types, promotes to larger types for safe calculation
 * @example
 * auto result1 = checked_mul<uint8_t>(100, 3);   // nullopt (overflow: 300 > 255)
 * auto result2 = checked_mul<uint8_t>(50, 4);    // 200
 * auto result3 = checked_mul<int>(-100, 2);     // -200
 */
template <typename T>
    requires std::integral<T>
std::optional<T> checked_mul(T v1, T v2) {
#if defined(_MSC_VER) && !defined(__clang__)  // MSVC only
    if constexpr (std::is_unsigned_v<T>) {
        if constexpr (std::is_same_v<T, uint64_t>) {
            unsigned __int64 hi, lo;
            lo = _umul128(v1, v2, &hi);
            if (hi != 0)
                return std::nullopt;  // high bit is not zero, overflow happens
            return static_cast<T>(lo);
        } else {
            // under small types(uint32_t and below) promote to uint64_t
            uint64_t result =
                static_cast<uint64_t>(v1) * static_cast<uint64_t>(v2);
            if (result > std::numeric_limits<T>::max()) return std::nullopt;
            return static_cast<T>(result);
        }
    } else {
        if constexpr (std::is_same_v<T, int64_t>) {
            // Check for overflow in multiplication
            if (v1 == 0 || v2 == 0) return 0;
            if (v1 > 0 && v2 > 0) {
                if (v1 > std::numeric_limits<T>::max() / v2) return std::nullopt;
            } else if (v1 < 0 && v2 < 0) {
                if (v1 < std::numeric_limits<T>::max() / v2) return std::nullopt;
            } else if (v1 > 0 && v2 < 0) {
                if (v2 < std::numeric_limits<T>::min() / v1) return std::nullopt;
            } else if (v1 < 0 && v2 > 0) {
                if (v1 < std::numeric_limits<T>::min() / v2) return std::nullopt;
            }
            return v1 * v2;
        } else {
            // under small types(int32_t and below) promote to int64_t
            int64_t result =
                static_cast<int64_t>(v1) * static_cast<int64_t>(v2);
            if (result > std::numeric_limits<T>::max() ||
                result < std::numeric_limits<T>::min()) {
                return std::nullopt;
            }
            return static_cast<T>(result);
        }
    }
#else
    // GCC / Clang
    T result;
    if (__builtin_mul_overflow(v1, v2, &result)) return std::nullopt;
    return result;
#endif
}
}  // namespace numerics

}  // namespace zuc