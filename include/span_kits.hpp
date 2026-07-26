#pragma once
#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <iterator>
#include <numeric>
#include <optional>
#include <span>
#include <stdexcept>
#include <vector>
#include "container_kits.hpp"
#include "common_concepts.hpp"

namespace zuc {
template <typename Range>
using range_value_type = std::remove_cv_t<
    typename std::iterator_traits<decltype(std::begin(std::declval<Range&>()))>::value_type
>;

/**
 * @brief Safely creates a subspan with bounds checking
 * @param s The input span
 * @param offset Starting position (must be <= s.size())
 * @param count Number of elements (default: dynamic_extent for remaining elements)
 * @return std::optional<std::span<T>> containing the subspan, or nullopt if invalid
 * @note Returns a subspan starting at offset with the specified count, or nullopt if the operation would be out of bounds
 * @example
 * std::vector<int> data = {1, 2, 3, 4, 5};
 * auto result = sub_span_safe(std::span(data), 1, 3);
 * if (result) {
 *     // result.value() contains {2, 3, 4}
 * }
 */
template <typename T>
inline std::optional<std::span<T>> sub_span_safe(
    std::span<T> s, size_t offset, size_t count = std::dynamic_extent) {
    if (offset > s.size()) {
        return std::nullopt;
    }
    if (count != std::dynamic_extent) {
        if (offset + count > s.size()) {
            return std::nullopt;
        }
    }
    return s.subspan(offset, count);
}

/**
 * @brief Converts a span to a vector with the same elements
 * @tparam Range Range type
 * @param range The input range
 * @return std::vector containing all elements from the range
 * @note Creates a new vector containing all elements from the span. Useful when you need owned storage or want to modify elements without affecting the original data
 * @example
 * std::array<int, 3> arr = {1, 2, 3};
 * auto vec = convert_span_to_vector(std::span(arr));
 * // vec is now {1, 2, 3}
 */

template <typename Range>
inline auto convert_span_to_vector(const Range& range) {
    using value_type = std::decay_t<decltype(*std::begin(range))>;
    return std::vector<value_type>(std::begin(range), std::end(range));
}
/**
 * @brief Checks if a span contains a specific element
 * @param sp The span to search in
 * @param contain_obj The element to look for
 * @return true if the element is found, false otherwise
 * @note Performs a linear search to determine if the element exists in the span
 * @example
 * std::span<const int> numbers = {1, 2, 3, 4, 5};
 * bool found = contains(numbers, 3); // returns true
 */
template <typename T>
constexpr bool contains(const std::span<T> sp, const T& contain_obj) {
    if (sp.empty()) {
        return false;
    }
    return std::find(sp.begin(), sp.end(), contain_obj) != sp.end();
}

/**
 * @brief Checks if a span contains any of the specified elements
 * @param sp The span to search in
 * @param contain_objs Span of elements to look for
 * @return true if any element is found, false otherwise
 * @note Returns true if at least one element from contain_objs exists in obj
 * @example
 * std::span<int> numbers = {1, 2, 3, 4, 5};
 * bool found = contains_any(numbers, std::span{7, 8, 3}); // returns true
 */
template <typename T>
constexpr bool contains_any(const std::span<T> sp, const std::span<T> contain_objs) {
    if (sp.empty() || contain_objs.empty()) {
        return false;
    }
    for (const auto& x : contain_objs) {
        if (contains(sp, x)) return true;  
    }
    return false;
}

template <typename Range, typename ElemType = get_rangelike_value_type<Range>>
    requires RangeLike<Range>
constexpr bool contains_if(const Range& range, UnaryPred<ElemType> auto pred) {
    return std::any_of(range.begin(), range.end(), pred);
    
}

/**
 * @brief Finds the first occurrence of a subspan within a span
 * @param obj The span to search in
 * @param target The subspan to look for
 * @return std::span<T> pointing to the found subspan, or empty if not found
 * @note Searches for target within obj and returns a span pointing to the matching sequence
 * @example
 * std::span<int> main_span = {1, 2, 3, 4, 5, 6};
 * std::span<int> target = {3, 4, 5};
 * auto found = find_subspan(main_span, target);
 * // found contains {3, 4, 5}
 */
template <typename T>
requires std::equality_comparable<T>
constexpr std::span<T> find_subspan(std::span<T> obj, std::span<T> target) {
    if (target.empty()) {
        return {};
    }
    if (obj.size() < target.size()) {
        return {};
    }
    auto it = std::search(obj.begin(), obj.end(), target.begin(), target.end());
    if (it == obj.end()) {
        return {};
    }
    size_t offset = std::distance(obj.begin(), it);
    return obj.subspan(offset, target.size());
}

/**
 * @class ConcatSpan
 * @brief Concatenates multiple spans into a single contiguous view
 * @tparam T The element type
 * @tparam N The number of spans to concatenate (must be known at compile time)
 * @note Provides a unified view over multiple spans, allowing iteration and random access as if they were a single contiguous span. Useful for working with data that's split across multiple containers without copying the data
 * @example
 * std::array<int, 3> part1 = {1, 2, 3};
 * std::vector<int> part2 = {4, 5, 6};
 * ConcatSpan<int, 2> concatenated({std::span(part1), std::span(part2)});
 * for (int val : concatenated) {
 *     // Iterates over 1, 2, 3, 4, 5, 6
 * }
 */
template <typename T, size_t N>
class ConcatSpan {
   public:
    /**
     * @brief Returns the total number of elements across all spans
     * @return Total size of the concatenated view
     */
    const size_t constexpr size() const { return total_size_; }

    /**
     * @brief Returns the array of underlying spans
     * @return Array containing all the spans that make up this concatenation
     */
    const std::array<std::span<T>, N> constexpr get_data() const {
        return all_data_;
    }

    /**
     * @brief Constructs a ConcatSpan from an array of spans
     * @param data Array of spans to concatenate
     */
    ConcatSpan(const std::array<std::span<T>, N>& data) {
        all_data_ = data;
        total_size_ = std::accumulate(
            data.begin(), data.end(), 0,
            [](size_t acc, const std::span<T>& s) { return acc + s.size(); });
    }

    /**
     * @class ConstIterator
     * @brief Random access iterator for ConcatSpan
     * @note Provides bidirectional iteration and random access across the concatenated spans, handling the complexity of spanning multiple underlying spans transparently
     */
    class ConstIterator {
       public:
        /**
         * @brief Constructs an iterator for a ConcatSpan
         * @param parent Pointer to the parent ConcatSpan
         * @param pos Starting position (default: 0)
         */
        using value_type = std::remove_cv_t<T>;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;
        using iterator_category = std::random_access_iterator_tag;
        ConstIterator(const ConcatSpan* parent, size_t pos = 0)
            : parent_(parent), pos_(pos), max_size_(parent_->size()) {
            assert(parent != nullptr);
            assert(pos <= parent_->size());
        }

        /**
         * @brief Pre-increment operator
         * @return Reference to this iterator
         */
        ConstIterator& operator++() {
            assert(pos_ < max_size_ && "pos is already at the end");
            ++pos_;
            return *this;
        }

        /**
         * @brief Pre-decrement operator
         * @return Reference to this iterator
         */
        ConstIterator& operator--() {
            assert(pos_ > 0 && "pos already is 0");
            --pos_;
            return *this;
        }

        /**
         * @brief Post-increment operator
         * @return Copy of iterator before increment
         */
        ConstIterator operator++(int) {
            assert(pos_ < max_size_ && "pos is already at the end");
            ConstIterator temp(*this);
            ++*this;
            return temp;
        }

        /**
         * @brief Post-decrement operator
         * @return Copy of iterator before decrement
         */
        ConstIterator operator--(int) {
            assert(pos_ > 0 && "pos already is 0");
            ConstIterator temp(*this);
            --*this;
            return temp;
        }

        /**
         * @brief Addition operator for random access
         * @param offset Number of positions to advance
         * @return New iterator at advanced position
         */
        ConstIterator operator+(size_t offset) {
            assert(pos_ + offset <= max_size_);
            return {parent_, pos_ + offset};
        }

        /**
         * @brief Subtraction operator for random access
         * @param offset Number of positions to go back
         * @return New iterator at retreated position
         */
        ConstIterator operator-(size_t offset) {
            assert(pos_ >= offset);
            return {parent_, pos_ - offset};
        }

        /**
         * @brief Subtraction assignment operator
         * @param offset Number of positions to go back
         * @return Reference to this iterator
         */
        ConstIterator operator-=(size_t offset) {
            assert(pos_ >= offset);
            pos_ -= offset;
            return *this;
        }

        /**
         * @brief Addition assignment operator
         * @param offset Number of positions to advance
         * @return Reference to this iterator
         */
        ConstIterator operator+=(size_t offset) {
            assert(pos_ + offset <= max_size_);
            pos_ += offset;
            return *this;
        }

        /**
         * @brief Equality comparison operator
         * @param it Iterator to compare with
         * @return true if iterators point to same position
         */
        bool operator==(const ConstIterator& it) const {
            return it.pos_ == pos_;
        }

        /**
         * @brief Inequality comparison operator
         * @param it Iterator to compare with
         * @return true if iterators point to different positions
         */
        bool operator!=(const ConstIterator& it) const {
            return it.pos_ != pos_;
        }

        /**
         * @brief Dereference operator
         * @return Const reference to the element at current position
         */
        const T& operator*() const {
            assert(pos_ < parent_->size());
            size_t offset = 0;
            for (const auto& span : parent_->all_data_) {
                if (pos_ < offset + span.size()) {
                    return span[pos_ - offset];
                }
                offset += span.size();
            }
        }

       protected:
       private:
        const ConcatSpan* const parent_ = nullptr;
        size_t pos_ = 0;
        size_t max_size_;
    };


    /**
     * @brief Returns iterator to the beginning
     * @return Iterator pointing to first element
     */
    ConstIterator begin() { return {this}; }

    /**
     * @brief Returns iterator to the end
     * @return Iterator pointing past the last element
     */
    ConstIterator end() { return {this, total_size_}; }

    /**
     * @brief Returns const iterator to the beginning
     * @return Const iterator pointing to first element
     */
    ConstIterator cbegin() { return {this}; }

    /**
     * @brief Returns const iterator to the end
     * @return Const iterator pointing past the last element
     */
    ConstIterator cend() { return {this, total_size_}; }

    /**
     * @brief Random access operator (no bounds checking)
     * @param index Element index
     * @return Const reference to element at index
     */
    const T& operator[](size_t index) const {
        return *ConstIterator(this, index);
    }

    /**
     * @brief Random access with bounds checking
     * @param index Element index
     * @return Const reference to element at index
     * @throws std::out_of_range if index >= size()
     */
    const T& at(size_t index) const {
        if (index >= total_size_) {
            throw std::out_of_range("ConcatSpan::at: index out of range");
        }
        return *ConstIterator(this, index);
    }

   protected:
   private:
    std::array<std::span<T>, N> all_data_;
    size_t total_size_;
};

}  // namespace zuc