#include "doctest.h"
#include <ostream>
#include "include/string_kits.hpp"
#include <vector>
#include <string>
#include <array>

using namespace zuc;

/**
 * @test String View Operations
 * Tests for string slicing, prefix/suffix operations, and string manipulation
 * using zero-allocation string views.
 */
TEST_SUITE("String View Operations") {
    TEST_CASE("string_slice basic functionality") {
        std::string test_str = "Hello World";
        std::string_view sv = test_str;

        auto sliced = string_slice(sv, 6, 5);
        CHECK(std::string(sliced) == "World");
    }

    TEST_CASE("string_slice with empty string") {
        std::string empty_str = "";
        std::string_view sv = empty_str;
        auto sliced = string_slice(sv, 0, 5);
        CHECK(sliced.empty());
    }

    TEST_CASE("string_slice at boundary") {
        std::string test_str = "Hello";
        std::string_view sv = test_str;
        auto sliced = string_slice(sv, 5, 0);
        CHECK(sliced.empty());
    }

    TEST_CASE("string_slice count exceeds length") {
        std::string test_str = "Hello";
        std::string_view sv = test_str;
        auto sliced = string_slice(sv, 2, 10);
        CHECK(std::string(sliced) == "llo");
    }

    TEST_CASE("prefix basic functionality") {
        std::string test_str = "Hello World";
        std::string_view sv = test_str;

        auto pref = prefix(sv, 5);
        CHECK(std::string(pref) == "Hello");
    }

    TEST_CASE("prefix entire string") {
        std::string test_str = "Hello";
        std::string_view sv = test_str;
        auto pref = prefix(sv, 5);
        CHECK(std::string(pref) == "Hello");
    }

    TEST_CASE("prefix empty string") {
        std::string empty_str = "";
        std::string_view sv = empty_str;
        auto pref = prefix(sv, 0);
        CHECK(pref.empty());
    }

    TEST_CASE("suffix basic functionality") {
        std::string test_str = "Hello World";
        std::string_view sv = test_str;

        auto suff = suffix(sv, 5);
        CHECK(std::string(suff) == "World");
    }

    TEST_CASE("suffix entire string") {
        std::string test_str = "Hello";
        std::string_view sv = test_str;
        auto suff = suffix(sv, 5);
        CHECK(std::string(suff) == "Hello");
    }

    TEST_CASE("remove_prefix basic functionality") {
        std::string test_str = "Hello World";
        std::string_view sv = test_str;

        auto no_pref = remove_prefix(sv, 6);
        CHECK(std::string(no_pref) == "World");
    }

    TEST_CASE("remove_prefix entire string") {
        std::string test_str = "Hello";
        std::string_view sv = test_str;
        auto no_pref = remove_prefix(sv, 5);
        CHECK(no_pref.empty());
    }

    TEST_CASE("remove_suffix basic functionality") {
        std::string test_str = "Hello World";
        std::string_view sv = test_str;

        auto no_suff = remove_suffix(sv, 6);
        CHECK(std::string(no_suff) == "Hello");
    }

    TEST_CASE("remove_suffix entire string") {
        std::string test_str = "Hello";
        std::string_view sv = test_str;
        auto no_suff = remove_suffix(sv, 5);
        CHECK(no_suff.empty());
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

    TEST_CASE("get_trimmed_left removes tabs") {
        std::string test_str = "\t\tHello World";
        auto trimmed_left = get_trimmed_left(test_str);
        CHECK(trimmed_left == "Hello World");
    }

    TEST_CASE("get_trimmed_right removes trailing spaces") {
        std::string test_str = "  Hello World  ";
        auto trimmed_right = get_trimmed_right(test_str);
        CHECK(trimmed_right == "  Hello World");
    }

    TEST_CASE("get_trimmed_right removes newlines") {
        std::string test_str = "Hello World\n\n";
        auto trimmed_right = get_trimmed_right(test_str);
        CHECK(trimmed_right == "Hello World");
    }

    TEST_CASE("get_trimmed removes both leading and trailing spaces") {
        std::string test_str = "  Hello World  ";
        auto trimmed = get_trimmed(test_str);
        CHECK(trimmed == "Hello World");
    }

    TEST_CASE("get_trimmed removes mixed whitespace") {
        std::string test_str = " \t\nHello World\n\t ";
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

    TEST_CASE("trim_left_view returns trimmed view") {
        std::string_view sv = "  View Test  ";
        auto trimmed_view = trim_left_view(sv);
        CHECK(std::string(trimmed_view) == "View Test  ");
    }

    TEST_CASE("trim_right_view returns trimmed view") {
        std::string_view sv = "  View Test  ";
        auto trimmed_view = trim_right_view(sv);
        CHECK(std::string(trimmed_view) == "  View Test");
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

    TEST_CASE("trim all whitespace") {
        std::string whitespace_str = " \t\n\r \t\n";
        auto trimmed = get_trimmed(whitespace_str);
        CHECK(trimmed.empty());
    }

    TEST_CASE("trim no trimming needed") {
        std::string no_trim_str = "Hello World";
        auto trimmed = get_trimmed(no_trim_str);
        CHECK(trimmed == "Hello World");
    }

    TEST_CASE("trim returns reference") {
        std::string test_str = "  test  ";
        auto& ref = trim(test_str);
        CHECK(&ref == &test_str);
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

    TEST_CASE("split by single character with empty parts") {
        std::string test_str = "apple,,banana,,cherry";
        auto parts = split(test_str, ',');

        CHECK(parts.size() == 5);
        CHECK(std::string(parts[0]) == "apple");
        CHECK(parts[1].empty());
        CHECK(std::string(parts[2]) == "banana");
        CHECK(parts[3].empty());
        CHECK(std::string(parts[4]) == "cherry");
    }

    TEST_CASE("split by single character with trailing delimiter") {
        std::string test_str = "apple,banana,cherry,";
        auto parts = split(test_str, ',');

        CHECK(parts.size() == 4);
        CHECK(std::string(parts[0]) == "apple");
        CHECK(std::string(parts[1]) == "banana");
        CHECK(std::string(parts[2]) == "cherry");
        CHECK(parts[3].empty());
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

    TEST_CASE("split_by_any with consecutive delimiters") {
        std::string test_str = "apple,,banana;;cherry";
        auto parts = split_by_any(test_str, ",;");

        CHECK(parts.size() == 5);
        CHECK(parts[0] == "apple");
        CHECK(parts[1].empty());
        CHECK(parts[2] == "banana");
        CHECK(parts[3].empty());
        CHECK(parts[4] == "cherry");
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

    TEST_CASE("split_to_string by string delimiter") {
        std::string test_str = "apple::banana::cherry";
        auto string_parts = split_to_string(test_str, "::");

        CHECK(string_parts.size() == 3);
        CHECK(string_parts[0] == "apple");
        CHECK(string_parts[1] == "banana");
        CHECK(string_parts[2] == "cherry");
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

    TEST_CASE("split with null delimiter") {
        auto parts = split("hello", '\0');
        CHECK(parts.empty());
    }

    TEST_CASE("split_by_any empty delimiters") {
        std::string test_str = "apple,banana,cherry";
        auto parts = split_by_any(test_str, "");
        CHECK(parts.size() == 1);
        CHECK(parts[0] == "apple,banana,cherry");
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

    TEST_CASE("join single element") {
        std::vector<std::string_view> strings = {"Hello"};
        auto joined = join(strings, ", ");
        CHECK(joined == "Hello");
    }

    TEST_CASE("join two elements") {
        std::vector<std::string_view> strings = {"Hello", "World"};
        auto joined = join(strings, " ");
        CHECK(joined == "Hello World");
    }

    TEST_CASE("join with empty strings") {
        std::vector<std::string_view> strings = {"Hello", "", "World"};
        auto joined = join(strings, ",");
        CHECK(joined == "Hello,,World");
    }

    TEST_CASE("join with multi-character delimiter") {
        std::vector<std::string_view> strings = {"A", "B", "C"};
        auto joined = join(strings, "::");
        CHECK(joined == "A::B::C");
    }

    TEST_CASE("join large number of strings") {
        std::vector<std::string_view> strings(100, "test");
        auto joined = join(strings, ",");
        CHECK(joined.size() == 100 * 4 + 99); // 100 * "test" + 99 commas
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

    TEST_CASE("Match with different types") {
        CHECK(match_any(std::string("test"), {"test", "other"}) == true);
        CHECK(match_any(42, {"42", "43"}) == true);
    }

    TEST_CASE("Match empty targets") {
        CHECK(match_any("hello", {}) == false);
    }

    TEST_CASE("Match with initializer_list") {
        CHECK(match_any("target", {"target1", "target", "target2"}) == true);
        CHECK(match_any("missing", {"a", "b", "c"}) == false);
    }

    TEST_CASE("Match case sensitive") {
        CHECK(match_any("Hello", {"hello", "Hello"}) == true);
        CHECK(match_any("Hello", {"hello", "HELLO"}) == false);
    }

    TEST_CASE("Match with empty string") {
        CHECK(match_any("", {"", "test"}) == true);
        CHECK(match_any("", {"test", "other"}) == false);
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

    TEST_CASE("contains with string_view") {
        std::string test_str = "Hello World";
        std::string_view sv = test_str;
        CHECK(contains(sv, "World") == true);
        CHECK(contains(sv, "world") == false);
    }

    TEST_CASE("contains case sensitive") {
        std::string test_str = "Hello World";
        CHECK(contains(test_str, "hello") == false);
        CHECK(contains(test_str, "Hello") == true);
    }

    TEST_CASE("contains empty substring") {
        std::string test_str = "Hello World";
        CHECK(contains(test_str, "") == false);
    }

    TEST_CASE("contains empty string") {
        CHECK(contains("", "test") == false);
    }

    TEST_CASE("contains_any returns true if any substring exists") {
        std::string test_str = "Hello World Test";
        std::array<std::string_view, 3> search_strings = {"Hello", "World", "Test"};
        CHECK(contains_any(test_str, search_strings) == true);
    }

    TEST_CASE("contains_any returns false if none exist") {
        std::string test_str = "Hello World Test";
        std::array<std::string_view, 3> search_strings = {"XYZ", "ABC", "DEF"};
        CHECK(contains_any(test_str, search_strings) == false);
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

    TEST_CASE("contains_all with multiple matches") {
        std::string test_str = "test test test";
        std::array<std::string_view, 2> search_strings = {"test", "test"};
        CHECK(contains_all(test_str, search_strings) == true);
    }

    TEST_CASE("contains_any with empty search strings") {
        std::string test_str = "Hello World";
        CHECK(contains_any(test_str, std::vector<std::string_view>{}) == false);
    }

    TEST_CASE("contains_all with empty search strings") {
        std::string test_str = "Hello World";
        CHECK(contains_all(test_str, std::vector<std::string_view>{}) == false);
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

    TEST_CASE("replace_all with longer replacement") {
        std::string test_str = "test";
        replace_all(test_str, "test", "testing");
        CHECK(test_str == "testing");
    }

    TEST_CASE("replace_all with shorter replacement") {
        std::string test_str = "testing";
        replace_all(test_str, "testing", "test");
        CHECK(test_str == "test");
    }

    TEST_CASE("replace_all repeated pattern") {
        std::string test_str = "aaa";
        replace_all(test_str, "a", "b");
        CHECK(test_str == "bbb");
    }

    TEST_CASE("replace_all no match") {
        std::string test_str = "hello world";
        replace_all(test_str, "xyz", "abc");
        CHECK(test_str == "hello world");
    }

    TEST_CASE("replace_all empty old_str") {
        std::string test_str = "hello world";
        replace_all(test_str, "", "x");
        CHECK(test_str == "hello world");
    }

    TEST_CASE("get_all_replaced returns copy") {
        std::string test_str = "test123test456test";
        auto replaced = get_all_replaced(test_str, "test", "demo");
        CHECK(replaced == "demo123demo456demo");
        CHECK(test_str == "test123test456test");  // original unchanged
    }

    TEST_CASE("get_all_replaced with empty new_str") {
        std::string test_str = "hello world hello";
        auto replaced = get_all_replaced(test_str, "hello", "");
        CHECK(replaced == " world ");
    }

    TEST_CASE("replace_all with empty new_str") {
        std::string test_str = "hello world hello";
        replace_all(test_str, "hello", "");
        CHECK(test_str == " world ");
    }

    TEST_CASE("replace_all overlapping patterns") {
        std::string test_str = "aaa";
        replace_all(test_str, "aa", "b");
        CHECK(test_str == "ba");
    }

    TEST_CASE("replace_all case sensitive") {
        std::string test_str = "Hello hello HELLO";
        replace_all(test_str, "hello", "hi");
        CHECK(test_str == "Hello hi HELLO");
    }

    TEST_CASE("replace_all with special characters") {
        std::string test_str = "hello\nworld\nhello";
        replace_all(test_str, "\n", " ");
        CHECK(test_str == "hello world hello");
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

    TEST_CASE("remove_all no match") {
        std::string test_str = "hello world";
        remove_all(test_str, "xyz");
        CHECK(test_str == "hello world");
    }

    TEST_CASE("remove_all empty pattern") {
        std::string test_str = "hello world";
        remove_all(test_str, "");
        CHECK(test_str == "hello world");
    }

    TEST_CASE("remove_all multiple patterns in-place") {
        std::string test_str = "test123test456test789";
        std::array<std::string_view, 2> to_remove = {"123", "456"};
        remove_all(test_str, to_remove);
        CHECK(test_str == "testtesttest789");
    }

    TEST_CASE("remove_all returns reference") {
        std::string test_str = "hello world hello";
        auto& ref = remove_all(test_str, "hello");
        CHECK(&ref == &test_str);
    }

    TEST_CASE("get_all_removed with empty result") {
        std::string test_str = "testtesttest";
        auto removed = get_all_removed(test_str, "test");
        CHECK(removed.empty());
    }
}

/**
 * @test Repeat Operations
 * Tests for string repetition functionality.
 */
TEST_SUITE("Repeat Operations") {
    TEST_CASE("repeat basic") {
        std::string test_str = "ab";
        auto repeated = repeat(test_str, 3);
        CHECK(repeated == "ababab");
    }

    TEST_CASE("repeat zero times") {
        std::string test_str = "hello";
        auto repeated = repeat(test_str, 0);
        CHECK(repeated.empty());
    }

    TEST_CASE("repeat empty string") {
        std::string test_str = "";
        auto repeated = repeat(test_str, 5);
        CHECK(repeated.empty());
    }

    TEST_CASE("repeat single character") {
        std::string test_str = "x";
        auto repeated = repeat(test_str, 10);
        CHECK(repeated == "xxxxxxxxxx");
    }

    TEST_CASE("repeat large number") {
        std::string test_str = "ab";
        auto repeated = repeat(test_str, 100);
        CHECK(repeated.size() == 200);
        CHECK(repeated.substr(0, 6) == "ababab");
    }
}

/**
 * @test String Conversion Operations
 * Tests for string conversion utilities.
 */
TEST_SUITE("String Conversion Operations") {
    TEST_CASE("convert_stringable_to_string from std::string") {
        std::string str = "hello";
        auto converted = convert_stringable_to_string(str);
        CHECK(converted == "hello");
    }

    TEST_CASE("convert_stringable_to_string from const char*") {
        const char* cstr = "world";
        auto converted = convert_stringable_to_string(cstr);
        CHECK(converted == "world");
    }

    TEST_CASE("convert_stringable_to_string from int") {
        int num = 42;
        auto converted = convert_stringable_to_string(num);
        CHECK(converted == "42");
    }

    TEST_CASE("convert_stringable_to_string from double") {
        double num = 3.14;
        auto converted = convert_stringable_to_string(num);
        CHECK(converted.find("3.14") != std::string::npos);
    }

    TEST_CASE("convert_stringable_to_string from float") {
        float num = 2.5f;
        auto converted = convert_stringable_to_string(num);
        CHECK(converted.find("2.5") != std::string::npos);
    }

    TEST_CASE("to_string_view from span") {
        std::vector<char> data = {'h', 'e', 'l', 'l', 'o'};
        auto sv = to_string_view(std::span(data));
        CHECK(sv == "hello");
    }

    TEST_CASE("to_string_view from array") {
        std::array<char, 5> data = {'w', 'o', 'r', 'l', 'd'};
        auto sv = to_string_view(std::span(data));
        CHECK(sv == "world");
    }

    TEST_CASE("to_string_view empty span") {
        std::vector<char> data;
        auto sv = to_string_view(std::span(data));
        CHECK(sv.empty());
    }
}

/**
 * @test String Edge Cases
 * Tests for edge cases and boundary conditions in string operations.
 */
TEST_SUITE("String Edge Cases") {
    TEST_CASE("operations on empty string") {
        std::string empty_str = "";
        
        CHECK(get_trimmed(empty_str).empty());
        CHECK(split(empty_str, ',').empty());
        CHECK(join(std::vector<std::string_view>{}).empty());
        CHECK(contains(empty_str, "test") == false);
    }

    TEST_CASE("operations on single character") {
        std::string single = "a";
        
        CHECK(get_trimmed(single) == "a");
        CHECK(split(single, ',').size() == 1);
        CHECK(contains(single, "a") == true);
    }

    TEST_CASE("operations on very long string") {
        std::string long_str(10000, 'a');
        
        auto trimmed = get_trimmed(long_str);
        CHECK(trimmed.size() == 10000);
        
        auto parts = split(long_str, 'a');
        CHECK(parts.size() == 10001);
    }

    TEST_CASE("unicode character handling") {
        std::string unicode_str = "Hello 世界";
        
        auto trimmed = get_trimmed(unicode_str);
        CHECK(trimmed == "Hello 世界");
        
        CHECK(contains(unicode_str, "世界") == true);
    }

    TEST_CASE("special characters in split") {
        std::string special = "a\nb\tc\rd";
        auto parts = split_by_any(special, "\n\t\r");
        CHECK(parts.size() == 4);
        CHECK(parts[0] == "a");
        CHECK(parts[1] == "b");
        CHECK(parts[2] == "c");
        CHECK(parts[3] == "d");
    }

    TEST_CASE("replace with same string") {
        std::string test_str = "hello";
        replace_all(test_str, "hello", "hello");
        CHECK(test_str == "hello");
    }

    TEST_CASE("remove entire string") {
        std::string test_str = "hello";
        remove_all(test_str, "hello");
        CHECK(test_str.empty());
    }

    TEST_CASE("join with very long delimiter") {
        std::vector<std::string_view> strings = {"a", "b", "c"};
        std::string long_delimiter(100, 'x');
        auto joined = join(strings, long_delimiter);
        CHECK(joined.size() == 3 + 2 * 100);
    }
}