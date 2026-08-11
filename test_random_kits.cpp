// tests/test_random_kits.cpp
#include <algorithm>
#include <ostream>
#include <vector>

#include "doctest.h"
#include "include/io_kits.hpp"
#include "include/random_kits.hpp"
#include "include/time_kits.hpp"


using namespace zuc;

/**
 * @brief Test function for random number generation benchmarking
 */
void get_random_int_and_print() {
    int i = random_int(-100, 100);
    write_string_to_console("{}\n", i);
}

/**
 * @test Random Number Generation
 * Comprehensive tests for random number generators including various types and
 * ranges.
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

    TEST_CASE("random_int basic range check") {
        CHECK(random_int(0, 100) >= 0);
        CHECK(random_int(0, 100) <= 100);
    }

    TEST_CASE("random_int edge cases") {
        // Test with same min and max
        int value = random_int(5, 5);
        CHECK(value == 5);

        // Test negative range
        int neg_value = random_int(-10, -1);
        CHECK(neg_value >= -10);
        CHECK(neg_value <= -1);

        // Test crossing zero
        int cross_value = random_int(-5, 5);
        CHECK(cross_value >= -5);
        CHECK(cross_value <= 5);
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

    TEST_CASE("random_double negative range") {
        double min = -1.0;
        double max = 1.0;

        for (int i = 0; i < 100; ++i) {
            double value = random_double(min, max);
            CHECK(value >= min);
            CHECK(value < max);
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

    TEST_CASE("random_long_long large range") {
        long long min = -1000000LL;
        long long max = 1000000LL;

        for (int i = 0; i < 50; ++i) {
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

    TEST_CASE("random_int32 edge cases") {
        // Test near int32 limits
        int32_t min = std::numeric_limits<int32_t>::max() - 100;
        int32_t max = std::numeric_limits<int32_t>::max();

        for (int i = 0; i < 10; ++i) {
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

    TEST_CASE("random_int64 large range") {
        int64_t min = -1000000000LL;
        int64_t max = 1000000000LL;

        for (int i = 0; i < 50; ++i) {
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

    TEST_CASE("uniform_random_long_long alias") {
        long long min = 1000LL;
        long long max = 5000LL;

        for (int i = 0; i < 50; ++i) {
            long long value = uniform_random_long_long(min, max);
            CHECK(value >= min);
            CHECK(value <= max);
        }
    }

    TEST_CASE("uniform_random_int32 alias") {
        int32_t min = -50;
        int32_t max = 50;

        for (int i = 0; i < 50; ++i) {
            int32_t value = uniform_random_int32(min, max);
            CHECK(value >= min);
            CHECK(value <= max);
        }
    }

    TEST_CASE("uniform_random_int64 alias") {
        int64_t min = 100000LL;
        int64_t max = 999999LL;

        for (int i = 0; i < 50; ++i) {
            int64_t value = uniform_random_int64(min, max);
            CHECK(value >= min);
            CHECK(value <= max);
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

    TEST_CASE("random distribution coverage") {
        // Test that we can generate different values across the range
        std::vector<int> values(100);

        for (auto& val : values) {
            val = random_int(0, 9);
        }

        // Check that we got at least some variety (not all the same value)
        bool has_variety = false;
        for (size_t i = 1; i < values.size(); ++i) {
            if (values[i] != values[0]) {
                has_variety = true;
                break;
            }
        }
        CHECK(has_variety);
    }

    TEST_CASE("random double precision") {
        // Test that we get different decimal values
        std::vector<double> values(100);
        for (auto& val : values) {
            val = random_double(0.0, 1.0);
        }

        // Check for variety in the values
        bool has_variety = false;
        for (size_t i = 1; i < values.size(); ++i) {
            if (std::abs(values[i] - values[0]) > 0.01) {
                has_variety = true;
                break;
            }
        }
        CHECK(has_variety);
    }

    TEST_CASE("random performance benchmark") {
        // Test that random generation is reasonably fast
        Timer timer;
        timer.start();

        const int iterations = 10000;
        for (int i = 0; i < iterations; ++i) {
            volatile int value = random_int(0, 100);
            (void)value;  // Prevent optimization
        }

        auto duration = timer.get_duration_seconds();
        REQUIRE(duration.has_value());

        // Should complete 10,000 iterations in less than 1 second
        CHECK(duration.value().count() < 1.0);
    }

    TEST_CASE("random_kits basic test") {
        CHECK(random_int(0, 100) >= 0);
        CHECK(random_int(0, 100) <= 100);
    }

    TEST_CASE("random_kits with timing") {
        // This test uses the timing functionality
        TimingSetting setting;
        setting.times_to_run = 3;
        setting.print_after_each_run = false;
        setting.print_max_time = false;
        setting.print_min_time = false;
        setting.print_average_time = false;

        CHECK_NOTHROW(
            time_a_function("random int", setting, get_random_int_and_print));
    }

    TEST_CASE("random reproducibility check") {
        // This test checks that the generator is working
        // (not testing for exact reproducibility as that would require seed
        // control)
        std::vector<int> batch1(10);
        std::vector<int> batch2(10);

        for (int i = 0; i < 10; ++i) {
            batch1[i] = random_int(0, 1000);
        }

        for (int i = 0; i < 10; ++i) {
            batch2[i] = random_int(0, 1000);
        }

        // The two batches should be different (with very high probability)
        bool batches_different = false;
        for (int i = 0; i < 10; ++i) {
            if (batch1[i] != batch2[i]) {
                batches_different = true;
                break;
            }
        }
        CHECK(batches_different);
    }
}

/**
 * @test Container Shuffling
 * Tests for shuffle functions including default engine, custom engine, and
 * shuffled copy.
 */
TEST_SUITE("Container Shuffling") {
    TEST_CASE("shuffle with default engine") {
        std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        auto original = vec;

        shuffle(vec);

        // Check that size is preserved
        CHECK(vec.size() == original.size());

        // Check that elements are the same (just reordered)
        std::sort(vec.begin(), vec.end());
        std::sort(original.begin(), original.end());
        CHECK(vec == original);
    }

    TEST_CASE("shuffle with custom engine") {
        std::vector<int> vec = {1, 2, 3, 4, 5};
        auto original = vec;

        std::mt19937 custom_engine(42);  // Fixed seed for reproducibility
        shuffle(vec, custom_engine);

        // Check that size is preserved
        CHECK(vec.size() == original.size());

        // Check that elements are the same (just reordered)
        std::sort(vec.begin(), vec.end());
        std::sort(original.begin(), original.end());
        CHECK(vec == original);
    }

    TEST_CASE("shuffle with custom engine reproducibility") {
        std::vector<int> vec1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        std::vector<int> vec2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

        std::mt19937 engine1(42);
        std::mt19937 engine2(42);

        shuffle(vec1, engine1);
        shuffle(vec2, engine2);

        // Same seed should produce same shuffle
        CHECK(vec1 == vec2);
    }

    TEST_CASE("shuffled returns copy") {
        std::vector<int> original = {1, 2, 3, 4, 5};
        auto copy = original;

        auto shuffled_vec = shuffled(original);

        // Original should be unchanged
        CHECK(original == copy);

        // Shuffled version should have same elements
        std::sort(shuffled_vec.begin(), shuffled_vec.end());
        std::sort(original.begin(), original.end());
        CHECK(shuffled_vec == original);
    }

    TEST_CASE("shuffle different containers") {
        // Test with std::vector
        std::vector<int> vec = {1, 2, 3, 4, 5};
        auto vec_original = vec;
        shuffle(vec);
        CHECK(vec.size() == vec_original.size());

        // Test with std::deque
        std::deque<int> deq = {1, 2, 3, 4, 5};
        auto deq_original = deq;
        shuffle(deq);
        CHECK(deq.size() == deq_original.size());

        // Test with std::array
        std::array<int, 5> arr = {1, 2, 3, 4, 5};
        auto arr_original = arr;
        shuffle(arr);
        CHECK(arr.size() == arr_original.size());
    }

    TEST_CASE("shuffle empty container") {
        std::vector<int> empty_vec;
        CHECK_NOTHROW(shuffle(empty_vec));
        CHECK(empty_vec.empty());
    }

    TEST_CASE("shuffle single element") {
        std::vector<int> single = {42};
        auto original = single;
        shuffle(single);
        CHECK(single == original);
    }

    TEST_CASE("shuffle two elements") {
        std::vector<int> two = {1, 2};
        auto original = two;

        shuffle(two);

        // Should still contain the same elements
        CHECK(two.size() == 2);
        CHECK(((two[0] == 1 && two[1] == 2) || (two[0] == 2 && two[1] == 1)));
    }

    TEST_CASE("shuffle large container") {
        std::vector<int> large(1000);
        for (size_t i = 0; i < large.size(); ++i) {
            large[i] = static_cast<int>(i);
        }
        auto original = large;

        shuffle(large);

        // Check size preserved
        CHECK(large.size() == original.size());

        // Check elements preserved
        std::sort(large.begin(), large.end());
        CHECK(large == original);
    }

    TEST_CASE("shuffle with strings") {
        std::vector<std::string> words = {"hello", "world", "test", "shuffle",
                                          "random"};
        auto original = words;

        shuffle(words);

        // Check size preserved
        CHECK(words.size() == original.size());

        // Check elements preserved
        std::sort(words.begin(), words.end());
        std::sort(original.begin(), original.end());
        CHECK(words == original);
    }

    TEST_CASE("shuffled with different types") {
        // Test with int
        std::vector<int> int_vec = {1, 2, 3};
        auto int_shuffled = shuffled(int_vec);
        CHECK(int_shuffled.size() == 3);
        CHECK(int_vec == std::vector<int>{1, 2, 3});  // Original unchanged

        // Test with double
        std::vector<double> double_vec = {1.1, 2.2, 3.3};
        auto double_shuffled = shuffled(double_vec);
        CHECK(double_shuffled.size() == 3);
        CHECK(double_vec ==
              std::vector<double>{1.1, 2.2, 3.3});  // Original unchanged

        // Test with string
        std::vector<std::string> string_vec = {"a", "b", "c"};
        auto string_shuffled = shuffled(string_vec);
        CHECK(string_shuffled.size() == 3);
        CHECK(string_vec ==
              std::vector<std::string>{"a", "b", "c"});  // Original unchanged
    }

    TEST_CASE("shuffle changes order") {
        // Test that shuffling actually changes the order (non-deterministic but
        // should change)
        std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

        // Try multiple times - at least once should change the order
        bool order_changed = false;
        for (int i = 0; i < 10; ++i) {
            std::vector<int> test_vec = vec;
            shuffle(test_vec);
            if (test_vec != vec) {
                order_changed = true;
                break;
            }
        }
        CHECK(order_changed);
    }
}

/**
 * @test Boolean Random Generation
 * Tests for random_true() and random_false() functions including probability
 * validation and edge cases.
 */
TEST_SUITE("Boolean Random Generation") {
    TEST_CASE("random_true with zero probability") {
        // With 0.0 probability, should always return false
        for (int i = 0; i < 100; ++i) {
            CHECK(random_true(0.0) == false);
        }
    }

    TEST_CASE("random_true with one probability") {
        // With 1.0 probability, should always return true
        for (int i = 0; i < 100; ++i) {
            CHECK(random_true(1.0) == true);
        }
    }

    TEST_CASE("random_true with boundary probabilities") {
        // Test edge cases where results are deterministic
        CHECK(random_true(0.0) == false);   // 0% probability
        CHECK(random_true(1.0) == true);    // 100% probability
        CHECK(random_true(-0.5) == false);  // Negative probability treated as 0
        CHECK(random_true(1.5) == true);    // Probability > 1 treated as 1

        // Test that function returns boolean type
        static_assert(std::is_same_v<decltype(random_true(0.5)), bool>);
    }



    TEST_CASE("random_false is inverse of random_true") {
        // Test that random_false uses the same probability logic as
        // !random_true Since both functions generate random numbers
        // independently, we test that they use the same probability threshold
        // logic

        // Test edge cases where results are deterministic
        CHECK(random_false(0.0) == true);   // !random_true(0.0) = !false = true
        CHECK(random_false(1.0) == false);  // !random_true(1.0) = !true = false
        CHECK(random_false(-0.5) ==
              true);  // !random_true(-0.5) = !false = true
        CHECK(random_false(1.5) == false);  // !random_true(1.5) = !true = false

        // Test that both functions use the same probability threshold
        // by checking statistical properties over many trials
        const int trials = 10000;
        int true_count_07 = 0;
        int false_count_07 = 0;

        for (int i = 0; i < trials; ++i) {
            if (random_true(0.7)) true_count_07++;
            if (random_false(0.7)) false_count_07++;
        }

        // random_true(0.7) should give ~70% true
        // random_false(0.7) should give ~30% true (which is 70% false)
        double true_ratio = static_cast<double>(true_count_07) / trials;
        double false_ratio = static_cast<double>(false_count_07) / trials;

        // Both should be close to expected values with 10% tolerance
        CHECK(std::abs(true_ratio - 0.7) < 0.1);
        CHECK(std::abs(false_ratio - 0.3) < 0.1);

        // Verify that true_ratio + false_ratio ≈ 1.0
        CHECK(std::abs((true_ratio + false_ratio) - 1.0) < 0.1);
    }

    TEST_CASE("random_false with zero probability") {
        // With 0.0 probability, should always return true (inverse of
        // random_true)
        for (int i = 0; i < 100; ++i) {
            CHECK(random_false(0.0) == true);
        }
    }

    TEST_CASE("random_false with one probability") {
        // With 1.0 probability, should always return false (inverse of
        // random_true)
        for (int i = 0; i < 100; ++i) {
            CHECK(random_false(1.0) == false);
        }
    }

    TEST_CASE("random_true with negative probability") {
        // With negative probability, should always return false
        for (int i = 0; i < 100; ++i) {
            CHECK(random_true(-0.5) == false);
        }
    }

    TEST_CASE("random_true with probability greater than 1") {
        // With probability > 1.0, should always return true
        for (int i = 0; i < 100; ++i) {
            CHECK(random_true(1.5) == true);
        }
    }

    TEST_CASE("random_false with negative probability") {
        // With negative probability, should always return true (inverse of
        // random_true)
        for (int i = 0; i < 100; ++i) {
            CHECK(random_false(-0.5) == true);
        }
    }

    TEST_CASE("random_false with probability greater than 1") {
        // With probability > 1.0, should always return false (inverse of
        // random_true)
        for (int i = 0; i < 100; ++i) {
            CHECK(random_false(1.5) == false);
        }
    }
}

TEST_CASE("random_true performance") {
    // Test that random_true is reasonably fast
    Timer timer;
    timer.start();

    const int iterations = 10000;
    volatile bool result;
    for (int i = 0; i < iterations; ++i) {
        result = random_true(0.5);
        (void)result;
    }

    auto duration = timer.get_duration_seconds();
    REQUIRE(duration.has_value());

    // Should complete 10,000 iterations in less than 1 second
    CHECK(duration.value().count() < 1.0);
}

TEST_CASE("random_false performance") {
    // Test that random_false is reasonably fast
    Timer timer;
    timer.start();

    const int iterations = 10000;
    volatile bool result;
    for (int i = 0; i < iterations; ++i) {
        result = random_false(0.5);
        (void)result;
    }

    auto duration = timer.get_duration_seconds();
    REQUIRE(duration.has_value());

    // Should complete 10,000 iterations in less than 1 second
    CHECK(duration.value().count() < 1.0);
}