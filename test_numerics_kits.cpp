// test_numerics_kits.cpp
#include "doctest.h"

#include "include/numerics_kits.hpp"  // adjust path if needed

#include <cstdint>
#include <limits>
#include <type_traits>

using namespace zuc;

// -----------------------------------------------------------------------------
// Tests for select_by_max_unsigned
// -----------------------------------------------------------------------------
TEST_CASE("select_by_max_unsigned") {
    static_assert(std::is_same_v<select_by_max_unsigned_t<0>, uint8_t>);
    static_assert(std::is_same_v<select_by_max_unsigned_t<255>, uint8_t>);
    static_assert(std::is_same_v<select_by_max_unsigned_t<256>, uint16_t>);
    static_assert(std::is_same_v<select_by_max_unsigned_t<65535>, uint16_t>);
    static_assert(std::is_same_v<select_by_max_unsigned_t<65536>, uint32_t>);
    static_assert(std::is_same_v<select_by_max_unsigned_t<4294967295ULL>, uint32_t>);
    static_assert(std::is_same_v<select_by_max_unsigned_t<4294967296ULL>, uint64_t>);
    static_assert(std::is_same_v<select_by_max_unsigned_t<18446744073709551615ULL>, uint64_t>);
}

// -----------------------------------------------------------------------------
// Tests for select_by_range
// -----------------------------------------------------------------------------
TEST_CASE("select_by_range") {
    // Unsigned ranges
    static_assert(std::is_same_v<select_by_range_t<0, 0>, uint8_t>);
    static_assert(std::is_same_v<select_by_range_t<0, 255>, uint8_t>);
    static_assert(std::is_same_v<select_by_range_t<0, 256>, uint16_t>);
    static_assert(std::is_same_v<select_by_range_t<100, 65535>, uint16_t>);
    static_assert(std::is_same_v<select_by_range_t<0, 65536>, uint32_t>);
    static_assert(std::is_same_v<select_by_range_t<1, 4294967295ULL>, uint32_t>);
    static_assert(std::is_same_v<select_by_range_t<0, 4294967296ULL>, uint64_t>);

    // Signed ranges
    static_assert(std::is_same_v<select_by_range_t<-128, 127>, int8_t>);
    static_assert(std::is_same_v<select_by_range_t<-128, 100>, int8_t>);
    static_assert(std::is_same_v<select_by_range_t<-1, 127>, int8_t>);
    static_assert(std::is_same_v<select_by_range_t<-128, -1>, int8_t>);
    static_assert(std::is_same_v<select_by_range_t<-129, 127>, int16_t>);
    static_assert(std::is_same_v<select_by_range_t<-32768, 32767>, int16_t>);
    static_assert(std::is_same_v<select_by_range_t<-32769, 32767>, int32_t>);
    static_assert(std::is_same_v<select_by_range_t<-2147483648LL, 2147483647LL>, int32_t>);
    static_assert(std::is_same_v<select_by_range_t<-2147483649LL, 2147483647LL>, int64_t>);

    // select_by_value_t
    static_assert(std::is_same_v<select_by_value_t<0>, uint8_t>);
    static_assert(std::is_same_v<select_by_value_t<255>, uint8_t>);
    static_assert(std::is_same_v<select_by_value_t<256>, uint16_t>);
    static_assert(std::is_same_v<select_by_value_t<-1>, int8_t>);
    static_assert(std::is_same_v<select_by_value_t<-128>, int8_t>);
    static_assert(std::is_same_v<select_by_value_t<-129>, int16_t>);
}

// -----------------------------------------------------------------------------
// Tests for is_64bit concept
// -----------------------------------------------------------------------------
TEST_CASE("is_64bit") {
    static_assert(is_64bit<int64_t>);
    static_assert(is_64bit<uint64_t>);
    static_assert(!is_64bit<int32_t>);
    static_assert(!is_64bit<uint32_t>);
    static_assert(!is_64bit<int16_t>);
    static_assert(!is_64bit<uint16_t>);
    static_assert(!is_64bit<int8_t>);
    static_assert(!is_64bit<uint8_t>);
}

// -----------------------------------------------------------------------------
// Tests for checked_add
// -----------------------------------------------------------------------------
TEST_CASE("checked_add") {
    // unsigned
    CHECK(checked_add<uint8_t>(200, 55) == std::optional<uint8_t>(255));
    CHECK(checked_add<uint8_t>(200, 56) == std::nullopt);
    CHECK(checked_add<uint16_t>(65535, 0) == std::optional<uint16_t>(65535));
    CHECK(checked_add<uint16_t>(65535, 1) == std::nullopt);
    CHECK(checked_add<uint32_t>(4294967295U, 0) == std::optional<uint32_t>(4294967295U));
    CHECK(checked_add<uint32_t>(4294967295U, 1) == std::nullopt);
    CHECK(checked_add<uint64_t>(18446744073709551615ULL, 0) == std::optional<uint64_t>(18446744073709551615ULL));
    CHECK(checked_add<uint64_t>(18446744073709551615ULL, 1) == std::nullopt);

    // signed
    CHECK(checked_add<int8_t>(100, 27) == std::optional<int8_t>(127));
    CHECK(checked_add<int8_t>(100, 28) == std::nullopt);
    CHECK(checked_add<int8_t>(-128, -1) == std::nullopt);
    CHECK(checked_add<int8_t>(-128, 0) == std::optional<int8_t>(-128));
    CHECK(checked_add<int16_t>(32767, 0) == std::optional<int16_t>(32767));
    CHECK(checked_add<int16_t>(32767, 1) == std::nullopt);
    CHECK(checked_add<int16_t>(-32768, -1) == std::nullopt);
    CHECK(checked_add<int32_t>(2147483647, 0) == std::optional<int32_t>(2147483647));
    CHECK(checked_add<int32_t>(2147483647, 1) == std::nullopt);
    CHECK(checked_add<int64_t>(9223372036854775807LL, 0) == std::optional<int64_t>(9223372036854775807LL));
    CHECK(checked_add<int64_t>(9223372036854775807LL, 1) == std::nullopt);
    CHECK(checked_add<int64_t>(-9223372036854775808LL, -1) == std::nullopt);
    CHECK(checked_add<int64_t>(-9223372036854775808LL, 0) == std::optional<int64_t>(-9223372036854775808LL));

    // mixed signs
    CHECK(checked_add<int8_t>(-128, 1) == std::optional<int8_t>(-127));
    CHECK(checked_add<int8_t>(-1, -128) == std::nullopt); // -129 overflow
}

// -----------------------------------------------------------------------------
// Tests for checked_sub
// -----------------------------------------------------------------------------
TEST_CASE("checked_sub") {
    // unsigned
    CHECK(checked_sub<uint8_t>(100, 50) == std::optional<uint8_t>(50));
    CHECK(checked_sub<uint8_t>(100, 101) == std::nullopt);
    CHECK(checked_sub<uint8_t>(0, 1) == std::nullopt);
    CHECK(checked_sub<uint16_t>(0, 1) == std::nullopt);
    CHECK(checked_sub<uint32_t>(0, 1) == std::nullopt);
    CHECK(checked_sub<uint64_t>(0, 1) == std::nullopt);

    // signed
    CHECK(checked_sub<int8_t>(100, 50) == std::optional<int8_t>(50));
    CHECK(checked_sub<int8_t>(-128, 1) == std::nullopt); // -129 overflow
    CHECK(checked_sub<int8_t>(-128, -1) == std::optional<int8_t>(-127));
    CHECK(checked_sub<int8_t>(127, -1) == std::nullopt); // 128 overflow
    CHECK(checked_sub<int16_t>(-32768, 1) == std::nullopt);
    CHECK(checked_sub<int16_t>(32767, -1) == std::nullopt);
    CHECK(checked_sub<int32_t>(2147483647, -1) == std::nullopt);
    CHECK(checked_sub<int64_t>(9223372036854775807LL, -1) == std::nullopt);
    CHECK(checked_sub<int64_t>(-9223372036854775808LL, 1) == std::nullopt);
}

// -----------------------------------------------------------------------------
// Tests for checked_mul
// -----------------------------------------------------------------------------
TEST_CASE("checked_mul") {
    // unsigned
    CHECK(checked_mul<uint8_t>(50, 5) == std::optional<uint8_t>(250));
    CHECK(checked_mul<uint8_t>(50, 6) == std::nullopt); // 300 > 255
    CHECK(checked_mul<uint8_t>(0, 255) == std::optional<uint8_t>(0));
    CHECK(checked_mul<uint16_t>(1000, 65) == std::optional<uint16_t>(65000));
    CHECK(checked_mul<uint16_t>(1000, 66) == std::nullopt); // 66000 > 65535
    CHECK(checked_mul<uint32_t>(100000, 42949) == std::optional<uint32_t>(4294900000U));
    CHECK(checked_mul<uint32_t>(100000, 42950) == std::nullopt);
    CHECK(checked_mul<uint64_t>(18446744073709551615ULL, 0) == std::optional<uint64_t>(0));
    CHECK(checked_mul<uint64_t>(18446744073709551615ULL, 1) == std::optional<uint64_t>(18446744073709551615ULL));
    CHECK(checked_mul<uint64_t>(18446744073709551615ULL, 2) == std::nullopt);

    // signed
    CHECK(checked_mul<int8_t>(10, 12) == std::optional<int8_t>(120));
    CHECK(checked_mul<int8_t>(10, 13) == std::nullopt);
    CHECK(checked_mul<int8_t>(-10, 12) == std::optional<int8_t>(-120));
    CHECK(checked_mul<int8_t>(-10, 13) == std::nullopt);
    CHECK(checked_mul<int8_t>(-128, -1) == std::nullopt);
    CHECK(checked_mul<int8_t>(-128, 0) == std::optional<int8_t>(0));
    CHECK(checked_mul<int16_t>(3276, 10) == std::optional<int16_t>(32760));
    CHECK(checked_mul<int16_t>(3276, 11) == std::nullopt);
    CHECK(checked_mul<int32_t>(2147483, 1000) == std::optional<int32_t>(2147483000));
    CHECK(checked_mul<int32_t>(2147483, 1001) == std::nullopt);
    CHECK(checked_mul<int64_t>(9223372036854775807LL, 0) == std::optional<int64_t>(0));
    CHECK(checked_mul<int64_t>(9223372036854775807LL, 1) == std::optional<int64_t>(9223372036854775807LL));
    CHECK(checked_mul<int64_t>(9223372036854775807LL, 2) == std::nullopt);
    CHECK(checked_mul<int64_t>(-9223372036854775808LL, 1) == std::optional<int64_t>(-9223372036854775808LL));
    CHECK(checked_mul<int64_t>(-9223372036854775808LL, 2) == std::nullopt);
}

// -----------------------------------------------------------------------------
// Tests for saturate_cast
// -----------------------------------------------------------------------------
TEST_CASE("saturate_cast integral to integral") {
    CHECK(saturate_cast<int8_t>(100) == 100);
    CHECK(saturate_cast<int8_t>(200) == 127);
    CHECK(saturate_cast<int8_t>(-200) == -128);
    CHECK(saturate_cast<int16_t>(40000) == 32767);
    CHECK(saturate_cast<int16_t>(-40000) == -32768);
    CHECK(saturate_cast<uint8_t>(300) == 255);
    CHECK(saturate_cast<uint8_t>(-1) == 0);
    CHECK(saturate_cast<uint16_t>(-1) == 0);
    CHECK(saturate_cast<uint16_t>(70000) == 65535);
    CHECK(saturate_cast<int16_t>(static_cast<int32_t>(32767)) == 32767);
    CHECK(saturate_cast<int16_t>(static_cast<int32_t>(32768)) == 32767);
    CHECK(saturate_cast<int8_t>(static_cast<int16_t>(-129)) == -128);
}

TEST_CASE("saturate_cast floating to integral") {
    // float -> int
    CHECK(saturate_cast<int8_t>(123.4f) == 123);
    CHECK(saturate_cast<int8_t>(127.0f) == 127);
    CHECK(saturate_cast<int8_t>(127.1f) == 127); // saturate
    CHECK(saturate_cast<int8_t>(-128.0f) == -128);
    CHECK(saturate_cast<int8_t>(-128.1f) == -128);

    // Use infinity to test saturation to max/min
    CHECK(saturate_cast<int8_t>(std::numeric_limits<float>::infinity()) == 127);
    CHECK(saturate_cast<int8_t>(-std::numeric_limits<float>::infinity()) == -128);
    // NaN -> 0
    CHECK(saturate_cast<int8_t>(std::nanf("")) == 0);

    // double -> uint8
    CHECK(saturate_cast<uint8_t>(255.0) == 255);
    CHECK(saturate_cast<uint8_t>(255.1) == 255);
    CHECK(saturate_cast<uint8_t>(-1.0) == 0);
    CHECK(saturate_cast<uint8_t>(std::nan("")) == 0);

    // double -> int64: use a value definitely out of range, e.g. 1e30
    CHECK(saturate_cast<int64_t>(1e30) == std::numeric_limits<int64_t>::max());
    CHECK(saturate_cast<int64_t>(-1e30) == std::numeric_limits<int64_t>::min());
    // Also test with infinity
    CHECK(saturate_cast<int64_t>(std::numeric_limits<double>::infinity()) == std::numeric_limits<int64_t>::max());
    CHECK(saturate_cast<int64_t>(-std::numeric_limits<double>::infinity()) == std::numeric_limits<int64_t>::min());
}

TEST_CASE("saturate_cast integral to floating") {
    // int -> float
    CHECK(saturate_cast<float>(123) == 123.0f);
    CHECK(saturate_cast<float>(std::numeric_limits<int64_t>::max()) == static_cast<float>(std::numeric_limits<int64_t>::max()));
    CHECK(saturate_cast<double>(std::numeric_limits<int64_t>::min()) == static_cast<double>(std::numeric_limits<int64_t>::min()));
    // uint64_t -> float
    CHECK(saturate_cast<float>(std::numeric_limits<uint64_t>::max()) == static_cast<float>(std::numeric_limits<uint64_t>::max()));
}

TEST_CASE("saturate_cast floating to floating") {
    // double -> float: saturate when value exceeds float range
    // Use values definitely outside float's finite range
    constexpr double too_large = 1e39;   // > FLT_MAX (~3.4e38)
    constexpr double too_small = -1e39;  // < -FLT_MAX
    CHECK(saturate_cast<float>(too_large) == std::numeric_limits<float>::max());
    CHECK(saturate_cast<float>(too_small) == -std::numeric_limits<float>::max());

    // Infinity is saturated to max/min (matching the implementation)
    CHECK(saturate_cast<float>(std::numeric_limits<double>::infinity()) == std::numeric_limits<float>::max());
    CHECK(saturate_cast<float>(-std::numeric_limits<double>::infinity()) == -std::numeric_limits<float>::max());

    // NaN -> quiet NaN if available
    float res = saturate_cast<float>(std::numeric_limits<double>::quiet_NaN());
    CHECK(std::isnan(res));

    // float -> double: never overflows, but saturate infinity to max/min (matching implementation)
    CHECK(saturate_cast<double>(std::numeric_limits<float>::infinity()) == std::numeric_limits<double>::max());
    CHECK(saturate_cast<double>(-std::numeric_limits<float>::infinity()) == -std::numeric_limits<double>::max());
    // Normal values
    CHECK(saturate_cast<double>(1e38f) == static_cast<double>(1e38f));
    double res2 = saturate_cast<double>(std::numeric_limits<float>::quiet_NaN());
    CHECK(std::isnan(res2));
}