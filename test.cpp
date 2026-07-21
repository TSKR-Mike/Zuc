/**
 * @file test.cpp
 * @brief Comprehensive test suite for zuc library
 * 
 * This file contains unit tests for all major components of the zuc library:
 * - String operations (slicing, trimming, splitting, joining, etc.)
 * - File I/O operations (FileMgr class, console I/O)
 * - Time utilities (Timer, Stopwatch, DateTime, benchmarking)
 * - Span operations (safe slicing, conversion, searching, concatenation)
 * - Random number generation (various types and distributions)
 * - Common concepts (type constraints and predicates)
 * 
 * Uses doctest framework for test execution and assertions.
 * 
 * Run tests with: ./zuc_test
 * Or with CMake: cmake --build . && ctest
 */

#include <ios>
#include <span>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <array>
#include <filesystem>
#include <format>
#include <fstream>
#include <string>
#include <vector>

#include "doctest.h"
#include "include/io_kits.hpp"
#include "include/string_kits.hpp"
#include "include/span_kits.hpp"
#include "include/time_kits.hpp"
#include "include/random_kits.hpp"
using namespace zuc;

// ============================================================================
// SPAN OPERATIONS TESTS
// ============================================================================

/**
 * @test Span Operations
 * Tests for span utilities including safe subspan operations,
 * span conversion, content checking, and span concatenation.
 */
TEST_SUITE("Span Operations") {
    std::vector<int> data = {1, 2, 3, 4, 5};
    TEST_CASE("sub_span_safe valid range") {
        auto result = sub_span_safe(std::span(data), 1, 3);
        
        CHECK(result.has_value());
        CHECK(result.value().size() == 3);
        CHECK(result.value()[0] == 2);
        CHECK(result.value()[1] == 3);
        CHECK(result.value()[2] == 4);
    }

    TEST_CASE("sub_span_safe offset out of bounds") {
        std::vector<int> data = {1, 2, 3, 4, 5};
        auto result = sub_span_safe(std::span(data), 10, 2);
        
        CHECK(!result.has_value());
    }

    TEST_CASE("sub_span_safe count exceeds bounds") {
        std::vector<int> data = {1, 2, 3, 4, 5};
        auto result = sub_span_safe(std::span(data), 3, 5);
        
        CHECK(!result.has_value());
    }

    TEST_CASE("sub_span_safe with dynamic_extent") {
        std::vector<int> data = {1, 2, 3, 4, 5};
        auto result = sub_span_safe(std::span(data), 2);
        
        CHECK(result.has_value());
        CHECK(result.value().size() == 3);
        CHECK(result.value()[0] == 3);
        CHECK(result.value()[1] == 4);
        CHECK(result.value()[2] == 5);
    }

    TEST_CASE("sub_span_safe empty span") {
        std::vector<int> data = {};
        auto result = sub_span_safe(std::span(data), 0, 0);
        
        CHECK(result.has_value());
        CHECK(result.value().empty());
    }

    TEST_CASE("convert_span_to_vector basic") {
        std::array<int, 4> arr = {10, 20, 30, 40};
        auto vec = convert_span_to_vector(arr);
        
        CHECK(vec.size() == 4);
        CHECK(vec[0] == 10);
        CHECK(vec[1] == 20);
        CHECK(vec[2] == 30);
        CHECK(vec[3] == 40);
    }

    TEST_CASE("convert_span_to_vector empty") {
        std::vector<int> data = {};
        auto vec = convert_span_to_vector(data);
        
        CHECK(vec.empty());
    }

    TEST_CASE("contains element found") {

        std::span<const int> numbers = data;
        CHECK(contains(numbers, 3) == true);
    }

    TEST_CASE("contains element not found") {
        std::span<const int> numbers = data;
        CHECK(contains(numbers, 10) == false);
    }

    TEST_CASE("contains empty span") {
        std::span<const int> empty = {};
        CHECK(contains(empty, 1) == false);
    }

    TEST_CASE("contains_any element found") {
        std::span<int> numbers = data;
        std::vector<int> targets = {7, 8, 3};   
        CHECK(contains_any(numbers, targets) == true);
    }

    TEST_CASE("contains_any no element found") {
        std::span<int> numbers = data;
        std::vector<int> targets = {6, 7, 8};   
        CHECK(contains_any(numbers, std::span(targets)) == false);
    }

    TEST_CASE("contains_any empty search span") {
        std::span<int> numbers = data;
        CHECK(contains_any(numbers, std::span<int>{}) == false);
    }

    TEST_CASE("contains_any empty target span") {
        std::span<int> empty = {};
        CHECK(contains_any(empty, std::span<int>{}) == false);
    }

    TEST_CASE("find_subspan found") {
        std::vector<int> d{3, 4, 5, 6};
        std::span<int> main_span = d;
        std::vector<int> targets = {4, 5, 6};   
        auto found = find_subspan(main_span, std::span(targets));
        
        CHECK(found.size() == 3);
        CHECK(found[0] == 4);
        CHECK(found[1] == 5);
        CHECK(found[2] == 6);
    }

    TEST_CASE("find_subspan not found") {
        std::span<int> main_span = data;
        std::vector<int> targets = {6, 7, 8};   
        auto found = find_subspan(main_span, std::span(targets));
        
        CHECK(found.empty());
    }

    TEST_CASE("find_subspan empty target") {
        std::span<int> main_span = data;
        auto found = find_subspan(main_span, std::span<int>{});
        
        CHECK(found.size() == 0);
    }

    TEST_CASE("find_subspan target larger than main") {
        std::span<int> main_span = data;
        std::vector<int> targets = {1, 2, 3, 4, 5, 6};   // Larger than main_span which has 5 elements
        auto found = find_subspan(main_span, std::span(targets));
        
        CHECK(found.empty());
    }

    TEST_CASE("ConcatSpan basic iteration") {
        std::array<int, 3> part1 = {1, 2, 3};
        std::vector<int> part2 = {4, 5, 6};
        std::array<int, 2> part3 = {7, 8};
        
        ConcatSpan<int, 3> concatenated({
            std::span(part1),
            std::span(part2), 
            std::span(part3)
        });
        
        CHECK(concatenated.size() == 8);
        
        std::vector<int> result;
        for (const auto& val : concatenated) {
            result.push_back(val);
        }
        
        CHECK(result.size() == 8);
        CHECK(result == std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8});
    }

    TEST_CASE("ConcatSpan random access") {
        std::array<int, 3> part1 = {1, 2, 3};
        std::vector<int> part2 = {4, 5, 6};
        std::array<int, 2> part3 = {7, 8};
        
        ConcatSpan<int, 3> concatenated({
            std::span(part1),
            std::span(part2), 
            std::span(part3)
        });
        
        CHECK(concatenated[0] == 1);
        CHECK(concatenated[2] == 3);
        CHECK(concatenated[3] == 4);
        CHECK(concatenated[5] == 6);
        CHECK(concatenated[7] == 8);
    }

    TEST_CASE("ConcatSpan at method") {
        std::array<int, 3> part1 = {1, 2, 3};
        std::vector<int> part2 = {4, 5, 6};
        
        ConcatSpan<int, 2> concatenated({
            std::span(part1),
            std::span(part2)
        });
        
        CHECK(concatenated.at(0) == 1);
        CHECK(concatenated.at(5) == 6);
    }

    TEST_CASE("ConcatSpan get_data") {
        std::array<int, 2> part1 = {1, 2};
        std::vector<int> part2 = {3, 4};
        
        ConcatSpan<int, 2> concatenated({
            std::span(part1),
            std::span(part2)
        });
        
        auto data = concatenated.get_data();
        CHECK(data.size() == 2);
        CHECK(data[0].size() == 2);
        CHECK(data[1].size() == 2);
    }

    TEST_CASE("ConcatSpan empty spans") {
        std::vector<int> empty1, empty2;
        std::array<int, 3> part3 = {1, 2, 3};
        
        ConcatSpan<int, 3> concatenated({
            std::span(empty1),
            std::span(empty2),
            std::span(part3)
        });
        
        CHECK(concatenated.size() == 3);
        CHECK(concatenated[0] == 1);
        CHECK(concatenated[2] == 3);
    }

    TEST_CASE("ConcatSpan iterator operations") {
        std::array<int, 2> part1 = {1, 2};
        std::array<int, 2> part2 = {3, 4};
        
        ConcatSpan<int, 2> concatenated({
            std::span(part1),
            std::span(part2)
        });
        
        // Test iterator increment
        auto it = concatenated.begin();
        CHECK(*it == 1);
        ++it;
        CHECK(*it == 2);
        
        // Test iterator decrement
        --it;
        CHECK(*it == 1);
        
        // Test iterator arithmetic
        auto it2 = it + 2;
        CHECK(*it2 == 3);
        
        // Test iterator comparison
        auto it3 = concatenated.begin();
        CHECK(it == it3);
        CHECK(it2 != it3);
    }

    TEST_CASE("sub_span_safe edge cases") {
        std::vector<int> data = {1, 2, 3, 4, 5};
        
        // Test offset at boundary
        auto result = sub_span_safe(std::span(data), 5, 0);
        CHECK(result.has_value());
        CHECK(result.value().empty());
        
        // Test count exactly matching remaining
        auto result2 = sub_span_safe(std::span(data), 2, 3);
        CHECK(result2.has_value());
        CHECK(result2.value().size() == 3);
        CHECK(result2.value()[0] == 3);
        CHECK(result2.value()[2] == 5);
        
        // Test dynamic_extent
        auto result3 = sub_span_safe(std::span(data), 3);
        CHECK(result3.has_value());
        CHECK(result3.value().size() == 2);
        CHECK(result3.value()[0] == 4);
        CHECK(result3.value()[1] == 5);
    }
}

// ============================================================================
// STRING OPERATIONS TESTS
// ============================================================================

/**
 * @test String View Operations
 * Tests for string slicing, prefix/suffix operations, and string manipulation
 * using zero-allocation string views.
 */
TEST_SUITE("String View Operations") {
    TEST_CASE("slice basic functionality") {
        std::string test_str = "Hello World";
        std::string_view sv = test_str;

        auto sliced = slice(sv, 6, 5);
        CHECK(std::string(sliced) == "World");
    }

    TEST_CASE("slice with empty string") {
        std::string empty_str = "";
        std::string_view sv = empty_str;
        auto sliced = slice(sv, 0, 5);
        CHECK(sliced.empty());
    }

    TEST_CASE("prefix basic functionality") {
        std::string test_str = "Hello World";
        std::string_view sv = test_str;

        auto pref = prefix(sv, 5);
        CHECK(std::string(pref) == "Hello");
    }

    TEST_CASE("suffix basic functionality") {
        std::string test_str = "Hello World";
        std::string_view sv = test_str;

        auto suff = suffix(sv, 5);
        CHECK(std::string(suff) == "World");
    }

    TEST_CASE("remove_prefix basic functionality") {
        std::string test_str = "Hello World";
        std::string_view sv = test_str;

        auto no_pref = remove_prefix(sv, 6);
        CHECK(std::string(no_pref) == "World");
    }

    TEST_CASE("remove_suffix basic functionality") {
        std::string test_str = "Hello World";
        std::string_view sv = test_str;

        auto no_suff = remove_suffix(sv, 6);
        CHECK(std::string(no_suff) == "Hello");
    }
}

/**
 * @test Trim Operations
 * Tests for string trimming functionality including in-place and copy-based
 * trimming operations with various edge cases.
 */
TEST_SUITE("Trim Operations") {
    TEST_CASE("get_trimmed_left removes leading spaces") {
        std::string test_str = "  Hello World  ";
        auto trimmed_left = get_trimmed_left(test_str);
        CHECK(trimmed_left == "Hello World  ");
    }

    TEST_CASE("get_trimmed_right removes trailing spaces") {
        std::string test_str = "  Hello World  ";
        auto trimmed_right = get_trimmed_right(test_str);
        CHECK(trimmed_right == "  Hello World");
    }

    TEST_CASE("get_trimmed removes both leading and trailing spaces") {
        std::string test_str = "  Hello World  ";
        auto trimmed = get_trimmed(test_str);
        CHECK(trimmed == "Hello World");
    }

    TEST_CASE("trim in-place modifies string") {
        std::string mutable_str = "  Test String  ";
        trim(mutable_str);
        CHECK(mutable_str == "Test String");
    }

    TEST_CASE("trim_left in-place modifies string") {
        std::string mutable_str = "  Test String  ";
        trim_left(mutable_str);
        CHECK(mutable_str == "Test String  ");
    }

    TEST_CASE("trim_right in-place modifies string") {
        std::string mutable_str = "  Test String  ";
        trim_right(mutable_str);
        CHECK(mutable_str == "  Test String");
    }

    TEST_CASE("trim_view returns trimmed view") {
        std::string_view sv = "  View Test  ";
        auto trimmed_view = trim_view(sv);
        CHECK(std::string(trimmed_view) == "View Test");
    }

    TEST_CASE("trim empty string") {
        std::string empty_str = "";
        auto trimmed = get_trimmed(empty_str);
        CHECK(trimmed.empty());
    }

    TEST_CASE("trim all spaces") {
        std::string spaces_str = "     ";
        auto trimmed = get_trimmed(spaces_str);
        CHECK(trimmed.empty());
    }
}

/**
 * @test Split Operations
 * Tests for string splitting by single characters, multiple characters,
 * and string delimiters with both view-based and string-based results.
 */
TEST_SUITE("Split Operations") {
    TEST_CASE("split by single character") {
        std::string test_str = "apple,banana,cherry,date";
        auto parts = split(test_str, ',');

        CHECK(parts.size() == 4);
        CHECK(std::string(parts[0]) == "apple");
        CHECK(std::string(parts[1]) == "banana");
        CHECK(std::string(parts[2]) == "cherry");
        CHECK(std::string(parts[3]) == "date");
    }

    TEST_CASE("split_to_string by single character") {
        std::string test_str = "apple,banana,cherry,date";
        auto string_parts = split_to_string(test_str, ',');

        CHECK(string_parts.size() == 4);
        CHECK(string_parts[0] == "apple");
        CHECK(string_parts[1] == "banana");
        CHECK(string_parts[2] == "cherry");
        CHECK(string_parts[3] == "date");
    }

    TEST_CASE("split by string delimiter") {
        std::string test_str = "apple::banana::cherry";
        auto parts = split(test_str, "::");

        CHECK(parts.size() == 3);
        CHECK(std::string(parts[0]) == "apple");
        CHECK(std::string(parts[1]) == "banana");
        CHECK(std::string(parts[2]) == "cherry");
    }

    TEST_CASE("split_by_any with multiple delimiters") {
        std::string test_str = "apple,banana;cherry:date";
        auto parts = split_by_any(test_str, ",;:");

        CHECK(parts.size() == 4);
        CHECK(parts[0] == "apple");
        CHECK(parts[1] == "banana");
        CHECK(parts[2] == "cherry");
        CHECK(parts[3] == "date");
    }

    TEST_CASE("split empty string") {
        auto parts = split("", ',');
        CHECK(parts.empty());
    }

    TEST_CASE("split single element") {
        auto parts = split("hello", ',');
        CHECK(parts.size() == 1);
        CHECK(std::string(parts[0]) == "hello");
    }
}

/**
 * @test Join Operations
 * Tests for joining multiple strings with optional delimiters,
 * including memory efficiency and edge cases.
 */
TEST_SUITE("Join Operations") {
    TEST_CASE("join without delimiter") {
        std::vector<std::string_view> strings = {"Hello", "World", "Test"};
        auto joined = join(strings);
        CHECK(joined == "HelloWorldTest");
    }

    TEST_CASE("join with array") {
        std::array<std::string_view, 3> arr = {"One", "Two", "Three"};
        auto joined = join(arr);
        CHECK(joined == "OneTwoThree");
    }

    TEST_CASE("join with delimiter") {
        std::vector<std::string_view> strings = {"Hello", "World", "Test"};
        auto joined = join(strings, ", ");
        CHECK(joined == "Hello, World, Test");
    }

    TEST_CASE("join empty vector") {
        auto joined = join(std::vector<std::string_view>());
        CHECK(joined.empty());
    }
}
/**
 * @test Match Operations
 * Tests for string matching against multiple target strings.
 */
TEST_SUITE("Match Operations") {
    TEST_CASE("Match exact string") {
        CHECK(match_any("hello", {"hello", "a"}) == true);
        CHECK(match_any("hello", {"world", "test"}) == false);
        CHECK(match_any("123", {"123", "456"}) == true);
    }


    TEST_CASE("Match empty targets") {
        CHECK(match_any("hello", {}) == false);
    }

    TEST_CASE("Match with initializer_list") {
        CHECK(match_any("target", {"target1", "target", "target2"}) == true);
        CHECK(match_any("missing", {"a", "b", "c"}) == false);
    }
}
/**
 * @test Contains Operations
 * Tests for substring checking including single substring, any of multiple
 * substrings, and all of multiple substrings.
 */
TEST_SUITE("Contains Operations") {
    TEST_CASE("contains returns true for existing substring") {
        std::string test_str = "Hello World Test";
        CHECK(contains(test_str, "World") == true);
    }

    TEST_CASE("contains returns false for non-existing substring") {
        std::string test_str = "Hello World Test";
        CHECK(contains(test_str, "XYZ") == false);
    }

    TEST_CASE("contains_any returns true if any substring exists") {
        std::string test_str = "Hello World Test";
        std::array<std::string_view, 3> search_strings = {"Hello", "World",
                                                          "Test"};
        CHECK(contains_any(test_str, search_strings) == true);
    }

    TEST_CASE("contains_all returns true if all substrings exist") {
        std::string test_str = "Hello World Test";
        std::array<std::string_view, 2> search_strings = {"Hello", "World"};
        CHECK(contains_all(test_str, search_strings) == true);
    }

    TEST_CASE("contains_all returns false if any substring missing") {
        std::string test_str = "Hello World Test";
        std::array<std::string_view, 2> search_strings = {"Hello", "XYZ"};
        CHECK(contains_all(test_str, search_strings) == false);
    }
}

/**
 * @test Replace Operations
 * Tests for string replacement including in-place and copy-based operations
 * with various patterns and edge cases.
 */
TEST_SUITE("Replace Operations") {
    TEST_CASE("replace_all in-place") {
        std::string test_str = "hello world hello";
        replace_all(test_str, "hello", "hi");
        CHECK(test_str == "hi world hi");
    }

    TEST_CASE("get_all_replaced returns copy") {
        std::string test_str = "test123test456test";
        auto replaced = get_all_replaced(test_str, "test", "demo");
        CHECK(replaced == "demo123demo456demo");
        CHECK(test_str == "test123test456test");  // original unchanged
    }

    TEST_CASE("replace_all repeated pattern") {
        std::string test_str = "aaa";
        replace_all(test_str, "a", "b");
        CHECK(test_str == "bbb");
    }
}

/**
 * @test Remove Operations
 * Tests for string removal operations including single and multiple patterns.
 */
TEST_SUITE("Remove Operations") {
    TEST_CASE("remove_all in-place") {
        std::string test_str = "hello world hello";
        remove_all(test_str, "hello");
        CHECK(test_str == " world ");
    }

    TEST_CASE("get_all_removed returns copy") {
        std::string test_str = "test123test456test";
        auto removed = get_all_removed(test_str, "test");
        CHECK(removed == "123456");
        CHECK(test_str == "test123test456test");  // original unchanged
    }

    TEST_CASE("get_all_removed multiple patterns") {
        std::string test_str = "abc123def456ghi";
        std::array<std::string_view, 2> to_remove = {"123", "456"};
        auto removed = get_all_removed(test_str, to_remove);
        CHECK(removed == "abcdefghi");
    }

    TEST_CASE("remove_all overlapping patterns") {
        std::string test_str = "aaaa";
        remove_all(test_str, "aa");
        CHECK(test_str.empty());
    }

    TEST_CASE("replace_all with empty new_str") {
        std::string test_str = "hello world hello";
        replace_all(test_str, "hello", "");
        CHECK(test_str == " world ");
    }

    TEST_CASE("contains edge cases") {
        CHECK(contains("", "") == false);
        CHECK(contains("test", "") == false);
        CHECK(contains("", "test") == false);
        CHECK(contains("hello", "hello world") == false);
    }

    TEST_CASE("split edge cases") {
        auto parts = split("a,b,c", ',');
        CHECK(parts.size() == 3);
        CHECK(std::string(parts[0]) == "a");
        CHECK(std::string(parts[2]) == "c");
        
        auto parts2 = split(",a,b,", ',');
        CHECK(parts2.size() == 4);
        CHECK(parts2[0].empty());
        CHECK(parts2[3].empty());
    }

    TEST_CASE("join with empty delimiter") {
        std::vector<std::string_view> strings = {"a", "b", "c"};
        auto joined = join(strings, "");
        CHECK(joined == "abc");
    }

    TEST_CASE("trim_view with only whitespace") {
        std::string_view sv = "   ";
        auto trimmed = trim_view(sv);
        CHECK(trimmed.empty());
    }

    TEST_CASE("slice edge cases") {
        std::string_view sv = "hello";
        
        auto s1 = slice(sv, 0, 0);
        CHECK(s1.empty());
        
        auto s2 = slice(sv, 0, 5);
        CHECK(std::string(s2) == "hello");
        
        auto s3 = slice(sv, 5, 0);
        CHECK(s3.empty());
    }
}

/**
 * @test Repeat Operations
 * Tests for string repetition functionality with various edge cases.
 */
TEST_SUITE("Repeat Operations") {
    TEST_CASE("repeat basic functionality") {
        std::string test_str = "abc";
        auto repeated = repeat(test_str, 3);
        CHECK(repeated == "abcabcabc");
    }

    TEST_CASE("repeat with empty string") {
        std::string empty_str = "";
        auto repeated = repeat(empty_str, 5);
        CHECK(repeated.empty());
    }

    TEST_CASE("repeat with zero times") {
        std::string test_str = "abc";
        auto repeated = repeat(test_str, 0);
        CHECK(repeated.empty());
    }
}

// ============================================================================
// FILE I/O TESTS
// ============================================================================

/**
 * @test File Operations
 * Tests for KitFile class including reading, writing, formatted output,
 * error handling, and resource management.
 */
TEST_SUITE("File Operations") {
    TEST_CASE("KitFile write and read") {
        const std::string test_file = "test_doctest_output.txt";

        // Write test
        {
            FileMgr write_file(test_file, std::ios::out);
            REQUIRE(write_file.is_valid());
            write_file.write("Line 1\n");
            write_file.write("Line 2\n");
            write_file.write("Line 3\n");
            write_file.close();
        }

        // Read test
        {
            FileMgr read_file(test_file, std::ios::in);
            REQUIRE(read_file.is_valid());
            auto lines = read_file.read_all();

            CHECK(lines.size() == 3);
            CHECK(lines[0] == "Line 1");
            CHECK(lines[1] == "Line 2");
            CHECK(lines[2] == "Line 3");
            read_file.close();
        }

        // Cleanup
        std::filesystem::remove(test_file);
    }

    TEST_CASE("KitFile formatted write") {
        const std::string test_file = "test_doctest_format.txt";

        {
            FileMgr format_file(test_file, std::ios::out);
            REQUIRE(format_file.is_valid());
            format_file.write("Number: {}, String: {}\n", 42, "hello");
            format_file.close();
        }

        // Verify content
        {
            std::ifstream file(test_file);
            std::string content((std::istreambuf_iterator<char>(file)),
                                std::istreambuf_iterator<char>());
            CHECK(content == "Number: 42, String: hello\n");
        }

        // Cleanup
        std::filesystem::remove(test_file);
    }

    TEST_CASE("KitFile invalid file throws exception") {
        CHECK_THROWS_AS(
            FileMgr("/invalid/path/that/does/not/exist.txt", std::ios::in),
            FileException);
    }
}

// ============================================================================
// RANDOM NUMBER GENERATION TESTS
// ============================================================================

/**
 * @test Random Number Generation
 * Tests for random number generators including various types and ranges.
 */
TEST_SUITE("Random Number Generation") {
    TEST_CASE("random_int within range") {
        int min = 10;
        int max = 20;
        
        for (int i = 0; i < 100; ++i) {
            int value = random_int(min, max);
            CHECK(value >= min);
            CHECK(value <= max);
        }
    }

    TEST_CASE("random_double within range") {
        double min = 0.0;
        double max = 1.0;
        
        for (int i = 0; i < 100; ++i) {
            double value = random_double(min, max);
            CHECK(value >= min);
            CHECK(value < max);  // max is exclusive for doubles
        }
    }

    TEST_CASE("random_long_long within range") {
        long long min = 1000LL;
        long long max = 2000LL;
        
        for (int i = 0; i < 100; ++i) {
            long long value = random_long_long(min, max);
            CHECK(value >= min);
            CHECK(value <= max);
        }
    }

    TEST_CASE("random_int32 within range") {
        int32_t min = -100;
        int32_t max = 100;
        
        for (int i = 0; i < 100; ++i) {
            int32_t value = random_int32(min, max);
            CHECK(value >= min);
            CHECK(value <= max);
        }
    }

    TEST_CASE("random_int64 within range") {
        int64_t min = -1000LL;
        int64_t max = 1000LL;
        
        for (int i = 0; i < 100; ++i) {
            int64_t value = random_int64(min, max);
            CHECK(value >= min);
            CHECK(value <= max);
        }
    }

    TEST_CASE("uniform_random_int alias") {
        int min = 5;
        int max = 15;
        
        for (int i = 0; i < 50; ++i) {
            int value = uniform_random_int(min, max);
            CHECK(value >= min);
            CHECK(value <= max);
        }
    }

    TEST_CASE("uniform_random_double alias") {
        double min = -1.0;
        double max = 1.0;
        
        for (int i = 0; i < 50; ++i) {
            double value = uniform_random_double(min, max);
            CHECK(value >= min);
            CHECK(value < max);
        }
    }

    TEST_CASE("random distribution variety") {
        // Test that we get different values (not all the same)
        std::vector<int> values(100);
        for (auto& val : values) {
            val = random_int(1, 100);
        }
        
        // Check that not all values are the same
        auto first = values[0];
        bool has_different = false;
        for (const auto& val : values) {
            if (val != first) {
                has_different = true;
                break;
            }
        }
        CHECK(has_different);
    }
}

// ============================================================================
// COMMON CONCEPTS TESTS
// ============================================================================

/**
 * @test Common Concepts
 * Tests for type concepts and predicates defined in common_concepts.hpp
 */
TEST_SUITE("Common Concepts") {
    TEST_CASE("Stringable concept") {
        // Test built-in types
        static_assert(Stringable<int>);
        static_assert(Stringable<double>);
        static_assert(Stringable<const char*>);
        static_assert(Stringable<std::string>);
        
        // Test that we can use Stringable in template functions
        auto test_stringable = [](const Stringable auto& value) -> std::string {
            return convert_stringable_to_string(value);
        };
        
        CHECK(test_stringable(42) == "42");
        CHECK(test_stringable(3.14) == "3.140000");
        CHECK(test_stringable("hello") == "hello");
        CHECK(test_stringable(std::string("world")) == "world");
    }

    TEST_CASE("OneOf concept") {
        static_assert(OneOf<int, int, float, double>);
        static_assert(OneOf<float, int, float, double>);
        static_assert(!OneOf<std::string, int, float, double>);
        
        // Test in template context
        auto process_oneof = []<OneOf<int, float, double> T>(T value) {
            return sizeof(T);
        };
        
        CHECK(process_oneof(42) == sizeof(int));
        CHECK(process_oneof(3.14f) == sizeof(float));
        CHECK(process_oneof(2.718) == sizeof(double));
    }

    TEST_CASE("UnaryPred concept") {
        auto is_positive = [](const int& x) { return x > 0; };
        static_assert(UnaryPred<decltype(is_positive), int>);
        
        CHECK(is_positive(5) == true);
        CHECK(is_positive(-3) == false);
    }

    TEST_CASE("BinaryPred concept") {
        auto are_equal = [](const int& a, const int& b) { return a == b; };
        static_assert(BinaryPred<decltype(are_equal), int, int>);
        
        CHECK(are_equal(5, 5) == true);
        CHECK(are_equal(5, 3) == false);
    }

    TEST_CASE("UnaryFunc concept") {
        auto square = [](int x) { return x * x; };
        static_assert(UnaryFunc<decltype(square), int>);
        
        CHECK(square(5) == 25);
        CHECK(square(-3) == 9);
    }

    TEST_CASE("BinaryFunc concept") {
        auto add = [](int a, int b) { return a + b; };
        static_assert(BinaryFunc<decltype(add), int, int>);
        
        CHECK(add(3, 4) == 7);
        CHECK(add(-2, 5) == 3);
    }

    TEST_CASE("InvocableReturns concept") {
        auto get_length = [](const std::string& s) { return s.length(); };
        static_assert(InvocableReturns<decltype(get_length), size_t, std::string>);
        
        CHECK(get_length("hello") == 5);
        CHECK(get_length("") == 0);
    }

    TEST_CASE("InvocableReturnsVoid concept") {
        int counter = 0;
        auto increment = [&counter](int x) { counter += x; };
        static_assert(InvocableReturnsVoid<decltype(increment), int>);
        
        increment(5);
        CHECK(counter == 5);
        increment(3);
        CHECK(counter == 8);
    }
}

// ============================================================================
// CONSOLE OPERATIONS TESTS
// ============================================================================

/**
 * @test Console Operations
 * Tests for console I/O functions including formatted output and input.
 */
TEST_SUITE("Console Operations") {
    TEST_CASE("write_string_to_console basic") {
        // This test mainly ensures the function compiles and doesn't throw
        CHECK_NOTHROW(write_string_to_console("Test message"));
    }

    TEST_CASE("write_a_line_to_console basic") {
        CHECK_NOTHROW(write_a_line_to_console("Test line"));
    }

    TEST_CASE("write_a_line_to_console formatted") {
        CHECK_NOTHROW(
            write_a_line_to_console("Number: {}, Text: '{}'", 123, "test"));
    }
    TEST_CASE("Test read from console") {
        std::string s = read_a_line_from_console("Test read line:");
        write_a_line_to_console(s);
    }
}

/**
 * @test Edge Cases
 * Tests for edge cases and boundary conditions across all string operations.
 */
TEST_SUITE("Edge Cases") {
    TEST_CASE("slice with count exceeding string length") {
        std::string test_str = "Hello";
        auto sliced = slice(test_str, 2, 10);
        CHECK(std::string(sliced) == "llo");
    }

    TEST_CASE("prefix with n equal to string length") {
        std::string test_str = "Hello";
        auto pref = prefix(test_str, 5);
        CHECK(std::string(pref) == "Hello");
    }

    TEST_CASE("suffix with n equal to string length") {
        std::string test_str = "Hello";
        auto suff = suffix(test_str, 5);
        CHECK(std::string(suff) == "Hello");
    }

    TEST_CASE("trim string with no spaces") {
        std::string no_trim_str = "Hello";
        auto trimmed = get_trimmed(no_trim_str);
        CHECK(trimmed == "Hello");
    }

    TEST_CASE("replace_all with empty old_str does nothing") {
        std::string test_str = "hello world";
        replace_all(test_str, "", "x");
        CHECK(test_str == "hello world");
    }

    TEST_CASE("remove_all with empty pattern does nothing") {
        std::string test_str = "hello world";
        remove_all(test_str, "");
        CHECK(test_str == "hello world");
    }

    TEST_CASE("contains with empty substring returns false") {
        std::string test_str = "Hello";
        CHECK(contains(test_str, "") == false);
    }
}

// ============================================================================
// TIME UTILITIES TESTS
// ============================================================================

/**
 * @brief Simple test function for timing benchmarks
 */
void simple_sleep_function() { sleep(0.01); }

/**
 * @test Time Tests
 * Comprehensive tests for time utilities including:
 * - Timer class functionality
 * - Stopwatch class with pause/resume
 * - DateTime manipulation and arithmetic
 * - Function benchmarking
 * - Time formatting and precision
 */
TEST_CASE("Time Tests") {
    SUBCASE("Timer basic functionality") {
        Timer timer;
        CHECK(!timer.get_duration_seconds().has_value());

        timer.start();
        sleep(0.01);
        auto duration = timer.get_duration_seconds();
        CHECK(duration.has_value());
        CHECK(duration->count() >= 0.01);
    }

    SUBCASE("Timer reset") {
        Timer timer;
        timer.start();
        sleep(0.01);
        timer.reset();
        CHECK(!timer.get_duration_seconds().has_value());
    }

    SUBCASE("Stopwatch basic start and elapsed") {
        Stopwatch sw;
        CHECK(sw.get_elapsed().count() == 0.0);
        CHECK(!sw.is_running());
        CHECK(!sw.is_paused());

        sw.start();
        CHECK(sw.is_running());
        CHECK(!sw.is_paused());
        sleep(0.02);
        auto elapsed = sw.get_elapsed();
        CHECK(elapsed.count() >= 0.02);
    }

    SUBCASE("Stopwatch pause and resume") {
        Stopwatch sw;
        sw.start();
        sleep(0.02);

        sw.pause();
        CHECK(!sw.is_running());
        CHECK(sw.is_paused());
        auto paused_elapsed = sw.get_elapsed();

        sleep(0.02);
        auto still_paused_elapsed = sw.get_elapsed();
        CHECK(still_paused_elapsed.count() == paused_elapsed.count());

        sw.resume();
        CHECK(sw.is_running());
        CHECK(!sw.is_paused());
        sleep(0.02);
        auto resumed_elapsed = sw.get_elapsed();
        CHECK(resumed_elapsed.count() > paused_elapsed.count());
    }

    SUBCASE("Stopwatch multiple pause/resume cycles") {
        Stopwatch sw;
        sw.start();
        sleep(0.01);
        sw.pause();

        sw.resume();
        sleep(0.01);
        sw.pause();

        sw.resume();
        sleep(0.01);
        sw.stop();

        auto total_elapsed = sw.get_elapsed();
        CHECK(total_elapsed.count() >= 0.03);
        CHECK(!sw.is_running());
        CHECK(sw.is_paused());
    }

    SUBCASE("Stopwatch reset") {
        Stopwatch sw;
        sw.start();
        sleep(0.02);
        sw.pause();

        CHECK(sw.get_elapsed().count() > 0);
        sw.reset();
        CHECK(sw.get_elapsed().count() == 0);
        CHECK(!sw.is_running());
        CHECK(!sw.is_paused());
    }

    SUBCASE("Stopwatch start when already running does nothing") {
        Stopwatch sw;
        sw.start();
        auto first_elapsed = sw.get_elapsed();

        sw.start();
        auto second_elapsed = sw.get_elapsed();

        CHECK(sw.is_running());
        CHECK(second_elapsed.count() >= first_elapsed.count());
    }

    SUBCASE("Stopwatch pause when not running does nothing") {
        Stopwatch sw;
        CHECK_NOTHROW(sw.pause());
        CHECK(!sw.is_running());
        CHECK(!sw.is_paused());
    }

    SUBCASE("Stopwatch resume when not paused starts new timing") {
        Stopwatch sw;
        sw.resume();
        CHECK(sw.is_running());
        CHECK(!sw.is_paused());
    }

    SUBCASE("Stopwatch copy and move semantics") {
        Stopwatch sw1;
        sw1.start();
        sleep(0.01);

        Stopwatch sw2 = sw1;
        CHECK(sw2.is_running());
        CHECK(sw2.get_elapsed().count() > 0);

        Stopwatch sw3 = std::move(sw1);
        CHECK(sw3.is_running());
        CHECK(sw3.get_elapsed().count() > 0);
    }

    SUBCASE("get_today_str") {
        auto today = get_today_str();
        CHECK(today.size() == 10);  // YYYY-MM-DD format
        CHECK(today[4] == '-');
        CHECK(today[7] == '-');
    }

    SUBCASE("get_today_time_detailed_str") {
        auto detailed = get_today_time_detailed_str();
        CHECK(detailed.size() >= 19);  // At least YYYY-MM-DD HH:MM:SS format
        CHECK(detailed[4] == '-');
        CHECK(detailed[7] == '-');
        CHECK(detailed[10] == ' ');
        CHECK(detailed[13] == ':');
        CHECK(detailed[16] == ':');
    }

    SUBCASE("time_a_function with simple sleep") {
        TimingSetting setting;
        setting.times_to_run = 5;
        setting.print_after_each_run = false;
        setting.print_max_time = false;
        setting.print_min_time = false;
        setting.print_average_time = false;

        CHECK_NOTHROW(time_a_function("simple sleep test", setting,
                                      simple_sleep_function));
    }

    SUBCASE("time_a_function with DEFAULT_TIMING_SETTING") {
        CHECK_NOTHROW(time_a_function("default timing test",
                                      DEFAULT_TIMING_SETTING,
                                      simple_sleep_function));
    }

    SUBCASE("format_duration with different precisions") {
        using namespace std::chrono;

        CHECK(format_duration(duration<double>(1.0),
                              TimePrecision::second_precs) == "1.00 s");
        CHECK(format_duration(duration<double>(0.001),
                              TimePrecision::millisecond_precs) == "1.00 ms");
        CHECK(format_duration(duration<double>(0.000001),
                              TimePrecision::microsecond_precs) == "1.00 μs");
    }

    SUBCASE("sleep function") {
        Timer timer;
        timer.start();
        sleep(0.05);
        auto elapsed = timer.get_duration_seconds();
        CHECK(elapsed.has_value());
        CHECK(elapsed->count() >= 0.04);  // Allow some tolerance
    }

    SUBCASE("DateTime now and to_string") {
        DateTime t = DateTime::now();
        auto str = t.to_string();
        CHECK(!str.empty());
    }

    SUBCASE("DateTime arithmetic with days") {
        DateTime t = DateTime::now();
        DateTime t_plus_3 = t + days{3};
        auto str = t_plus_3.to_string();
        CHECK(!str.empty());
    }

    SUBCASE("DateTime from_timestamp") {
        DateTime t = DateTime::from_timestamp(0.0);
        auto str = t.to_string();
        CHECK(!str.empty());
    }

    SUBCASE("DateTime from_ymd") {
        DateTime t = DateTime::from_ymd(2024, 1, 1, 12, 30, 45);
        auto str = t.to_string();
        CHECK(!str.empty());
    }

    SUBCASE("DateTime invalid date throws exception") {
        CHECK_THROWS_AS(DateTime::from_ymd(2024, 13, 1),
                        DateTime::InvalidDateError);
        CHECK_THROWS_AS(DateTime::from_ymd(2024, 2, 30),
                        DateTime::InvalidDateError);
    }
}

// ============================================================================
// RANDOM NUMBER GENERATION TESTS
// ============================================================================

/**
 * @brief Test function for random number generation benchmarking
 */
void get_random_int_and_print() {
    int i = random_int(-100, 100);
    write_string_to_console("{}\n", i);
}

/**
 * @test Random Kits
 * Tests for random number generation functions including range validation
 * and performance benchmarking.
 */
TEST_CASE("random_kits") {
    CHECK(random_int(0, 100) >= 0);
    CHECK(random_int(0, 100) <= 100);
    time_a_function("random int", DEFAULT_TIMING_SETTING, get_random_int_and_print);
}