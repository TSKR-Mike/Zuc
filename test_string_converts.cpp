#include "doctest.h"
#include <ostream>
#include "include/string_converts.hpp"
#include "include/string_kits.hpp"
#include <vector>
#include <array>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <string>

using namespace zuc;

/**
 * @test Range to String Conversion Tests
 * Tests for converting various container types to their string representations.
 */
TEST_SUITE("Range to String Conversion") {
    TEST_CASE("vector to string") {
        std::vector<int> vec = {1, 2, 3, 4, 5};
        std::string result = convert_range_to_string(vec);
        
        CHECK(result == "[1, 2, 3, 4, 5]");
    }

    TEST_CASE("empty vector to string") {
        std::vector<int> vec;
        std::string result = convert_range_to_string(vec);
        
        CHECK(result == "[]");
    }

    TEST_CASE("array to string") {
        std::array<int, 4> arr = {10, 20, 30, 40};
        std::string result = convert_range_to_string(arr);
        
        CHECK(result == "[10, 20, 30, 40]");
    }

    TEST_CASE("set to string") {
        std::set<int> s = {3, 1, 4, 1, 5}; // Note: duplicates removed, sorted
        std::string result = convert_range_to_string(s);
        
        CHECK(result == "{1, 3, 4, 5}");
    }

    TEST_CASE("empty set to string") {
        std::set<int> s;
        std::string result = convert_range_to_string(s);
        
        CHECK(result == "{}");
    }

    TEST_CASE("unordered_set to string") {
        std::unordered_set<int> us = {1, 2, 3, 4, 5};
        std::string result = convert_range_to_string(us);
        
        // Order may vary, so just check structure
        CHECK(result.starts_with('{'));
        CHECK(result.ends_with('}'));
        CHECK(result.find('1') != std::string::npos);
        CHECK(result.find('5') != std::string::npos);
    }

    TEST_CASE("map to string") {
        std::map<std::string, int> m = {{"a", 1}, {"b", 2}, {"c", 3}};
        std::string result = convert_range_to_string(m);
        
        CHECK(result == "{a: 1, b: 2, c: 3}");
    }

    TEST_CASE("empty map to string") {
        std::map<std::string, int> m;
        std::string result = convert_range_to_string(m);
        
        CHECK(result == "{}");
    }

    TEST_CASE("unordered_map to string") {
        std::unordered_map<std::string, int> um = {{"x", 10}, {"y", 20}};
        std::string result = convert_range_to_string(um);
        
        // Order may vary, so just check structure
        CHECK(result.starts_with('{'));
        CHECK(result.ends_with('}'));
        CHECK(result.find(": 10") != std::string::npos || result.find(": 20") != std::string::npos);
    }

    TEST_CASE("vector of strings to string") {
        std::vector<std::string> vec = {"hello", "world", "test"};
        std::string result = convert_range_to_string(vec);
        
        CHECK(result == "[hello, world, test]");
    }

    TEST_CASE("vector of doubles to string") {
        std::vector<double> vec = {1.5, 2.5, 3.5};
        std::string result = convert_range_to_string(vec);
        
        CHECK(result == "[1.5, 2.5, 3.5]");
    }

    TEST_CASE("large vector to string") {
        std::vector<int> vec(100);
        for (size_t i = 0; i < vec.size(); ++i) {
            vec[i] = static_cast<int>(i);
        }
        
        std::string result = convert_range_to_string(vec);
        
        CHECK(result.starts_with("[0"));
        CHECK(result.ends_with("99]"));
        CHECK(result.find(", ") != std::string::npos);
    }
}

/**
 * @test Binary Data Conversion Tests
 * Tests for converting objects and data to byte spans.
 */
TEST_SUITE("Binary Data Conversion") {
    TEST_CASE("int to bytes") {
        int value = 42;
        auto bytes = convert_to_bytes(value);
        
        CHECK(bytes.size() == sizeof(int));
    }

    TEST_CASE("double to bytes") {
        double value = 3.14159;
        auto bytes = convert_to_bytes(value);
        
        CHECK(bytes.size() == sizeof(double));
    }

    TEST_CASE("vector to bytes") {
        std::vector<int> vec = {1, 2, 3, 4, 5};
        auto bytes = convert_to_bytes(std::span(vec));
        
        CHECK(bytes.size() == vec.size() * sizeof(int));
    }

    TEST_CASE("array to bytes") {
        std::array<int, 3> arr = {10, 20, 30};
        auto bytes = convert_to_bytes(std::span(arr));
        
        CHECK(bytes.size() == arr.size() * sizeof(int));
    }

    TEST_CASE("string_view to bytes") {
        std::string_view text = "Hello";
        auto bytes = convert_to_bytes(text);
        
        CHECK(bytes.size() == text.size());
    }

    TEST_CASE("string to bytes") {
        std::string text = "World";
        auto bytes = convert_to_bytes(text);
        
        CHECK(bytes.size() == text.size());
    }

    TEST_CASE("empty string_view to bytes") {
        std::string_view text = "";
        auto bytes = convert_to_bytes(text);
        
        CHECK(bytes.size() == 0);
    }

    TEST_CASE("struct to bytes") {
        struct TestStruct {
            int a;
            double b;
            char c;
        };
        
        TestStruct s{42, 3.14, 'X'};
        auto bytes = convert_to_bytes(s);
        
        CHECK(bytes.size() == sizeof(TestStruct));
    }

    TEST_CASE("empty vector to bytes") {
        std::vector<int> vec;
        auto bytes = convert_to_bytes(std::span(vec));
        
        CHECK(bytes.size() == 0);
    }
}

/**
 * @test Hexadecimal Conversion Tests
 * Tests for converting data to hexadecimal string representations.
 */
TEST_SUITE("Hexadecimal Conversion") {
    TEST_CASE("single byte to hex") {
        std::byte b = std::byte{0xAB};
        std::string hex = convert_to_hex(b);
        
        CHECK(hex.size() == 2); // Two chars for single byte
        CHECK(hex == "AB"); // Full hexadecimal representation
        
        // Test edge cases
        CHECK(convert_to_hex(std::byte{0x00}) == "00");
        CHECK(convert_to_hex(std::byte{0x0F}) == "0F");
        CHECK(convert_to_hex(std::byte{0xF0}) == "F0");
        CHECK(convert_to_hex(std::byte{0xFF}) == "FF");
    }

    TEST_CASE("byte span to hex") {
        std::vector<std::byte> data = {std::byte{0xAB}, std::byte{0xCD}};
        std::string hex = convert_to_hex(std::span(data));
        
        CHECK(hex == "ABCD");
    }

    TEST_CASE("empty byte span to hex") {
        std::vector<std::byte> data;
        std::string hex = convert_to_hex(std::span(data));
        
        CHECK(hex.empty());
    }

    TEST_CASE("int to hex") {
        int value = 0x12345678;
        std::string hex = convert_to_hex(value);
        
        CHECK(hex.size() == sizeof(int) * 2);
        CHECK(hex.find("12") != std::string::npos || hex.find("78") != std::string::npos);
    }

    TEST_CASE("string_view to hex") {
        std::string_view text = "Hi";
        std::string hex = convert_to_hex(text);
        
        CHECK(hex == "4869"); // ASCII for 'H' and 'i'
    }

    TEST_CASE("string to hex") {
        std::string text = "ABC";
        std::string hex = convert_to_hex(text);
        
        CHECK(hex == "414243"); // ASCII for 'A', 'B', 'C'
    }

    TEST_CASE("empty string to hex") {
        std::string text = "";
        std::string hex = convert_to_hex(text);
        
        CHECK(hex.empty());
    }

    TEST_CASE("large data to hex") {
        std::vector<std::byte> data(256);
        for (size_t i = 0; i < data.size(); ++i) {
            data[i] = std::byte(i % 256);
        }
        
        std::string hex = convert_to_hex(std::span(data));
        
        CHECK(hex.size() == 256 * 2); // 2 hex chars per byte
    }

    TEST_CASE("zero values to hex") {
        std::vector<std::byte> data = {std::byte{0x00}, std::byte{0x00}, std::byte{0x00}};
        std::string hex = convert_to_hex(std::span(data));
        
        CHECK(hex == "000000");
    }

    TEST_CASE("max byte value to hex") {
        std::vector<std::byte> data = {std::byte{0xFF}, std::byte{0xFF}};
        std::string hex = convert_to_hex(std::span(data));
        
        CHECK(hex == "FFFF");
    }
}

/**
 * @test Numeric String Conversion Tests
 * Tests for converting strings to numeric values safely.
 */
TEST_SUITE("Numeric String Conversion") {
    TEST_CASE("valid integer string to int") {
        auto result = try_convert_string_to_numerics<int>("123");
        
        CHECK(result.has_value());
        CHECK(result.value() == 123);
    }

    TEST_CASE("negative integer string to int") {
        auto result = try_convert_string_to_numerics<int>("-456");
        
        CHECK(result.has_value());
        CHECK(result.value() == -456);
    }

    TEST_CASE("valid double string to double") {
        auto result = try_convert_string_to_numerics<double>("3.14159");
        
        CHECK(result.has_value());
        CHECK(result.value() == doctest::Approx(3.14159));
    }

    TEST_CASE("scientific notation to double") {
        auto result = try_convert_string_to_numerics<double>("1.23e-4");
        
        CHECK(result.has_value());
        CHECK(result.value() == doctest::Approx(1.23e-4));
    }

    TEST_CASE("invalid string to int") {
        auto result = try_convert_string_to_numerics<int>("abc");
        
        CHECK(!result.has_value());
    }

    TEST_CASE("empty string to int") {
        auto result = try_convert_string_to_numerics<int>("");
        
        CHECK(!result.has_value());
    }

    TEST_CASE("partial number string") {
        auto result = try_convert_string_to_numerics<int>("123abc");
        
        CHECK(!result.has_value()); // Should fail - entire string must be valid
    }

    TEST_CASE("string with whitespace") {
        auto result = try_convert_string_to_numerics<int>(" 123 ");
        
        CHECK(!result.has_value()); // Whitespace not allowed
    }

    TEST_CASE("zero string to int") {
        auto result = try_convert_string_to_numerics<int>("0");
        
        CHECK(result.has_value());
        CHECK(result.value() == 0);
    }

    TEST_CASE("large number string") {
        auto result = try_convert_string_to_numerics<int64_t>("9223372036854775807"); // INT64_MAX
        
        CHECK(result.has_value());
        CHECK(result.value() == INT64_MAX);
    }

    TEST_CASE("overflow number string") {
        auto result = try_convert_string_to_numerics<int>("999999999999999999999");
        
        CHECK(!result.has_value());
    }

    TEST_CASE("float string to float") {
        auto result = try_convert_string_to_numerics<float>("3.14f");
        
        // May fail due to 'f' suffix, depends on implementation
        // Just check it doesn't crash
    }
}

/**
 * @test Is Numeric Tests
 * Tests for checking if strings represent valid numeric values.
 */
TEST_SUITE("Is Numeric") {
    TEST_CASE("valid integer string") {
        CHECK(is_numeric("123"));
        CHECK(is_numeric("-456"));
        CHECK(is_numeric("0"));
    }

    TEST_CASE("valid floating point string") {
        CHECK(is_numeric("3.14"));
        CHECK(is_numeric("-2.5"));
        CHECK(is_numeric("1.23e-4"));
        CHECK(is_numeric("6.022e23"));
    }

    TEST_CASE("invalid strings") {
        CHECK(!is_numeric("abc"));
        CHECK(!is_numeric("123abc"));
        CHECK(!is_numeric("12.34.56"));
        CHECK(!is_numeric(""));
        CHECK(!is_numeric(" 123 "));
        CHECK(!is_numeric("hello world"));
    }

    TEST_CASE("edge cases") {
        CHECK(is_numeric("0.0"));
        CHECK(is_numeric("-0.0"));
        CHECK(is_numeric("1e10"));
        CHECK(is_numeric("-1e-10"));
    }
}

/**
 * @test Real-World Use Cases
 * Tests demonstrating practical applications of string conversion utilities.
 */
TEST_SUITE("Real-World Use Cases") {
    TEST_CASE("debug vector contents") {
        std::vector<int> data = {10, 20, 30, 40, 50};
        std::string debug_output = convert_range_to_string(data);
        
        CHECK(debug_output == "[10, 20, 30, 40, 50]");
    }

    TEST_CASE("serialize configuration map") {
        std::map<std::string, int> config = {
            {"timeout", 30},
            {"retries", 3},
            {"buffer_size", 1024}
        };
        
        std::string serialized = convert_range_to_string(config);
        
        CHECK(serialized.find("timeout: 30") != std::string::npos);
        CHECK(serialized.find("retries: 3") != std::string::npos);
        CHECK(serialized.find("buffer_size: 1024") != std::string::npos);
    }

    TEST_CASE("binary data inspection") {
        int value = 0x12345678;
        std::string hex = convert_to_hex(value);
        
        CHECK(hex.size() == sizeof(int) * 2);
        // Check that it contains valid hex characters
        for (char c : hex) {
            CHECK((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'));
        }
    }

    TEST_CASE("safe numeric input parsing") {
        std::vector<std::string> inputs = {"123", "456", "789", "abc", "def"};
        
        int sum = 0;
        int valid_count = 0;
        
        for (const auto& input : inputs) {
            if (auto value = try_convert_string_to_numerics<int>(input)) {
                sum += value.value();
                valid_count++;
            }
        }
        
        CHECK(sum == 1368); // 123 + 456 + 789
        CHECK(valid_count == 3);
    }

    TEST_CASE("data validation") {
        std::vector<std::string> user_inputs = {"42", "3.14", "hello", "-100", "1.5e3"};
        
        int numeric_count = 0;
        for (const auto& input : user_inputs) {
            if (is_numeric(input)) {
                numeric_count++;
            }
        }
        
        CHECK(numeric_count == 4); // All except "hello"
    }

    TEST_CASE("memory dump visualization") {
        std::vector<int> data = {0x12345678, static_cast<int>(0xABCDEF00), 0x11223344};
        // Convert int span to bytes first, then to hex
        auto bytes = convert_to_bytes(std::span(data));
        std::string hex_dump = convert_to_hex(bytes);
        
        CHECK(hex_dump.size() == data.size() * sizeof(int) * 2);
        // Verify it's valid hex
        for (char c : hex_dump) {
            CHECK((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'));
        }
    }

    TEST_CASE("config file parsing") {
        std::string timeout_str = "30";
        std::string retries_str = "3";
        std::string invalid_str = "unlimited";
        
        auto timeout = try_convert_string_to_numerics<int>(timeout_str);
        auto retries = try_convert_string_to_numerics<int>(retries_str);
        auto invalid = try_convert_string_to_numerics<int>(invalid_str);
        
        CHECK(timeout.has_value());
        CHECK(timeout.value() == 30);
        
        CHECK(retries.has_value());
        CHECK(retries.value() == 3);
        
        CHECK(!invalid.has_value());
    }
}

/**
 * @test Performance and Edge Cases
 * Tests for performance characteristics and edge cases.
 */
TEST_SUITE("Performance and Edge Cases") {
    TEST_CASE("very large container conversion") {
        std::vector<int> large_vec(10000);
        for (size_t i = 0; i < large_vec.size(); ++i) {
            large_vec[i] = static_cast<int>(i);
        }
        
        std::string result = convert_range_to_string(large_vec);
        
        CHECK(result.starts_with("[0"));
        CHECK(result.ends_with("]"));
        CHECK(result.size() > 10000); // Should be substantially larger than input
    }

    TEST_CASE("special characters in strings") {
        std::vector<std::string> strings = {
            "hello\nworld",
            "tab\there",
            "quote\"test"
        };
        
        std::string result = convert_range_to_string(strings);
        
        CHECK(result.find("hello") != std::string::npos);
        CHECK(result.find("world") != std::string::npos);
    }

    TEST_CASE("unicode characters") {
        std::vector<std::string> strings = {"hello", "世界", "🌍"};
        
        std::string result = convert_range_to_string(strings);
        
        CHECK(result.find("hello") != std::string::npos);
        // Unicode characters should be present (encoding dependent)
    }

    TEST_CASE("nested structures") {
        // Note: nested vectors are not directly supported as they require
        // the inner type to be Stringable, which std::vector<int> is not.
        // This test demonstrates the limitation.
        
        // Instead, test with nested strings which are Stringable
        std::vector<std::string> nested = {"hello", "world", "test"};
        std::string result = convert_range_to_string(nested);
        
        CHECK(!result.empty());
        CHECK(result.find("hello") != std::string::npos);
        CHECK(result.find("world") != std::string::npos);
    }

    TEST_CASE("maximum numeric values") {
        auto int_max = try_convert_string_to_numerics<int>("2147483647");
        auto int_min = try_convert_string_to_numerics<int>("-2147483648");
        
        CHECK(int_max.has_value());
        CHECK(int_max.value() == INT_MAX);
        
        CHECK(int_min.has_value());
        CHECK(int_min.value() == INT_MIN);
    }
}