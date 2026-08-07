/**
 * @file random_kits.hpp
 * @brief Random number generation utilities for C++20
 * @date 2026-07-15
 * @copyright Copyright (c) 2026
 * @note Provides convenient functions for generating random numbers using a seeded Mersenne Twister generator (mt19937). All functions use a shared global generator for efficiency.
 */

#pragma once
#include <random>
#include <chrono>
#include <algorithm>
#include <cstdint>
#include "common_concepts.hpp"

namespace zuc {

// Global random device and generator initialization
thread_local inline std::random_device rd_device;

// Initialize seed with multiple entropy sources for better randomness
thread_local inline unsigned init_seed = std::random_device{}() 
              ^ static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()) 
              ^ reinterpret_cast<uintptr_t>(std::addressof(init_seed));

// Global Mersenne Twister generator (mt19937) - shared across all functions
thread_local inline std::mt19937 global_generator(init_seed);


/**
 * @brief Generate a random integer in the inclusive range [min, max]
 * @param min Minimum value (inclusive)
 * @param max Maximum value (inclusive)
 * @return Random integer between min and max
 */
int inline random_int(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(global_generator);
}

/**
 * @brief Generate a random double in the range [min, max)
 * @param min Minimum value (inclusive)
 * @param max Maximum value (exclusive)
 * @return Random double between min and max
 */
double inline random_double(double min, double max) {
    std::uniform_real_distribution<double> dist(min, max);
    return dist(global_generator);
}

/**
 * @brief Generate a random long long in the inclusive range [min, max]
 * @param min Minimum value (inclusive)
 * @param max Maximum value (inclusive)
 * @return Random long long between min and max
 */
long long inline random_long_long(long long min, long long max) {
    std::uniform_int_distribution<long long> dist(min, max);
    return dist(global_generator);
}

/**
 * @brief Generate a random 32-bit integer in the inclusive range [min, max]
 * @param min Minimum value (inclusive)
 * @param max Maximum value (inclusive)
 * @return Random int32_t between min and max
 */
int32_t inline random_int32(int32_t min, int32_t max) {
    std::uniform_int_distribution<int32_t> dist(min, max);
    return dist(global_generator);
}

/**
 * @brief Generate a random 64-bit integer in the inclusive range [min, max]
 * @param min Minimum value (inclusive)
 * @param max Maximum value (inclusive)
 * @return Random int64_t between min and max
 */
int64_t inline random_int64(int64_t min, int64_t max) {
    std::uniform_int_distribution<int64_t> dist(min, max);
    return dist(global_generator);
}

/**
 * @brief Generate a uniformly distributed random integer (alias for random_int)
 * @param min Minimum value (inclusive)
 * @param max Maximum value (inclusive)
 * @return Random integer between min and max
 * @note This is an alias for random_int() for semantic clarity
 */
int inline uniform_random_int(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(global_generator);
}

/**
 * @brief Generate a uniformly distributed random long long (alias for random_long_long)
 * @param min Minimum value (inclusive)
 * @param max Maximum value (inclusive)
 * @return Random long long between min and max
 * @note This is an alias for random_long_long() for semantic clarity
 */
long long inline uniform_random_long_long(long long min, long long max) {
    std::uniform_int_distribution<long long> dist(min, max);
    return dist(global_generator);
}

/**
 * @brief Generate a uniformly distributed random 32-bit integer (alias for random_int32)
 * @param min Minimum value (inclusive)
 * @param max Maximum value (inclusive)
 * @return Random int32_t between min and max
 * @note This is an alias for random_int32() for semantic clarity
 */
int32_t inline uniform_random_int32(int32_t min, int32_t max) {
    std::uniform_int_distribution<int32_t> dist(min, max);
    return dist(global_generator);
}

/**
 * @brief Generate a uniformly distributed random 64-bit integer (alias for random_int64)
 * @param min Minimum value (inclusive)
 * @param max Maximum value (inclusive)
 * @return Random int64_t between min and max
 * @note This is an alias for random_int64() for semantic clarity
 */
int64_t inline uniform_random_int64(int64_t min, int64_t max) {
    std::uniform_int_distribution<int64_t> dist(min, max);
    return dist(global_generator);
}

/**
 * @brief Generate a uniformly distributed random double (alias for random_double)
 * @param min Minimum value (inclusive)
 * @param max Maximum value (exclusive)
 * @return Random double between min and max
 * @note This is an alias for random_double() for semantic clarity
 */
double inline uniform_random_double(double min, double max) {
    std::uniform_real_distribution<double> dist(min, max);
    return dist(global_generator);
}

/**
 * @brief Shuffle a container using the default random engine
 * @tparam Container Container type with random access iterators
 * @param c Container to shuffle
 * @note Uses the global Mersenne Twister generator for reproducibility
 * @note Container must support random access (vector, deque, array, etc.)
 */
template <typename Container>
void shuffle(Container& c) {
    std::shuffle(std::begin(c), std::end(c), global_generator);
}

/**
 * @brief Shuffle a container using a custom random engine
 * @tparam Container Container type with random access iterators
 * @tparam URBG Uniform Random Bit Generator type
 * @param c Container to shuffle
 * @param g Random engine to use for shuffling
 * @note Allows flexibility to use different random engines
 * @note Container must support random access (vector, deque, array, etc.)
 */
template <typename Container, typename URBG>
void shuffle(Container& c, URBG&& g) {
    std::shuffle(std::begin(c), std::end(c), std::forward<URBG>(g));
}

/**
 * @brief Return a shuffled copy of a container
 * @tparam Container Container type with random access iterators
 * @param c Container to copy and shuffle
 * @return New container with shuffled elements
 * @note Original container remains unchanged
 * @note Container must support random access (vector, deque, array, etc.)
 */
template <typename Container>
Container shuffled(Container c) {
    shuffle(c);
    return c;
}


}  // namespace zuc