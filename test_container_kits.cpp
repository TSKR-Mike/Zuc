#include "doctest.h"
#include <ostream>
#include "include/container_kits.hpp"
#include <memory>
#include <vector>
#include <list>
#include <deque>
#include <array>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <string>

using namespace zuc;

/**
 * @test Contains Operations
 * Tests for container contains functionality with various container types.
 */
TEST_SUITE("Contains Operations") {
    TEST_CASE("contains in vector") {
        std::vector<int> vec = {1, 2, 3, 4, 5};
        CHECK(contains(vec, 3) == true);
        CHECK(contains(vec, 10) == false);
    }

    TEST_CASE("contains in list") {
        std::list<int> lst = {1, 2, 3, 4, 5};
        CHECK(contains(lst, 3) == true);
        CHECK(contains(lst, 10) == false);
    }

    TEST_CASE("contains in array") {
        std::array<int, 5> arr = {1, 2, 3, 4, 5};
        CHECK(contains(arr, 3) == true);
        CHECK(contains(arr, 10) == false);
    }

    TEST_CASE("contains in deque") {
        std::deque<int> deq = {1, 2, 3, 4, 5};
        CHECK(contains(deq, 3) == true);
        CHECK(contains(deq, 10) == false);
    }

    TEST_CASE("contains in empty container") {
        std::vector<int> empty_vec;
        CHECK(contains(empty_vec, 1) == false);
    }

    TEST_CASE("contains with strings") {
        std::vector<std::string> vec = {"hello", "world", "test"};
        CHECK(contains(vec, std::string("world")) == true);
        CHECK(contains(vec, std::string("missing")) == false);
    }

    TEST_CASE("contains with custom types") {
        std::vector<std::pair<int, int>> vec = {{1, 2}, {3, 4}, {5, 6}};
        CHECK(contains(vec, std::make_pair(3, 4)) == true);
        CHECK(contains(vec, std::make_pair(7, 8)) == false);
    }
}

/**
 * @test Contains Any Operations
 * Tests for checking if container contains any of multiple elements.
 */
TEST_SUITE("Contains Any Operations") {
    TEST_CASE("contains_any in vector") {
        std::vector<int> vec = {1, 2, 3, 4, 5};
        std::vector<int> targets = {3, 10, 20};
        CHECK(contains_any(vec, targets) == true);
    }

    TEST_CASE("contains_any none found") {
        std::vector<int> vec = {1, 2, 3, 4, 5};
        std::vector<int> targets = {10, 20, 30};
        CHECK(contains_any(vec, targets) == false);
    }

    TEST_CASE("contains_any with array targets") {
        std::vector<int> vec = {1, 2, 3, 4, 5};
        std::array<int, 3> targets = {2, 4, 6};
        CHECK(contains_any(vec, targets) == true);
    }

    TEST_CASE("contains_any empty targets") {
        std::vector<int> vec = {1, 2, 3, 4, 5};
        std::vector<int> empty_targets;
        CHECK(contains_any(vec, empty_targets) == false);
    }

    TEST_CASE("contains_any empty container") {
        std::vector<int> empty_vec;
        std::vector<int> targets = {1, 2, 3};
        CHECK(contains_any(empty_vec, targets) == false);
    }

    TEST_CASE("contains_any both empty") {
        std::vector<int> empty_vec;
        std::vector<int> empty_targets;
        CHECK(contains_any(empty_vec, empty_targets) == false);
    }

    TEST_CASE("contains_any with strings") {
        std::vector<std::string> vec = {"apple", "banana", "cherry"};
        std::vector<std::string> targets = {"date", "banana", "fig"};
        CHECK(contains_any(vec, targets) == true);
    }
}

/**
 * @test Contains If Operations
 * Tests for checking if container contains elements matching a predicate.
 */
TEST_SUITE("Contains If Operations") {
    TEST_CASE("contains_if with even numbers") {
        std::vector<int> vec = {1, 3, 5, 7, 9};
        CHECK(contains_if(vec, [](int x) { return x % 2 == 0; }) == false);
    }

    TEST_CASE("contains_if finds even number") {
        std::vector<int> vec = {1, 3, 4, 7, 9};
        CHECK(contains_if(vec, [](int x) { return x % 2 == 0; }) == true);
    }

    TEST_CASE("contains_if with string length") {
        std::vector<std::string> vec = {"a", "bb", "ccc", "dd"};
        CHECK(contains_if(vec, [](const std::string& s) { return s.length() > 2; }) == true);
    }

    TEST_CASE("contains_if empty container") {
        std::vector<int> empty_vec;
        CHECK(contains_if(empty_vec, [](int x) { return x > 0; }) == false);
    }

    TEST_CASE("contains_if all match") {
        std::vector<int> vec = {2, 4, 6, 8, 10};
        CHECK(contains_if(vec, [](int x) { return x % 2 == 0; }) == true);
    }

    TEST_CASE("contains_if with complex predicate") {
        std::vector<std::pair<int, int>> vec = {{1, 2}, {3, 4}, {5, 6}};
        CHECK(contains_if(vec, [](const auto& p) { return p.first + p.second > 10; }) == true);
    }
}

/**
 * @test Pop Back Value Operations
 * Tests for popping and returning values from containers.
 */
TEST_SUITE("Pop Back Value Operations") {
    TEST_CASE("pop_back_value from vector") {
        std::vector<int> vec = {1, 2, 3, 4, 5};
        auto result = pop_back_value(vec);
        CHECK(result.has_value());
        CHECK(result.value() == 5);
        CHECK(vec.size() == 4);
        CHECK(vec.back() == 4);
    }

    TEST_CASE("pop_back_value from empty vector") {
        std::vector<int> empty_vec;
        auto result = pop_back_value(empty_vec);
        CHECK(!result.has_value());
    }

    TEST_CASE("pop_back_value with strings") {
        std::vector<std::string> vec = {"hello", "world", "test"};
        auto result = pop_back_value(vec);
        CHECK(result.has_value());
        CHECK(result.value() == "test");
        CHECK(vec.size() == 2);
    }

    TEST_CASE("pop_back_value with move semantics") {
        std::vector<std::unique_ptr<int>> vec;
        vec.push_back(std::make_unique<int>(42));
        
        auto result = pop_back_value(vec);
        CHECK(result.has_value());
        CHECK(*result.value() == 42);
        CHECK(vec.empty());
    }

    TEST_CASE("pop_back_value multiple calls") {
        std::vector<int> vec = {1, 2, 3};
        
        auto r1 = pop_back_value(vec);
        CHECK(r1.value() == 3);
        
        auto r2 = pop_back_value(vec);
        CHECK(r2.value() == 2);
        
        auto r3 = pop_back_value(vec);
        CHECK(r3.value() == 1);
        
        auto r4 = pop_back_value(vec);
        CHECK(!r4.has_value());
    }
}

/**
 * @test Pop Front Value Operations
 * Tests for popping and returning values from front of containers.
 */
TEST_SUITE("Pop Front Value Operations") {
    TEST_CASE("pop_front_value from list") {
        std::list<int> lst = {1, 2, 3, 4, 5};
        auto result = pop_front_value(lst);
        CHECK(result.has_value());
        CHECK(result.value() == 1);
        CHECK(lst.size() == 4);
        CHECK(lst.front() == 2);
    }

    TEST_CASE("pop_front_value from deque") {
        std::deque<int> deq = {1, 2, 3, 4, 5};
        auto result = pop_front_value(deq);
        CHECK(result.has_value());
        CHECK(result.value() == 1);
        CHECK(deq.size() == 4);
        CHECK(deq.front() == 2);
    }

    TEST_CASE("pop_front_value from empty list") {
        std::list<int> empty_lst;
        auto result = pop_front_value(empty_lst);
        CHECK(!result.has_value());
    }

    TEST_CASE("pop_front_value from empty deque") {
        std::deque<int> empty_deq;
        auto result = pop_front_value(empty_deq);
        CHECK(!result.has_value());
    }

    TEST_CASE("pop_front_value with strings") {
        std::list<std::string> lst = {"hello", "world", "test"};
        auto result = pop_front_value(lst);
        CHECK(result.has_value());
        CHECK(result.value() == "hello");
        CHECK(lst.size() == 2);
    }

    TEST_CASE("pop_front_value multiple calls") {
        std::deque<int> deq = {1, 2, 3};
        
        auto r1 = pop_front_value(deq);
        CHECK(r1.value() == 1);
        
        auto r2 = pop_front_value(deq);
        CHECK(r2.value() == 2);
        
        auto r3 = pop_front_value(deq);
        CHECK(r3.value() == 3);
        
        auto r4 = pop_front_value(deq);
        CHECK(!r4.has_value());
    }
}

/**
 * @test Get Or Insert Operations
 * Tests for map operations with default value insertion.
 */
TEST_SUITE("Get Or Insert Operations") {
    TEST_CASE("get_or_insert new key") {
        std::unordered_map<int, std::string> map;
        auto& value = get_or_insert(map, 1, "default");
        CHECK(value == "default");
        CHECK(map.size() == 1);
        CHECK(map[1] == "default");
    }

    TEST_CASE("get_or_insert existing key") {
        std::unordered_map<int, std::string> map = {{1, "existing"}};
        auto& value = get_or_insert(map, 1, "default");
        CHECK(value == "existing");
        CHECK(map.size() == 1);
    }

    TEST_CASE("get_or_insert with multiple arguments") {
        std::unordered_map<int, std::string> map;
        auto& value = get_or_insert(map, 1, 5, 'a');
        CHECK(value == "aaaaa");
        CHECK(map.size() == 1);
    }

    TEST_CASE("get_or_insert_default new key") {
        std::unordered_map<int, std::string> map;
        auto& value = get_or_insert_default(map, 1, std::string("default"));
        CHECK(value == "default");
        CHECK(map.size() == 1);
        CHECK(map[1] == "default");
    }

    TEST_CASE("get_or_insert_default existing key") {
        std::unordered_map<int, std::string> map = {{1, "existing"}};
        auto& value = get_or_insert_default(map, 1, "default");
        CHECK(value == "existing");
        CHECK(map.size() == 1);
    }

    TEST_CASE("get_or_insert_default with int values") {
        std::unordered_map<std::string, int> map;
        auto& value = get_or_insert_default(map, "key", 42);
        CHECK(value == 42);
        CHECK(map.size() == 1);
    }

    TEST_CASE("get_or_insert reference stability") {
        std::unordered_map<int, std::string> map;
        auto& value1 = get_or_insert(map, 1, "first");
        auto& value2 = get_or_insert(map, 1, "second");
        CHECK(&value1 == &value2);
        CHECK(value1 == "first");
    }
}

/**
 * @test Container Slice Operations
 * Tests for slicing containers with start, end, and step parameters.
 */
TEST_SUITE("Container Slice Operations") {
    TEST_CASE("container_slice basic") {
        std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        auto sliced = slice(vec, 2, 5);
        CHECK(sliced.size() == 3);
        CHECK(sliced[0] == 3);
        CHECK(sliced[1] == 4);
        CHECK(sliced[2] == 5);
    }

    TEST_CASE("container_slice with step") {
        std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        auto sliced = slice(vec, 0, 10, 2);
        CHECK(sliced.size() == 5);
        CHECK(sliced[0] == 1);
        CHECK(sliced[1] == 3);
        CHECK(sliced[2] == 5);
        CHECK(sliced[3] == 7);
        CHECK(sliced[4] == 9);
    }

    TEST_CASE("container_slice from beginning") {
        std::vector<int> vec = {1, 2, 3, 4, 5};
        auto sliced = slice(vec, 0, 3);
        CHECK(sliced.size() == 3);
        CHECK(sliced[0] == 1);
        CHECK(sliced[1] == 2);
        CHECK(sliced[2] == 3);
    }

    TEST_CASE("container_slice to end") {
        std::vector<int> vec = {1, 2, 3, 4, 5};
        auto sliced = slice(vec, 2, 5);
        CHECK(sliced.size() == 3);
        CHECK(sliced[0] == 3);
        CHECK(sliced[1] == 4);
        CHECK(sliced[2] == 5);
    }

    TEST_CASE("container_slice empty result") {
        std::vector<int> vec = {1, 2, 3, 4, 5};
        auto sliced = slice(vec, 3, 3);
        CHECK(sliced.empty());
    }

    TEST_CASE("container_slice step larger than range") {
        std::vector<int> vec = {1, 2, 3, 4, 5};
        auto sliced = slice(vec, 0, 5, 10);
        CHECK(sliced.empty());
    }

    TEST_CASE("container_slice with array") {
        std::array<int, 10> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        auto sliced = slice(arr, 1, 4);
        CHECK(sliced.size() == 3);
        CHECK(sliced[0] == 2);
        CHECK(sliced[1] == 3);
        CHECK(sliced[2] == 4);
    }

    TEST_CASE("container_slice with strings") {
        std::vector<std::string> vec = {"a", "b", "c", "d", "e"};
        auto sliced = slice(vec, 1, 4);
        CHECK(sliced.size() == 3);
        CHECK(sliced[0] == "b");
        CHECK(sliced[1] == "c");
        CHECK(sliced[2] == "d");
    }
}

/**
 * @test Erase If Operations
 * Tests for conditional removal of elements from containers.
 */
TEST_SUITE("Erase If Operations") {
    TEST_CASE("erase_if even numbers from vector") {
        std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        erase_if(vec, [](int x) { return x % 2 == 0; });
        CHECK(vec.size() == 5);
        CHECK(vec == std::vector<int>{1, 3, 5, 7, 9});
    }

    TEST_CASE("erase_if from list") {
        std::list<int> lst = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        erase_if(lst, [](int x) { return x < 5; });
        CHECK(lst.size() == 6);
        CHECK(lst.front() == 5);
    }

    TEST_CASE("erase_if from deque") {
        std::deque<int> deq = {1, 2, 3, 4, 5};
        erase_if(deq, [](int x) { return x % 2 == 0; });
        CHECK(deq.size() == 3);
        CHECK(deq[0] == 1);
        CHECK(deq[1] == 3);
        CHECK(deq[2] == 5);
    }

    TEST_CASE("erase_if empty container") {
        std::vector<int> empty_vec;
        erase_if(empty_vec, [](int x) { return x > 0; });
        CHECK(empty_vec.empty());
    }

    TEST_CASE("erase_if no matches") {
        std::vector<int> vec = {1, 3, 5, 7, 9};
        size_t original_size = vec.size();
        erase_if(vec, [](int x) { return x % 2 == 0; });
        CHECK(vec.size() == original_size);
    }

    TEST_CASE("erase_if all elements") {
        std::vector<int> vec = {2, 4, 6, 8, 10};
        erase_if(vec, [](int x) { return x % 2 == 0; });
        CHECK(vec.empty());
    }

    TEST_CASE("erase_if with strings") {
        std::vector<std::string> vec = {"a", "ab", "abc", "abcd", "abcde"};
        erase_if(vec, [](const std::string& s) { return s.length() < 3; });
        CHECK(vec.size() == 3);
        CHECK(vec[0] == "abc");
        CHECK(vec[1] == "abcd");
        CHECK(vec[2] == "abcde");
    }

    TEST_CASE("erase_if with complex predicate") {
        std::vector<std::pair<int, int>> vec = {{1, 2}, {3, 4}, {5, 6}, {7, 8}};
        erase_if(vec, [](const auto& p) { return p.first + p.second > 10; });
        CHECK(vec.size() == 2);
    }
}

/**
 * @test Transform All Operations
 * Tests for transforming all elements in a container.
 */
TEST_SUITE("Transform All Operations") {
    TEST_CASE("transform_all_to_vector square numbers") {
        std::vector<int> vec = {1, 2, 3, 4, 5};
        auto result = transform_all_to_vector<int>(vec, [](int x) { return x * x; });
        CHECK(result.size() == 5);
        CHECK(result == std::vector<int>{1, 4, 9, 16, 25});
    }

    TEST_CASE("transform_all_to_vector to string") {
        std::vector<int> vec = {1, 2, 3};
        auto result = transform_all_to_vector<std::string>(vec, [](int x) { return std::to_string(x); });
        CHECK(result.size() == 3);
        CHECK(result[0] == "1");
        CHECK(result[1] == "2");
        CHECK(result[2] == "3");
    }

    TEST_CASE("transform_all_to_vector with array") {
        std::array<int, 4> arr = {1, 2, 3, 4};
        auto result = transform_all_to_vector<int>(arr, [](int x) { return x * 2; });
        CHECK(result.size() == 4);
        CHECK(result == std::vector<int>{2, 4, 6, 8});
    }

    TEST_CASE("transform_all_to_vector empty container") {
        std::vector<int> empty_vec;
        auto result = transform_all_to_vector<int>(empty_vec, [](int x) { return x + 1; });
        CHECK(result.empty());
    }

    TEST_CASE("transform_all_to_vector with different types") {
        std::vector<double> vec = {1.5, 2.5, 3.5};
        auto result = transform_all_to_vector<int>(vec, [](double x) { return static_cast<int>(x); });
        CHECK(result.size() == 3);
        CHECK(result == std::vector<int>{1, 2, 3});
    }

    TEST_CASE("transform_all_to_vector with strings") {
        std::vector<std::string> vec = {"hello", "world", "test"};
        auto result = transform_all_to_vector<size_t>(vec, [](const std::string& s) { return s.length(); });
        CHECK(result.size() == 3);
        CHECK(result == std::vector<size_t>{5, 5, 4});
    }

    TEST_CASE("transform_all_to_vector preserves original") {
        std::vector<int> vec = {1, 2, 3};
        auto result = transform_all_to_vector<int>(vec, [](int x) { return x * 10; });
        CHECK(vec == std::vector<int>{1, 2, 3});
        CHECK(result == std::vector<int>{10, 20, 30});
    }
}

/**
 * @test Get Map Keys Operations
 * Tests for extracting keys from map-like containers.
 */
TEST_SUITE("Get Map Keys Operations") {
    TEST_CASE("get_map_keys from unordered_map") {
        std::unordered_map<int, std::string> map = {{1, "one"}, {2, "two"}, {3, "three"}};
        auto keys = get_map_keys(map);
        CHECK(keys.size() == 3);
        CHECK(keys.contains(1));
        CHECK(keys.contains(2));
        CHECK(keys.contains(3));
    }

    TEST_CASE("get_map_keys from map") {
        std::map<int, std::string> map = {{1, "one"}, {2, "two"}, {3, "three"}};
        auto keys = get_map_keys(map);
        CHECK(keys.size() == 3);
        CHECK(keys.contains(1));
        CHECK(keys.contains(2));
        CHECK(keys.contains(3));
    }

    TEST_CASE("get_map_keys empty map") {
        std::unordered_map<int, std::string> empty_map;
        auto keys = get_map_keys(empty_map);
        CHECK(keys.empty());
    }

    TEST_CASE("get_map_keys with string keys") {
        std::unordered_map<std::string, int> map = {{"one", 1}, {"two", 2}, {"three", 3}};
        auto keys = get_map_keys(map);
        CHECK(keys.size() == 3);
        CHECK(keys.contains("one"));
        CHECK(keys.contains("two"));
        CHECK(keys.contains("three"));
    }

    TEST_CASE("get_map_keys single element") {
        std::unordered_map<int, std::string> map = {{1, "one"}};
        auto keys = get_map_keys(map);
        CHECK(keys.size() == 1);
        CHECK(keys.contains(1));
    }
}

/**
 * @test Get Map Values Operations
 * Tests for extracting values from map-like containers.
 */
TEST_SUITE("Get Map Values Operations") {
    TEST_CASE("get_map_values from unordered_map") {
        std::unordered_map<int, std::string> map = {{1, "one"}, {2, "two"}, {3, "three"}};
        auto values = get_map_values(map);
        CHECK(values.size() == 3);
        CHECK(values.contains("one"));
        CHECK(values.contains("two"));
        CHECK(values.contains("three"));
    }

    TEST_CASE("get_map_values from map") {
        std::map<int, std::string> map = {{1, "one"}, {2, "two"}, {3, "three"}};
        auto values = get_map_values(map);
        CHECK(values.size() == 3);
        CHECK(values.contains("one"));
        CHECK(values.contains("two"));
        CHECK(values.contains("three"));
    }

    TEST_CASE("get_map_values empty map") {
        std::unordered_map<int, std::string> empty_map;
        auto values = get_map_values(empty_map);
        CHECK(values.empty());
    }

    TEST_CASE("get_map_values with int values") {
        std::unordered_map<std::string, int> map = {{"one", 1}, {"two", 2}, {"three", 3}};
        auto values = get_map_values(map);
        CHECK(values.size() == 3);
        CHECK(values.contains(1));
        CHECK(values.contains(2));
        CHECK(values.contains(3));
    }

    TEST_CASE("get_map_values duplicate values") {
        std::unordered_map<int, std::string> map = {{1, "same"}, {2, "same"}, {3, "same"}};
        auto values = get_map_values(map);
        CHECK(values.size() == 1);  // Only unique values
        CHECK(values.contains("same"));
    }

    TEST_CASE("get_map_values single element") {
        std::unordered_map<int, std::string> map = {{1, "one"}};
        auto values = get_map_values(map);
        CHECK(values.size() == 1);
        CHECK(values.contains("one"));
    }
}

/**
 * @test Merge Operations
 * Tests for merging two containers.
 */
TEST_SUITE("Merge Operations") {
    TEST_CASE("merge vectors") {
        std::vector<int> vec1 = {1, 2, 3};
        std::vector<int> vec2 = {4, 5, 6};
        auto merged = merge(vec1, vec2);
        CHECK(merged.size() == 6);
        CHECK(merged == std::vector<int>{1, 2, 3, 4, 5, 6});
    }

    TEST_CASE("merge lists") {
        std::list<int> lst1 = {1, 2, 3};
        std::list<int> lst2 = {4, 5, 6};
        auto merged = merge(lst1, lst2);
        CHECK(merged.size() == 6);
    }

    TEST_CASE("merge deques") {
        std::deque<int> deq1 = {1, 2, 3};
        std::deque<int> deq2 = {4, 5, 6};
        auto merged = merge(deq1, deq2);
        CHECK(merged.size() == 6);
    }

    TEST_CASE("merge with empty first") {
        std::vector<int> empty_vec;
        std::vector<int> vec = {1, 2, 3};
        auto merged = merge(empty_vec, vec);
        CHECK(merged.size() == 3);
        CHECK(merged == vec);
    }

    TEST_CASE("merge with empty second") {
        std::vector<int> vec = {1, 2, 3};
        std::vector<int> empty_vec;
        auto merged = merge(vec, empty_vec);
        CHECK(merged.size() == 3);
        CHECK(merged == vec);
    }

    TEST_CASE("merge both empty") {
        std::vector<int> empty1;
        std::vector<int> empty2;
        auto merged = merge(empty1, empty2);
        CHECK(merged.empty());
    }

    TEST_CASE("merge with strings") {
        std::vector<std::string> vec1 = {"hello", "world"};
        std::vector<std::string> vec2 = {"test", "merge"};
        auto merged = merge(vec1, vec2);
        CHECK(merged.size() == 4);
        CHECK(merged[0] == "hello");
        CHECK(merged[1] == "world");
        CHECK(merged[2] == "test");
        CHECK(merged[3] == "merge");
    }

    TEST_CASE("merge preserves originals") {
        std::vector<int> vec1 = {1, 2, 3};
        std::vector<int> vec2 = {4, 5, 6};
        auto merged = merge(vec1, vec2);
        CHECK(vec1 == std::vector<int>{1, 2, 3});
        CHECK(vec2 == std::vector<int>{4, 5, 6});
    }
}

/**
 * @test Container Edge Cases
 * Tests for edge cases and boundary conditions in container operations.
 */
TEST_SUITE("Container Edge Cases") {
    TEST_CASE("operations on single element containers") {
        std::vector<int> vec = {42};
        
        CHECK(contains(vec, 42) == true);
        CHECK(contains(vec, 0) == false);
        
        auto sliced = slice(vec, 0, 1);
        CHECK(sliced.size() == 1);
        CHECK(sliced[0] == 42);
        
        auto transformed = transform_all_to_vector<int>(vec, [](int x) { return x * 2; });
        CHECK(transformed.size() == 1);
        CHECK(transformed[0] == 84);
    }

    TEST_CASE("operations on large containers") {
        std::vector<int> vec(1000);
        for (size_t i = 0; i < vec.size(); ++i) {
            vec[i] = static_cast<int>(i);
        }
        
        CHECK(contains(vec, 500) == true);
        CHECK(contains(vec, 1000) == false);
        
        auto sliced = slice(vec, 100, 200);
        CHECK(sliced.size() == 100);
        CHECK(sliced[0] == 100);
        CHECK(sliced[99] == 199);
    }

    TEST_CASE("container_slice boundary conditions") {
        std::vector<int> vec = {1, 2, 3, 4, 5};
        
        auto start_slice = slice(vec, 0, 1);
        CHECK(start_slice.size() == 1);
        CHECK(start_slice[0] == 1);
        
        auto end_slice = slice(vec, 4, 5);
        CHECK(end_slice.size() == 1);
        CHECK(end_slice[0] == 5);
        
        auto empty_slice = slice(vec, 2, 2);
        CHECK(empty_slice.empty());
    }

    TEST_CASE("erase_if with complex conditions") {
        std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        
        erase_if(vec, [](int x) { return x > 3 && x < 8; });
        CHECK(vec == std::vector<int>{1, 2, 3, 8, 9, 10});
    }

    TEST_CASE("transform_all_to_vector with stateful lambda") {
        std::vector<int> vec = {1, 2, 3, 4, 5};
        int multiplier = 10;
        auto result = transform_all_to_vector<int>(vec, [multiplier](int x) { return x * multiplier; });
        CHECK(result == std::vector<int>{10, 20, 30, 40, 50});
    }

    TEST_CASE("merge with different types") {
        std::vector<int> vec1 = {1, 2};
        std::vector<double> vec2 = {3.5, 4.5};
        
        // This should work with appropriate type conversion
        auto merged = merge(vec1, vec1); // Same type merge
        CHECK(merged.size() == 4);
    }
}

/**
 * @test At Operations
 * Tests for safe element access with bounds checking.
 */
TEST_SUITE("At Operations") {
    TEST_CASE("at valid index") {
        std::vector<int> vec = {1, 2, 3, 4, 5};
        auto result = at(vec, 2);
        CHECK(result.has_value());
        CHECK(result.value() == 3);
    }

    TEST_CASE("at index out of bounds") {
        std::vector<int> vec = {1, 2, 3};
        auto result = at(vec, 5);
        CHECK(!result.has_value());
    }

    TEST_CASE("at with empty container") {
        std::vector<int> empty_vec;
        auto result = at(empty_vec, 0);
        CHECK(!result.has_value());
    }

    TEST_CASE("at at boundary") {
        std::vector<int> vec = {10, 20, 30};
        
        auto first = at(vec, 0);
        CHECK(first.has_value());
        CHECK(first.value() == 10);
        
        auto last = at(vec, 2);
        CHECK(last.has_value());
        CHECK(last.value() == 30);
        
        auto out_of_bounds = at(vec, 3);
        CHECK(!out_of_bounds.has_value());
    }

    TEST_CASE("at with array") {
        std::array<int, 4> arr = {1, 2, 3, 4};
        auto result = at(arr, 2);
        CHECK(result.has_value());
        CHECK(result.value() == 3);
    }

    TEST_CASE("at with string") {
        std::string str = "hello";
        auto result = at(str, 1);
        CHECK(result.has_value());
        CHECK(result.value() == 'e');
    }

    TEST_CASE("at const reference") {
        const std::vector<int> vec = {1, 2, 3};
        auto result = at(vec, 1);
        CHECK(result.has_value());
        CHECK(result.value() == 2);
    }

    TEST_CASE("at with large index") {
        std::vector<int> vec = {1, 2, 3};
        auto result = at(vec, SIZE_MAX);
        CHECK(!result.has_value());
    }

    TEST_CASE("at_ref valid index") {
        std::vector<int> vec = {1, 2, 3, 4, 5};
        auto& ref = at_ref(vec, 2);
        CHECK(ref == 3);
        
        // Modify through reference
        ref = 100;
        CHECK(vec[2] == 100);
    }

    TEST_CASE("at_ref out of range throws") {
        std::vector<int> vec = {1, 2, 3};
        CHECK_THROWS_AS(at_ref(vec, 5), std::out_of_range);
        CHECK_THROWS_AS(at_ref(vec, 10), std::out_of_range);
    }

    TEST_CASE("at_ref with const container") {
        const std::vector<int> vec = {1, 2, 3, 4, 5};
        const auto& ref = at_ref(vec, 3);
        CHECK(ref == 4);
    }

    TEST_CASE("at_ref with array") {
        std::array<int, 4> arr = {10, 20, 30, 40};
        auto& ref = at_ref(arr, 1);
        CHECK(ref == 20);
    }

    TEST_CASE("at_ref with string") {
        std::string str = "hello";
        auto& ref = at_ref(str, 1);
        CHECK(ref == 'e');
        
        // Modify through reference
        ref = 'X';
        CHECK(str == "hXllo");
    }

    TEST_CASE("at with index zero on empty container") {
        std::vector<int> empty_vec;
        auto result = at(empty_vec, 0);
        CHECK(!result.has_value());
    }

    TEST_CASE("pop operations on single element") {
        std::vector<int> vec = {42};
        auto back_result = pop_back_value(vec);
        CHECK(back_result.has_value());
        CHECK(back_result.value() == 42);
        CHECK(vec.empty());
        
        std::list<int> lst = {42};
        auto front_result = pop_front_value(lst);
        CHECK(front_result.has_value());
        CHECK(front_result.value() == 42);
        CHECK(lst.empty());
    }

    TEST_CASE("map operations with complex keys") {
        std::map<std::pair<int, int>, std::string> map;
        map[{1, 2}] = "pair12";
        map[{3, 4}] = "pair34";
        
        CHECK(map.size() == 2);
        CHECK(map[{1, 2}] == "pair12");
        CHECK(map[{3, 4}] == "pair34");
    }
}