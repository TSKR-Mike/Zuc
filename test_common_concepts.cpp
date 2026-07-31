#include "doctest.h"
#include <ostream>
#include "include/common_concepts.hpp"
#include "include/string_kits.hpp"
#include <deque>
#include <vector>
#include <list>
#include <array>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <string>
#include <functional>
#include<iostream>

using namespace zuc;

/**
 * @test Stringable Concept Tests
 * Tests for the Stringable concept which identifies types that can be converted to strings.
 */
TEST_SUITE("Stringable Concept") {
    TEST_CASE("built-in types are Stringable") {
        CHECK(Stringable<int>);
        CHECK(Stringable<double>);
        CHECK(Stringable<float>);
        CHECK(Stringable<char>);
        CHECK(Stringable<unsigned int>);
        CHECK(Stringable<long>);
        CHECK(Stringable<long long>);
    }

    TEST_CASE("string types are Stringable") {
        CHECK(Stringable<std::string>);
        CHECK(Stringable<const std::string>);
        CHECK(Stringable<std::string&>);
        CHECK(Stringable<const std::string&>);
    }

    TEST_CASE("c-string types are Stringable") {
        CHECK(Stringable<char*>);
        CHECK(Stringable<const char*>);
        CHECK(Stringable<char[]>);
        CHECK(Stringable<const char[]>);
    }

    TEST_CASE("string_view is Stringable") {
        CHECK(Stringable<std::string_view>);
        CHECK(Stringable<const std::string_view>);
    }

    TEST_CASE("non-stringable types") {
        struct NonStringable {};
        CHECK(!Stringable<NonStringable>);
        // Containers are not Stringable even if they contain Stringable elements
        CHECK(!Stringable<std::vector<int>>);
        CHECK(!Stringable<std::map<int, int>>);
        CHECK(!Stringable<std::vector<std::string>>);
    }

    TEST_CASE("convert_stringable_to_string with int") {
        std::string result = convert_stringable_to_string(42);
        CHECK(result == "42");
    }

    TEST_CASE("convert_stringable_to_string with double") {
        std::string result = convert_stringable_to_string(3.14);
        // std::to_string for doubles produces default precision, not exact input
        CHECK(result == "3.140000");
    }

    TEST_CASE("convert_stringable_to_string with string") {
        std::string result = convert_stringable_to_string(std::string("hello"));
        CHECK(result == "hello");
    }

    TEST_CASE("convert_stringable_to_string with c-string") {
        const char* cstr = "world";
        std::string result = convert_stringable_to_string(cstr);
        CHECK(result == "world");
    }
}

/**
 * @test OneOf Concept Tests
 * Tests for the OneOf concept which checks if a type is one of specified types.
 */
TEST_SUITE("OneOf Concept") {
    TEST_CASE("basic type matching") {
        CHECK(OneOf<int, int, float, double>);
        CHECK(OneOf<float, int, float, double>);
        CHECK(OneOf<double, int, float, double>);
        CHECK(!OneOf<char, int, float, double>);
        CHECK(!OneOf<std::string, int, float, double>);
    }

    TEST_CASE("single type list") {
        CHECK(OneOf<int, int>);
        CHECK(!OneOf<float, int>);
    }

    TEST_CASE("multiple types with duplicates") {
        CHECK(OneOf<int, int, int, int>);
        CHECK(OneOf<int, float, int, double>);
    }

    TEST_CASE("template with OneOf constraint") {
        auto process_int = []<OneOf<int, float, double> T>(T value) {
            return sizeof(T);
        };

        CHECK(process_int(42) == sizeof(int));
        CHECK(process_int(3.14f) == sizeof(float));
        CHECK(process_int(2.718) == sizeof(double));
    }
}

/**
 * @test IsNotOneOf Concept Tests
 * Tests for the IsNotOneOf concept which checks if a type is NOT one of specified types.
 */
TEST_SUITE("IsNotOneOf Concept") {
    TEST_CASE("basic type exclusion") {
        CHECK(!IsNotOneOf<int, int, float, double>);
        CHECK(IsNotOneOf<char, int, float, double>);
        CHECK(IsNotOneOf<std::string, int, float, double>);
    }

    TEST_CASE("all types excluded") {
        CHECK(!IsNotOneOf<int, int>);
        CHECK(!IsNotOneOf<float, float>);
    }

    TEST_CASE("no types excluded") {
        CHECK(IsNotOneOf<int, float>);
        CHECK(IsNotOneOf<std::string, int, double>);
    }
}

/**
 * @test UnaryPred Concept Tests
 * Tests for the UnaryPred concept which identifies unary predicate functions.
 */
TEST_SUITE("UnaryPred Concept") {
    TEST_CASE("lambda predicates") {
        auto is_positive = [](const int& x) { return x > 0; };
        auto is_even = [](const int& x) { return x % 2 == 0; };
        auto is_empty = [](const std::string& s) { return s.empty(); };
        
        CHECK(UnaryPred<decltype(is_positive), int>);
        CHECK(UnaryPred<decltype(is_even), int>);
        CHECK(UnaryPred<decltype(is_empty), std::string>);
    }

    TEST_CASE("function pointers") {
        bool (*is_positive)(const int&) = [](const int& x) { return x > 0; };
        CHECK(UnaryPred<decltype(is_positive), int>);
    }

    TEST_CASE("std::function") {
        std::function<bool(const int&)> pred = [](const int& x) { return x > 0; };
        CHECK(UnaryPred<decltype(pred), int>);
    }

    TEST_CASE("non-predicate functions") {
        // Note: std::predicate accepts functions that return types convertible to bool
        // So functions returning int would still satisfy std::predicate since int is convertible to bool
        auto add_one = [](const int& x) { return x + 1; }; // returns int, which is convertible to bool
        CHECK(UnaryPred<decltype(add_one), int>); // This will pass because int is convertible to bool
    }

    TEST_CASE("wrong signature") {
        // std::predicate is flexible about parameter types - it accepts any invocable
        auto wrong_param = [](int x) { return x > 0; }; // takes int, not const int&
        CHECK(UnaryPred<decltype(wrong_param), int>); // This passes because int can be passed to int
    }
}

/**
 * @test BinaryPred Concept Tests
 * Tests for the BinaryPred concept which identifies binary predicate functions.
 */
TEST_SUITE("BinaryPred Concept") {
    TEST_CASE("lambda predicates") {
        auto are_equal = [](const int& a, const int& b) { return a == b; };
        auto less_than = [](const int& a, const int& b) { return a < b; };
        auto same_length = [](const std::string& a, const std::string& b) { 
            return a.length() == b.length(); 
        };
        
        CHECK(BinaryPred<decltype(are_equal), int, int>);
        CHECK(BinaryPred<decltype(less_than), int, int>);
        CHECK(BinaryPred<decltype(same_length), std::string, std::string>);
    }

    TEST_CASE("mixed type predicates") {
        auto int_less_double = [](const int& a, const double& b) { return a < b; };
        CHECK(BinaryPred<decltype(int_less_double), int, double>);
    }

    TEST_CASE("non-predicate functions") {
        // Note: std::predicate accepts functions that return types convertible to bool
        // So functions returning int would still satisfy std::predicate since int is convertible to bool
        auto add = [](const int& a, const int& b) { return a + b; };
        CHECK(BinaryPred<decltype(add), int, int>); // This passes because int is convertible to bool
    }

    TEST_CASE("standard comparison functions") {
        auto equal_to_func = std::equal_to<int>{};
        auto less_func = std::less<int>{};
        auto greater_func = std::greater<int>{};
        
        CHECK(BinaryPred<decltype(equal_to_func), int, int>);
        CHECK(BinaryPred<decltype(less_func), int, int>);
        CHECK(BinaryPred<decltype(greater_func), int, int>);
    }
}

/**
 * @test UnaryFunc Concept Tests
 * Tests for the UnaryFunc concept which identifies unary functions.
 */
TEST_SUITE("UnaryFunc Concept") {
    TEST_CASE("lambda functions") {
        auto square = [](int x) { return x * x; };
        auto to_string = [](int x) { return std::to_string(x); };
        auto increment = [](int& x) { ++x; };
        
        CHECK(UnaryFunc<decltype(square), int>);
        CHECK(UnaryFunc<decltype(to_string), int>);
        CHECK(UnaryFunc<decltype(increment), int&>);
    }

    TEST_CASE("functions with different return types") {
        auto get_size = [](const std::string& s) { return s.size(); };
        auto is_empty = [](const std::string& s) { return s.empty(); };
        
        CHECK(UnaryFunc<decltype(get_size), std::string>);
        CHECK(UnaryFunc<decltype(is_empty), std::string>);
    }

    TEST_CASE("standard functions") {
        // Wrap standard functions in lambdas for proper type deduction
        auto abs_func = [](int x) { return std::abs(x); };
        CHECK(UnaryFunc<decltype(abs_func), int>);
    }
}

/**
 * @test BinaryFunc Concept Tests
 * Tests for the BinaryFunc concept which identifies binary functions.
 */
TEST_SUITE("BinaryFunc Concept") {
    TEST_CASE("lambda functions") {
        auto add = [](int a, int b) { return a + b; };
        auto concatenate = [](const std::string& a, const std::string& b) { 
            return a + b; 
        };
        auto multiply = [](double a, double b) { return a * b; };
        
        CHECK(BinaryFunc<decltype(add), int, int>);
        CHECK(BinaryFunc<decltype(concatenate), std::string, std::string>);
        CHECK(BinaryFunc<decltype(multiply), double, double>);
    }

    TEST_CASE("standard arithmetic functions") {
        auto plus_func = std::plus<int>{};
        auto minus_func = std::minus<int>{};
        auto multiplies_func = std::multiplies<int>{};
        
        CHECK(BinaryFunc<decltype(plus_func), int, int>);
        CHECK(BinaryFunc<decltype(minus_func), int, int>);
        CHECK(BinaryFunc<decltype(multiplies_func), int, int>);
    }

    TEST_CASE("mixed type functions") {
        auto assign = [](int& a, int b) { a = b; };
        CHECK(BinaryFunc<decltype(assign), int&, int>);
    }
}

/**
 * @test InvocableReturns Concept Tests
 * Tests for the InvocableReturns concept which checks callables with specific return types.
 */
TEST_SUITE("InvocableReturns Concept") {
    TEST_CASE("functions returning specific types") {
        auto get_length = [](const std::string& s) { return s.length(); };
        auto get_size = [](const std::vector<int>& v) { return v.size(); };
        auto to_int = [](const std::string& s) { return std::stoi(s); };
        
        CHECK(InvocableReturns<decltype(get_length), size_t, std::string>);
        CHECK(InvocableReturns<decltype(get_size), size_t, std::vector<int>>);
        CHECK(InvocableReturns<decltype(to_int), int, std::string>);
    }

    TEST_CASE("wrong return type") {
        auto get_length = [](const std::string& s) { return s.length(); };
        // Note: InvocableReturns uses std::convertible_to, so size_t is convertible to int
        CHECK(InvocableReturns<decltype(get_length), int, std::string>); // size_t is convertible to int
        CHECK(!InvocableReturns<decltype(get_length), std::string, std::string>); // size_t is not convertible to std::string
    }

    TEST_CASE("functions with multiple parameters") {
        auto add = [](int a, int b) { return a + b; };
        CHECK(InvocableReturns<decltype(add), int, int, int>);
    }

    TEST_CASE("void return type") {
        auto print = [](int x) { std::cout << x; };
        CHECK(!InvocableReturns<decltype(print), int, int>);
    }
}

/**
 * @test InvocableReturnsVoid Concept Tests
 * Tests for the InvocableReturnsVoid concept which identifies callables returning void.
 */
TEST_SUITE("InvocableReturnsVoid Concept") {
    TEST_CASE("void returning functions") {
        auto print_int = [](int x) { std::cout << x; };
        auto clear_vector = [](std::vector<int>& v) { v.clear(); };
        auto noop = []() {};
        
        CHECK(InvocableReturnsVoid<decltype(print_int), int>);
        CHECK(InvocableReturnsVoid<decltype(clear_vector), std::vector<int>&>);
        CHECK(InvocableReturnsVoid<decltype(noop)>);
    }

    TEST_CASE("non-void returning functions") {
        auto add_one = [](int x) { return x + 1; };
        auto get_size = [](const std::vector<int>& v) { return v.size(); };
        
        CHECK(!InvocableReturnsVoid<decltype(add_one), int>);
        CHECK(!InvocableReturnsVoid<decltype(get_size), std::vector<int>>);
    }
}

/**
 * @test RangeLike Concept Tests
 * Tests for the RangeLike concept which identifies types with begin() and end().
 */
TEST_SUITE("RangeLike Concept") {
    TEST_CASE("standard containers") {
        CHECK(RangeLike<std::vector<int>>);
        CHECK(RangeLike<std::list<int>>);
        CHECK(RangeLike<std::deque<int>>);
        CHECK(RangeLike<std::array<int, 5>>);
        CHECK(RangeLike<std::set<int>>);
        CHECK(RangeLike<std::map<int, int>>);
    }

    TEST_CASE("string types") {
        CHECK(RangeLike<std::string>);
        CHECK(RangeLike<std::string_view>);
    }

    TEST_CASE("unordered containers") {
        CHECK(RangeLike<std::unordered_set<int>>);
        CHECK(RangeLike<std::unordered_map<int, int>>);
    }

    TEST_CASE("native arrays") {
        int arr[5] = {1, 2, 3, 4, 5};
        CHECK(RangeLike<decltype(arr)>);
    }

    TEST_CASE("non-range types") {
        CHECK(!RangeLike<int>);
        CHECK(!RangeLike<double>);
        CHECK(!RangeLike<std::pair<int, int>>);
    }

    TEST_CASE("initializer lists") {
        CHECK(RangeLike<std::initializer_list<int>>);
    }
}

/**
 * @test RangeLikeElemTypeSpecified Concept Tests
 * Tests for the RangeLikeElemTypeSpecified concept which checks ranges with specific element types.
 */
TEST_SUITE("RangeLikeElemTypeSpecified Concept") {
    TEST_CASE("correct element types") {
        CHECK(RangeLikeElemTypeSpecified<std::vector<int>, int>);
        CHECK(RangeLikeElemTypeSpecified<std::list<double>, double>);
        CHECK(RangeLikeElemTypeSpecified<std::array<std::string, 3>, std::string>);
    }

    TEST_CASE("incorrect element types") {
        CHECK(!RangeLikeElemTypeSpecified<std::vector<int>, double>);
        CHECK(!RangeLikeElemTypeSpecified<std::list<std::string>, int>);
    }

    TEST_CASE("map element types") {
        CHECK(RangeLikeElemTypeSpecified<std::map<int, std::string>, std::pair<const int, std::string>>);
    }
}

/**
 * @test get_rangelike_value_type Tests
 * Tests for the get_rangelike_value_type type trait.
 */
TEST_SUITE("get_rangelike_value_type") {
    TEST_CASE("vector value type") {
        using ValueType = get_rangelike_value_type<std::vector<int>>;
        CHECK(std::is_same_v<ValueType, int>);
    }

    TEST_CASE("list value type") {
        using ValueType = get_rangelike_value_type<std::list<double>>;
        CHECK(std::is_same_v<ValueType, double>);
    }

    TEST_CASE("array value type") {
        using ValueType = get_rangelike_value_type<std::array<std::string, 5>>;
        CHECK(std::is_same_v<ValueType, std::string>);
    }

    TEST_CASE("string value type") {
        using ValueType = get_rangelike_value_type<std::string>;
        CHECK(std::is_same_v<ValueType, char>);
    }

    TEST_CASE("map value type") {
        using ValueType = get_rangelike_value_type<std::map<int, std::string>>;
        CHECK(std::is_same_v<ValueType, std::pair<const int, std::string>>);
    }

    TEST_CASE("const range") {
        using ValueType = get_rangelike_value_type<const std::vector<int>>;
        CHECK(std::is_same_v<ValueType, int>); // const is removed
    }
}

/**
 * @test Map-Like Type Traits Tests
 * Tests for is_map_like type trait and is_map_like_v variable template.
 */
TEST_SUITE("Map-Like Type Traits") {
    TEST_CASE("map types") {
        CHECK(is_map_like_v<std::map<int, std::string>>);
        CHECK(is_map_like_v<std::multimap<int, std::string>>);
        CHECK(is_map_like_v<std::unordered_map<int, std::string>>);
        CHECK(is_map_like_v<std::unordered_multimap<int, std::string>>);
    }

    TEST_CASE("non-map types") {
        CHECK(!is_map_like_v<std::vector<int>>);
        CHECK(!is_map_like_v<std::set<int>>);
        CHECK(!is_map_like_v<std::list<int>>);
        CHECK(!is_map_like_v<std::string>);
    }

    TEST_CASE("map with custom comparators") {
        CHECK(is_map_like_v<std::map<int, std::string, std::greater<int>>>);
        CHECK(is_map_like_v<std::unordered_map<int, std::string, std::hash<int>>>);
    }
}

/**
 * @test Set-Like Type Traits Tests
 * Tests for is_set_like type trait and is_set_like_v variable template.
 */
TEST_SUITE("Set-Like Type Traits") {
    TEST_CASE("set types") {
        CHECK(is_set_like_v<std::set<int>>);
        CHECK(is_set_like_v<std::multiset<int>>);
        CHECK(is_set_like_v<std::unordered_set<int>>);
        CHECK(is_set_like_v<std::unordered_multiset<int>>);
    }

    TEST_CASE("non-set types") {
        CHECK(!is_set_like_v<std::vector<int>>);
        CHECK(!is_set_like_v<std::map<int, int>>);
        CHECK(!is_set_like_v<std::list<int>>);
        CHECK(!is_set_like_v<std::string>);
    }

    TEST_CASE("set with custom comparators") {
        CHECK(is_set_like_v<std::set<int, std::greater<int>>>);
        CHECK(is_set_like_v<std::unordered_set<int, std::hash<int>>>);
    }
}

/**
 * @test Insertable Concept Tests
 * Tests for the Insertable concept which identifies containers that support insert().
 */
TEST_SUITE("Insertable Concept") {
    TEST_CASE("insertable containers") {
        CHECK(Insertable<std::vector<int>>);
        CHECK(Insertable<std::list<int>>);
        CHECK(Insertable<std::deque<int>>);
        CHECK(Insertable<std::set<int>>);
        CHECK(Insertable<std::multiset<int>>);
    }

    TEST_CASE("non-insertable containers") {
        CHECK(!Insertable<std::array<int, 5>>);
        // std::string actually does have insert(iterator, char) that returns iterator
        CHECK(Insertable<std::string>); 
        // std::map has insert(iterator, value_type) that returns iterator
        CHECK(Insertable<std::map<int, int>>);
    }
}

/**
 * @test NumericType Concept Tests
 * Tests for the NumericType concept which identifies numeric types.
 */
TEST_SUITE("NumericType Concept") {
    TEST_CASE("integral types") {
        CHECK(NumericType<int>);
        CHECK(NumericType<unsigned int>);
        CHECK(NumericType<long>);
        CHECK(NumericType<long long>);
        CHECK(NumericType<short>);
        CHECK(NumericType<char>);
    }

    TEST_CASE("floating point types") {
        CHECK(NumericType<float>);
        CHECK(NumericType<double>);
        CHECK(NumericType<long double>);
    }

    TEST_CASE("non-numeric types") {
        CHECK(!NumericType<std::string>);
        CHECK(!NumericType<std::vector<int>>);
        // Note: bool is integral, so it is considered numeric by this concept
        CHECK(NumericType<bool>); // bool is integral, so it satisfies NumericType
    }

    TEST_CASE("const and volatile variants") {
        CHECK(NumericType<const int>);
        CHECK(NumericType<volatile double>);
        CHECK(NumericType<const volatile float>);
    }
}

/**
 * @test Real-World Use Cases
 * Tests demonstrating practical applications of concepts.
 */
TEST_SUITE("Real-World Use Cases") {
    TEST_CASE("generic function with Stringable constraint") {
        auto print_stringable = []<Stringable T>(const T& value) {
            return convert_stringable_to_string(value);
        };
        
        CHECK(print_stringable(42) == "42");
        CHECK(print_stringable(3.14) == "3.140000"); // std::to_string default precision
        CHECK(print_stringable(std::string("hello")) == "hello");
        CHECK(print_stringable("world") == "world");
    }

    TEST_CASE("container processing with RangeLike") {
        auto process_range = []<RangeLike R>(const R& range) {
            size_t count = 0;
            for (const auto& elem : range) {
                (void)elem; // suppress unused warning
                count++;
            }
            return count;
        };
        
        std::vector<int> vec = {1, 2, 3, 4, 5};
        std::list<double> lst = {1.1, 2.2, 3.3};
        
        CHECK(process_range(vec) == 5);
        CHECK(process_range(lst) == 3);
    }

    TEST_CASE("predicate-based filtering") {
        auto filter = []<RangeLike R, UnaryPred<get_rangelike_value_type<R>> Pred>(
            const R& range, Pred pred) {
            std::vector<get_rangelike_value_type<R>> result;
            for (const auto& elem : range) {
                if (pred(elem)) {
                    result.push_back(elem);
                }
            }
            return result;
        };
        
        std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        auto evens = filter(numbers, [](int x) { return x % 2 == 0; });
        
        CHECK(evens.size() == 5);
        CHECK(evens[0] == 2);
        CHECK(evens[4] == 10);
    }

    TEST_CASE("type-safe arithmetic") {
        auto safe_add = []<NumericType T>(T a, T b) -> T {
            return a + b;
        };
        
        CHECK(safe_add(1, 2) == 3);
        CHECK(safe_add(1.5, 2.5) == doctest::Approx(4.0));
    }

    TEST_CASE("map processing") {
        auto get_keys = []<typename M>(const M& map) requires is_map_like_v<M> {
            std::vector<typename M::key_type> keys;
            for (const auto& [key, value] : map) {
                keys.push_back(key);
                (void)value; // suppress unused warning
            }
            return keys;
        };
        
        std::map<std::string, int> m = {{"a", 1}, {"b", 2}, {"c", 3}};
        auto keys = get_keys(m);
        
        CHECK(keys.size() == 3);
        CHECK(keys[0] == "a");
        CHECK(keys[2] == "c");
    }

    TEST_CASE("conditional compilation with concepts") {
        // Test concept evaluation at compile time
        static_assert(Stringable<int>); // int is Stringable
        static_assert(NumericType<double>); // double is Numeric
        static_assert(RangeLike<std::vector<int>>); // vector is Range
        
        // Test that containers are not Stringable even with numeric elements
        static_assert(!Stringable<std::vector<int>>);
        static_assert(!Stringable<std::map<int, int>>);
        
        struct NonConceptType {};
        static_assert(!Stringable<NonConceptType>);
        static_assert(!NumericType<NonConceptType>);
        static_assert(!RangeLike<NonConceptType>);
    }
}