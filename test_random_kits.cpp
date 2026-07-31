// tests/test_random_kits.cpp
#include "doctest.h"
#include <ostream>
#include "include/random_kits.hpp"
#include "include/io_kits.hpp"
#include "include/time_kits.hpp"
#include <vector>
#include <algorithm>

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
 * Comprehensive tests for random number generators including various types and ranges.
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
        // Test that we get values across the range
        std::vector<int> histogram(10, 0);
        const int iterations = 1000;
        
        for (int i = 0; i < iterations; ++i) {
            int value = random_int(0, 9);
            histogram[value]++;
        }
        
        // Check that each bucket got some hits (with high probability)
        int non_empty_buckets = 0;
        for (int count : histogram) {
            if (count > 0) {
                non_empty_buckets++;
            }
        }
        
        // With 1000 iterations and 10 buckets, we should have hits in most buckets
        CHECK(non_empty_buckets >= 8);
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
            (void)value; // Prevent optimization
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
        
        CHECK_NOTHROW(time_a_function("random int", setting, get_random_int_and_print));
    }

    TEST_CASE("random statistical properties") {
        // Test basic statistical properties
        const int sample_size = 1000;
        const int min_val = 0;
        const int max_val = 100;
        
        std::vector<int> samples;
        samples.reserve(sample_size);
        
        for (int i = 0; i < sample_size; ++i) {
            samples.push_back(random_int(min_val, max_val));
        }
        
        // Calculate mean
        double sum = 0.0;
        for (int val : samples) {
            sum += val;
        }
        double mean = sum / sample_size;
        
        // For uniform distribution, mean should be around (min + max) / 2
        double expected_mean = (min_val + max_val) / 2.0;
        
        // Allow 20% deviation due to randomness
        CHECK(std::abs(mean - expected_mean) < expected_mean * 0.2);
    }

    TEST_CASE("random reproducibility check") {
        // This test checks that the generator is working
        // (not testing for exact reproducibility as that would require seed control)
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