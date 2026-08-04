
#include <array>
#include <ostream>
#include <span>
#include <vector>

#include "doctest.h"
#include "include/span_kits.hpp"


using namespace zuc;

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
        std::vector<int> targets = {1, 2, 3, 4, 5, 6};
        auto found = find_subspan(main_span, std::span(targets));
        CHECK(found.empty());
    }

    TEST_CASE("ConcatSpan basic iteration") {
        std::array<int, 3> part1 = {1, 2, 3};
        std::vector<int> part2 = {4, 5, 6};
        std::array<int, 2> part3 = {7, 8};

        ConcatSpan<int, 3> concatenated(
            {std::span(part1), std::span(part2), std::span(part3)});

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

        ConcatSpan<int, 3> concatenated(
            {std::span(part1), std::span(part2), std::span(part3)});

        CHECK(concatenated[0] == 1);
        CHECK(concatenated[2] == 3);
        CHECK(concatenated[3] == 4);
        CHECK(concatenated[5] == 6);
        CHECK(concatenated[7] == 8);
    }

    TEST_CASE("ConcatSpan at method") {
        std::array<int, 3> part1 = {1, 2, 3};
        std::vector<int> part2 = {4, 5, 6};

        ConcatSpan<int, 2> concatenated({std::span(part1), std::span(part2)});

        CHECK(concatenated.at(0) == 1);
        CHECK(concatenated.at(5) == 6);
    }

    TEST_CASE("ConcatSpan get_data") {
        std::array<int, 2> part1 = {1, 2};
        std::vector<int> part2 = {3, 4};

        ConcatSpan<int, 2> concatenated({std::span(part1), std::span(part2)});

        auto data = concatenated.get_data();
        CHECK(data.size() == 2);
        CHECK(data[0].size() == 2);
        CHECK(data[1].size() == 2);
    }

    TEST_CASE("ConcatSpan empty spans") {
        std::vector<int> empty1, empty2;
        std::array<int, 3> part3 = {1, 2, 3};

        ConcatSpan<int, 3> concatenated(
            {std::span(empty1), std::span(empty2), std::span(part3)});

        CHECK(concatenated.size() == 3);
        CHECK(concatenated[0] == 1);
        CHECK(concatenated[2] == 3);
    }

    TEST_CASE("ConcatSpan iterator operations") {
        std::array<int, 2> part1 = {1, 2};
        std::array<int, 2> part2 = {3, 4};

        ConcatSpan<int, 2> concatenated({std::span(part1), std::span(part2)});

        auto it = concatenated.begin();
        CHECK(*it == 1);
        ++it;
        CHECK(*it == 2);

        --it;
        CHECK(*it == 1);

        auto it2 = it + 2;
        CHECK(*it2 == 3);

        auto it3 = concatenated.begin();
        CHECK(it == it3);
        CHECK(it2 != it3);
    }

    TEST_CASE("sub_span_safe edge cases") {
        std::vector<int> data = {1, 2, 3, 4, 5};

        auto result = sub_span_safe(std::span(data), 5, 0);
        CHECK(result.has_value());
        CHECK(result.value().empty());

        auto result2 = sub_span_safe(std::span(data), 2, 3);
        CHECK(result2.has_value());
        CHECK(result2.value().size() == 3);
        CHECK(result2.value()[0] == 3);
        CHECK(result2.value()[2] == 5);

        auto result3 = sub_span_safe(std::span(data), 3);
        CHECK(result3.has_value());
        CHECK(result3.value().size() == 2);
        CHECK(result3.value()[0] == 4);
        CHECK(result3.value()[1] == 5);
    }
}

/**
 * @test ChunkSpan Operations
 * Tests for chunked span operations with proper chunk count and size
 * calculations.
 */
TEST_SUITE("ChunkSpan Operations") {
    TEST_CASE("ChunkSpan exact multiple") {
        std::vector<int> data = {1, 2, 3, 4, 5, 6};
        ChunkSpan<std::vector<int>, 2> chunks(data);

        CHECK(chunks.size() == 6);  // 6 elements 

        auto it = chunks.begin();
        auto chunk0 = *it;
        CHECK(chunk0.size() == 2);
        CHECK(chunk0[0] == 1);
        CHECK(chunk0[1] == 2);

        ++it;
        auto chunk1 = *it;
        CHECK(chunk1.size() == 2);
        CHECK(chunk1[0] == 3);
        CHECK(chunk1[1] == 4);

        ++it;
        auto chunk2 = *it;
        CHECK(chunk2.size() == 2);
        CHECK(chunk2[0] == 5);
        CHECK(chunk2[1] == 6);
    }

    TEST_CASE("ChunkSpan with remainder") {
        std::vector<int> data = {1, 2, 3, 4, 5};  // 5 elements, chunk_size=2
        ChunkSpan<std::vector<int>, 2> chunks(data);

        CHECK(chunks.size() == 5);  // Should be 5 elements

        auto it = chunks.begin();
        auto chunk0 = *it;
        CHECK(chunk0.size() == 2);
        CHECK(chunk0[0] == 1);
        CHECK(chunk0[1] == 2);

        ++it;
        auto chunk1 = *it;
        CHECK(chunk1.size() == 2);
        CHECK(chunk1[0] == 3);
        CHECK(chunk1[1] == 4);

        ++it;
        auto chunk2 = *it;          // Last chunk with remainder
        CHECK(chunk2.size() == 1);  // Should be 1, not 2
        CHECK(chunk2[0] == 5);
    }

    TEST_CASE("ChunkSpan single element") {
        std::vector<int> data = {42};
        ChunkSpan<std::vector<int>, 3> chunks(data);

        CHECK(chunks.size() == 1);  // Should be 1 chunk

        auto it = chunks.begin();
        auto chunk0 = *it;
        CHECK(chunk0.size() == 1);  // Should be 1, not 3
        CHECK(chunk0[0] == 42);
    }

    TEST_CASE("ChunkSpan empty data") {
        std::vector<int> data = {};
        ChunkSpan<std::vector<int>, 2> chunks(data);

        CHECK(chunks.size() == 0);
    }

    TEST_CASE("ChunkSpan larger chunk size") {
        std::vector<int> data = {1, 2, 3};
        ChunkSpan<std::vector<int>, 5> chunks(data);

        CHECK(chunks.size() == 3);

        auto it = chunks.begin();
        auto chunk0 = *it;
        CHECK(chunk0.size() == 3);  // Should be 3, not 5
        CHECK(chunk0[0] == 1);
        CHECK(chunk0[1] == 2);
        CHECK(chunk0[2] == 3);
    }

    TEST_CASE("ChunkSpan iteration") {
        std::vector<int> data = {1, 2, 3, 4, 5, 6, 7};
        ChunkSpan<std::vector<int>, 3> chunks(data);

        CHECK(chunks.size() == 7);

        int total = 0;
        for (const auto& chunk : chunks) {
            for (int val : chunk) {
                total += val;
            }
        }
        CHECK(total == 28);  // 1+2+3+4+5+6+7 = 28
    }

    TEST_CASE("ChunkSpan operator[] returns chunk") {
        std::vector<int> data = {1, 2, 3, 4, 5};
        ChunkSpan<std::vector<int>, 2> chunks(data);
        
        // Test operator[] returns chunks
        auto chunk0 = chunks[0];
        CHECK(chunk0.size() == 2);
        CHECK(chunk0[0] == 1);
        CHECK(chunk0[1] == 2);
        
        auto chunk1 = chunks[1];
        CHECK(chunk1.size() == 2);
        CHECK(chunk1[0] == 3);
        CHECK(chunk1[1] == 4);
        
        auto chunk2 = chunks[2];
        CHECK(chunk2.size() == 1);
        CHECK(chunk2[0] == 5);
    }

    TEST_CASE("ChunkSpan element_at method") {
        std::vector<int> data = {1, 2, 3, 4, 5};
        ChunkSpan<std::vector<int>, 2> chunks(data);
        
        // Test element access with element_at() method
        CHECK(chunks.element_at(0) == 1);
        CHECK(chunks.element_at(1) == 2);
        CHECK(chunks.element_at(2) == 3);
        CHECK(chunks.element_at(3) == 4);
        CHECK(chunks.element_at(4) == 5);
        
        // Test out of range access
        CHECK_THROWS_AS(chunks.element_at(5), std::out_of_range);
        CHECK_THROWS_AS(chunks.element_at(10), std::out_of_range);
    }

    TEST_CASE("ChunkSpan chunk_at method") {
        std::vector<int> data = {1, 2, 3, 4, 5};
        ChunkSpan<std::vector<int>, 2> chunks(data);
        
        // Test chunk access with chunk_at() method
        auto chunk0 = chunks.chunk_at(0);
        CHECK(chunk0.size() == 2);
        CHECK(chunk0[0] == 1);
        CHECK(chunk0[1] == 2);
        
        auto chunk2 = chunks.chunk_at(2);
        CHECK(chunk2.size() == 1);
        CHECK(chunk2[0] == 5);
        
        // Test out of range access
        CHECK_THROWS_AS(chunks.chunk_at(3), std::out_of_range);
        CHECK_THROWS_AS(chunks.chunk_at(10), std::out_of_range);
    }

    TEST_CASE("ChunkSpan iterator dereference") {
        std::vector<int> data = {1, 2, 3, 4, 5};
        ChunkSpan<std::vector<int>, 2> chunks(data);

        auto it = chunks.begin();
        auto chunk0 = *it;  // Dereference iterator to get span
        CHECK(chunk0.size() == 2);
        CHECK(chunk0[0] == 1);
        CHECK(chunk0[1] == 2);

        ++it;
        auto chunk1 = *it;
        CHECK(chunk1.size() == 2);
        CHECK(chunk1[0] == 3);
        CHECK(chunk1[1] == 4);

        ++it;
        auto chunk2 = *it;
        CHECK(chunk2.size() == 1);
        CHECK(chunk2[0] == 5);
    }

    TEST_CASE("ChunkSpan const iteration") {
        std::vector<int> data = {1, 2, 3, 4, 5};
        ChunkSpan<std::vector<int>, 2> chunks(data);

        int count = 0;
        for (auto it = chunks.cbegin(); it != chunks.cend(); ++it) {
            count++;
        }
        CHECK(count == 3);
    }

    TEST_CASE("ChunkSpan get_data") {
        std::vector<int> data = {1, 2, 3, 4, 5};
        ChunkSpan<std::vector<int>, 2> chunks(data);

        auto original_data = chunks.get_data();
        CHECK(original_data.size() == 5);
        CHECK(original_data[0] == 1);
        CHECK(original_data[4] == 5);
    }
}