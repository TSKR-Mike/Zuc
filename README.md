# zuc (Zero-overhead Utility Collection)

✨ **Modern C++20 Utilities Library**

zuc is a modern, header-only C++ library that provides fast, intuitive utilities for everyday programming tasks. Built with C++20 standards, it combines performance with convenience - offering zero-allocation views, safe I/O operations, and a growing collection of tools to make C++ development more enjoyable.

## 🔥 Why I Built Zuc (and Why You Might Need It)
While Abseil and Folly are great, they require heavy dependencies and non-header-only builds. Zuc is designed for developers who want a lightweight, header-only alternative with a focus on modern C++20 features.
Here are 5 real-world C++ pain points I encountered repeatedly, and how Zuc eliminates them:

### 1. 🧵 String Splitting Requires a Loop and Copy Every Time
```cpp
// ❌ Standard library: manual loop + index management + heap copies
std::string s = "hello,world,cpp";
std::vector<std::string> parts;
size_t start = 0, end = 0;
while ((end = s.find(',', start)) != std::string::npos) {
    parts.emplace_back(s.substr(start, end - start));
    start = end + 1;
}
parts.emplace_back(s.substr(start));

// ✅ Zuc: one line, zero copy (returns string_view views)
auto parts = zuc::split(s, ',');
```
### 2. 💥 Integer Overflow is Undefined Behavior
```cpp
// ❌ Standard library: no protection, UB on overflow
int32_t a = 2'000'000'000, b = 2'000'000'000;
int32_t c = a + b;  // 💥 UB (typically wraps to negative)

// ✅ Zuc: returns optional, explicitly signals overflow
auto result = zuc::checked_add(a, b);
if (result) use(*result); else handle_overflow();
```

### 3. 🧹 Manual Resource Cleanup is Error-Prone
```cpp
// ❌ Standard library: manual unlock on every return path
void process() {
    std::mutex m;
    m.lock();
    if (error) { m.unlock(); return; }
    // ... many branches
    m.unlock();
}

// ✅ Zuc: one-liner RAII guard
void process() {
    std::mutex m;
    auto guard = zuc::scope_guard([&] { m.unlock(); });
    // ... even if early return, mutex is guaranteed to unlock
}
```
### 4. 🐌 Concatenating Multiple Vectors Copies Everything
```cpp
// ❌ Standard library: multiple allocations and copies
std::vector<int> a{1,2}, b{3,4}, c{5,6};
std::vector<int> all;
all.insert(all.end(), a.begin(), a.end());
all.insert(all.end(), b.begin(), b.end());
all.insert(all.end(), c.begin(), c.end());

// ✅ Zuc: zero-copy concatenated view
zuc::ConcatSpan<int, 3> all({a, b, c});  // iterate as one contiguous range
for (int x : all) { /* ... */ }
```

### 5. 📦 pop_back() Returns Nothing
```cpp
// ❌ Standard library: two lines, no empty-container semantics
if (!v.empty()) { auto val = v.back(); v.pop_back(); }

// ✅ Zuc: one line, returns optional (nullopt for empty)
auto val = zuc::pop_back_value(v);
if (val) use(*val);
```

### 🚀 No Overhead, No Surprises
- **Header-only** - Just #include "zuc/string_kits.hpp", no linking hassle. No ABI worries.

- **C++20 Core**: Uses Concepts for readable error messages, constexpr where possible.

Tested on: GCC, Clang, MSVC (CI passes).


## 🎯 Key Features
- **Zero-Overhead Philosophy** - Designed for maximum performance with minimal runtime cost
- **Zero-Allocation Views** - Non-allocating string operations using `std::string_view`
- **Modern C++20** - Leverages latest C++ features including concepts, ranges, and formatting
- **Noexcept Safety** - Functions are `noexcept` where no dynamic allocation occurs
- **Header-Only** - Easy integration, no build dependencies
- **Exception Safety** - RAII-based resource management with comprehensive error handling
- **Comprehensive Time Support** - Easy-to-use timing, date manipulation, and benchmarking tools
- **Type-Safe Operations** - Strong type checking using C++20 concepts
- **Well-Documented** - Extensive documentation with examples for all APIs

## 📦 What's Inside

### String Utilities (`string_kits.hpp`)
- **String Slicing** - `slice()`, `prefix()`, `suffix()`, `remove_prefix()`, `remove_suffix()`
- **Trimming** - `trim_left()`, `trim_right()`, `trim()` with view variants
- **Content Checks** - `contains()`, `contains_any()`, `contains_all()`
- **Replace/Remove** - In-place and copy variants for string manipulation
- **Splitting/Joining** - `split()`, `split_by_any()`, `join()` for string manipulation
- **Pattern Matching** - `match_any()` for checking against multiple patterns

### Container Utilities (`container_kits.hpp`)
- **Content Checking** - `contains()`, `contains_any()`, `contains_if()` for element searching
- **Value Extraction** - `pop_back_value()`, `pop_front_value()` for safe element removal
- **Map Operations** - `get_or_insert()`, `get_or_insert_default()`, `get_map_keys()`, `get_map_values()`
- **Container Slicing** - `slice()` with step support for sub-container extraction
- **Safe Access** - `at()`, `at_ref()` for safe element access with bounds checking
- **Container Modification** - `erase_if()`, `merge()` for safe container manipulation
- **Container Transformation** - `transform_all_to_vector()` for element mapping

### Span Utilities (`span_kits.hpp`)
- **Safe Span Operations** - `sub_span_safe()` for bounds-checked span slicing with proper error handling
- **Span Conversion** - `convert_span_to_vector()` for efficient span-to-vector conversion
- **Content Search** - `contains()`, `contains_any()` for element checking in spans
- **Subspan Finding** - `find_subspan()` for locating subspans within spans
- **Span Concatenation** - `ConcatSpan` class for combining multiple spans into a single view
- **Random Access** - Full random access support for concatenated spans
- **Iterator Support** - Complete iterator interface for range-based for loops

### I/O Operations (`io_kits.hpp`)
- **File Handling** - `FileMgr` class for safe file operations with RAII
- **File Writing** - `write_string()`, `write_a_line()`, `write_all()` for flexible file output
- **Console I/O** - Formatted console output and input functions
- **Line Reading** - Efficient line-by-line file reading with error handling
- **Exception Safety** - Custom exceptions for file and I/O operations
- **Convenient API** - Simple, intuitive functions that make I/O operations effortless

### Time Utilities (`time_kits.hpp`)
- **Timer & Stopwatch** - Easy-to-use timing utilities for performance measurement
- **DateTime Class** - Convenient date/time manipulation with arithmetic operations
- **Function Benchmarking** - Simple function timing with detailed statistics
- **Time Formatting** - Built-in time string formatting with various precision levels
- **Sleep Functions** - Convenient thread sleeping with duration support

### Random Generation (`random_kits.hpp`)
- **Basic Random Numbers** - `random_int()`, `random_double()`, `random_long_long()`
- **Sized Random Numbers** - `random_int32()`, `random_int64()` for specific bit widths
- **Boolean Random Generation** - `random_true()`, `random_false()` for probability-based boolean generation
- **Uniform Distribution** - Semantic aliases for uniform distribution functions
- **Container Shuffling** - `shuffle()`, `shuffled()` for randomizing container order (random access only)
- **Thread Safety** - Modern C++ random engine with proper seeding

### Numerics Utilities (`numerics_kits.hpp`)
- **Compile-time Type Selection** - `select_by_max_unsigned_t`, `select_by_range_t`, `select_by_value_t`
- **Overflow-Checked Arithmetic** - `checked_add()`, `checked_sub()`, `checked_mul()`
- **Platform Optimizations** - Uses compiler intrinsics for optimal performance
- **Safe Numeric Operations** - Returns `std::optional` for error handling

### RAII Utilities (`raii_kits.hpp`)
- **Scope Guards** - `generate_scope_guard()` for automatic cleanup
- **Success/Failure Guards** - Conditional execution based on exception state
- **Resource Management** - Automatic cleanup with RAII patterns
- **Dismissable Guards** - Control when cleanup actions execute

### String Conversion (`string_converts.hpp`)
- **Range to String** - `convert_range_to_string()` for containers
- **Binary Data Handling** - `convert_to_bytes()` for object serialization
- **Hexadecimal Conversion** - `convert_to_hex()` for binary representation
- **Numeric Parsing** - `try_convert_string_to_numerics()` for safe number conversion

### Common Concepts (`common_concepts.hpp`)
- **Type Constraints** - `Stringable`, `OneOf`, `UnaryPred`, `BinaryPred`
- **Callable Concepts** - `InvocableReturns` for function type requirements
- **Range Concepts** - `RangeLike`, `RangeLikeElemTypeSpecified` for container constraints
- **Compile-time Safety** - Type-safe template programming with concepts

## 🛠️ Quick Start

### Installation
1. Clone the repository or download the include directory
2. Copy everything inside the `./include` directory to your code base
3. Include the desired headers in your project

**That's it!** No build configuration needed.

### Basic Usage
```cpp
#include "string_kits.hpp"       // For string operations
#include "io_kits.hpp"           // For file I/O
#include "time_kits.hpp"         // For time utilities
#include "span_kits.hpp"         // For span operations
#include "random_kits.hpp"       // For random number generation
#include "container_kits.hpp"    // For container operations
#include "common_concepts.hpp"   // For common type concepts
#include "numerics_kits.hpp"     // For safe numeric operations
#include "raii_kits.hpp"         // For RAII utilities
#include "string_converts.hpp"   // For string conversion

using namespace zuc;

// Start using the library!
auto text = "  Hello, World!  ";
auto trimmed = trim(text);  // "Hello, World!"

// Container operations
std::vector<int> numbers = {1, 2, 3, 4, 5};
bool found = contains(numbers, 3);  // true

// Random shuffling
shuffle(numbers);  // Randomize the order
auto shuffled_copy = shuffled(numbers);  // Get shuffled copy, original unchanged

// Safe numeric operations
auto sum = checked_add<int>(200, 55);  // Optional<int> containing 255
if (sum) {
    // Safe to use the result
}

// RAII scope guard
{
    FILE* file = fopen("data.txt", "r");
    auto guard = generate_scope_guard([file]() { fclose(file); });
    // File will be closed automatically
}
```

### Building Tests
```bash
mkdir build && cd build
cmake ..
cmake --build .
./zuc_test  # Run tests
```

**Notice** - You **DON'T** need to clone the CMake files for your own projects. They are included only for building and running the test suite. The CMake files contain personal development settings and are not required for using the library.

## 💡 Examples

### Container Utilities - Safe & Convenient Container Operations

```cpp
#include "container_kits.hpp"

using namespace zuc;

// ❌ Traditional approach - verbose and error-prone
std::vector<int> numbers = {1, 2, 3, 4, 5};
bool found = false;
for (int num : numbers) {
    if (num == 3) {
        found = true;
        break;
    }
}

// ✅ zuc approach - simple and safe
std::vector<int> numbers = {1, 2, 3, 4, 5};
bool found = contains(numbers, 3);  // true

// Check if any element matches condition
bool has_even = contains_if(numbers, [](int x) { return x % 2 == 0; });  // true

// Check if container contains any of multiple values
std::vector<int> search = {2, 4, 6};
bool found_any = contains_any(numbers, search);  // true

// Safe value extraction from containers
std::vector<int> vec = {1, 2, 3};
auto last_value = pop_back_value(vec);  // Optional<int> containing 3
if (last_value) {
    // Use the value safely
}

std::list<int> lst = {1, 2, 3};
auto first_value = pop_front_value(lst);  // Optional<int> containing 1

// Convenient map operations
std::unordered_map<std::string, int> scores;
auto& score = get_or_insert(scores, "player1", 100);  // Insert if not exists
auto& default_score = get_or_insert_default(scores, "player2", 0);  // Insert with default

// Extract keys or values from maps
std::unordered_map<std::string, int> user_ages = {{"Alice", 25}, {"Bob", 30}};
auto names = get_map_keys(user_ages);   // {"Alice", "Bob"}
auto ages = get_map_values(user_ages); // {25, 30}

// Container slicing with step support
std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9};
auto even_numbers = slice(data, 1, 9, 2);  // {2, 4, 6, 8}
auto every_third = slice(data, 0, 9, 3);   // {1, 4, 7}

// Safe container modification
std::vector<int> numbers = {1, 2, 3, 4, 5, 6};
erase_if(numbers, [](int x) { return x % 2 == 0; });  // {1, 3, 5}

// Merge containers
std::vector<int> first = {1, 2, 3};
std::vector<int> second = {4, 5, 6};
auto merged = merge(first, second);  // {1, 2, 3, 4, 5, 6}

// Transform container elements
std::vector<int> numbers = {1, 2, 3, 4, 5};
auto squared = transform_all_to_vector<int>(numbers, [](int x) { return x * x; });
// {1, 4, 9, 16, 25}
```

### Time Utilities - Effortless Time Management

```cpp
#include "time_kits.hpp"

using namespace zuc;

// ❌ Traditional approach - verbose and error-prone
auto start = std::chrono::high_resolution_clock::now();
// ... do some work ...
auto end = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
std::cout << "Time: " << duration.count() << "ms" << std::endl;

// ✅ zuc approach - simple and convenient
Timer timer;
timer.start();
// ... do some work ...
auto elapsed = timer.get_duration_seconds();
write_a_line_to_console("Time: {}", format_duration(*elapsed, TimePrecision::millisecond_precs));

// Stopwatch with pause/resume - incredibly convenient
Stopwatch sw;
sw.start();
// ... work ...
sw.pause();  // Pause timing
// ... do something else ...
sw.resume(); // Continue timing
// ... more work ...
sw.stop();
write_a_line_to_console("Total time: {}", format_duration(sw.get_elapsed(), TimePrecision::second_precs));

// DateTime manipulation - intuitive and powerful
auto now = DateTime::now();
auto tomorrow = now.offset_days(1);
auto next_week = now + std::chrono::days{7};

write_a_line_to_console("Today: {}", now.to_string());
write_a_line_to_console("Tomorrow: {}", tomorrow.to_string());

// Create specific dates easily
auto birthday = DateTime::from_ymd(2000, 6, 15, 14, 30, 0);
auto age = DateTime::now() - birthday;
write_a_line_to_console("Age: {} days", std::chrono::duration_cast<std::chrono::days>(age).count());

// Benchmark functions with detailed statistics - convenience at its best
void my_function() {
    // ... some work ...
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

TimingSetting settings{
    .print_after_each_run = true,
    .times_to_run = 5,
    .prec = TimePrecision::millisecond_precs
};

time_a_function("my_function", settings, my_function);
// Output:
// Start timing my_function at 2026-07-17 14:30:45
// [1/5] Completed in 100.23 ms
// [2/5] Completed in 99.87 ms
// [3/5] Completed in 100.45 ms
// [4/5] Completed in 100.12 ms
// [5/5] Completed in 100.34 ms
// All 5 runs completed in 501.01ms
// Max:100.45 ms
// Min:99.87 ms
// Avg:100.20 ms

// Convenient sleep function
sleep(2.5); // Sleep for 2.5 seconds - so much simpler than std::this_thread::sleep_for
```

### String Kits - Zero-Allocation Convenience

```cpp
#include "string_kits.hpp"

using namespace zuc;

// ❌ Traditional approach - allocates new strings
std::string text = "Hello, World!";
std::string substring = text.substr(7, 5);  // Creates new string

// ✅ zuc approach - zero allocation
std::string_view text = "Hello, World!";
auto result = slice(text, 7, 5);  // Returns view, no allocation!

// Chain operations efficiently
auto cleaned = trim(slice(raw_data, 10, 50));  // No intermediate allocations

// Advanced content checks
std::string_view log = "[ERROR] File not found: config.txt";
if (contains_any(log, {"[ERROR]", "[FATAL]", "[CRITICAL]"})) {
    // Handle error - zero allocation check!
}

// Safe in-place modifications
std::string config = "key=value\n";
replace_all(config, "=", ": ");  // "key: value\n"
```

### Span Kits - Safe Container Views

```cpp
#include "span_kits.hpp"

using namespace zuc;

// ❌ Traditional approach - manual bounds checking
std::vector<int> data = {1, 2, 3, 4, 5};
size_t offset = 2, count = 3;
if (offset + count <= data.size()) {
    // Safe to access data[offset] to data[offset + count - 1]
}

// ✅ zuc approach - safe span operations
std::vector<int> data = {1, 2, 3, 4, 5};
auto result = sub_span_safe(std::span(data), 2, 3);
if (result) {
    // Safe to use the span - bounds already checked!
    for (int val : result.value()) {
        // Process values 3, 4, 5
    }
}

// Convert span to vector efficiently
std::array<int, 4> arr = {10, 20, 30, 40};
auto vec = convert_span_to_vector(std::span(arr));
// vec is now {10, 20, 30, 40}

// Check if span contains elements
std::span<const int> numbers = {1, 2, 3, 4, 5};
if (contains(numbers, 3)) {
    // Found 3 in the span
}

if (contains_any(numbers, std::span{7, 8, 3})) {
    // Found any of 7, 8, or 3 in the span
}

// Find subspan within a span
std::span<int> main_span = {1, 2, 3, 4, 5, 6, 7, 8, 9};
std::span<int> target = {4, 5, 6};
auto found = find_subspan(main_span, target);
// found is a span pointing to {4, 5, 6} within main_span

// Concatenate multiple spans into a single view
std::array<int, 3> part1 = {1, 2, 3};
std::vector<int> part2 = {4, 5, 6};
std::array<int, 2> part3 = {7, 8};

ConcatSpan<int, 3> concatenated({
    std::span(part1),
    std::span(part2), 
    std::span(part3)
});

// Iterate over concatenated spans as if they were one
for (const auto& val : concatenated) {
    // Access 1, 2, 3, 4, 5, 6, 7, 8 seamlessly
}

// Random access to concatenated spans
int third_element = concatenated[2];  // Access 3
int sixth_element = concatenated[5];  // Access 6
```

### I/O Operations - Safe, Simple & Convenient

```cpp
#include "io_kits.hpp"

using namespace zuc;

// ❌ Traditional C-style - error-prone
FILE* file = fopen("data.txt", "r");
if (!file) { /* handle error */ }
char buffer[256];
if (fgets(buffer, sizeof(buffer), file)) { /* ... */ }
fclose(file);  // Easy to forget!

// ✅ zuc approach - RAII, exception safe & incredibly convenient
try {
    FileMgr file("data.txt", "r");  // Automatically closed when out of scope
    
    // Read all lines efficiently
    auto lines = file.read_all();
    for (const auto& line : lines) {
        write_a_line_to_console("Line: {}", line);
    }
    
    // Write content without newline
    file.write_string("Processing: {} items", lines.size());
    
    // Write content with automatic newline
    file.write_a_line("Total lines: {}", lines.size());
    
    // Write multiple items, one per line
    std::vector<std::string> items = {"item1", "item2", "item3"};
    file.write_all(std::span(items));
    
} catch (const FileException& e) {
    write_a_line_to_console("Error: {}", e.what());
}

// Interactive console I/O - convenient and type-safe
auto name = read_a_line_from_console("Enter your name: ");
write_a_line_to_console("Hello, {}!", name);

// Write formatted content easily
file.write("Processing complete at: {}\n", get_today_time_detailed_str());
```

### Why zuc - The Ultimate Convenience Library?

| Feature | Traditional C++ | zuc |
|---------|----------------|--------|
| String slicing | Creates new strings | Zero-allocation views |
| Container operations | Manual loops and checks | Simple, safe functions |
| Span operations | Manual bounds checking | Safe, bounds-checked operations |
| File handling | Manual resource management | RAII with automatic cleanup |
| Error handling | Return codes | Exception-based |
| Console I/O | printf/scanf (type-unsafe) | Type-safe formatting |
| Time operations | Verbose chrono code | Simple, intuitive API |
| Date manipulation | Complex calculations | Easy arithmetic operations |
| Function benchmarking | Manual timing code | One-function solution |
| vs stdio | Manual cleanup, error-prone | Convenient, exception-safe |

## 📋 Requirements

- **C++20** compatible compiler (GCC 13+, Clang 19+, MSVC 2022+)
- Standard library implementation with C++20 support
- For DateTime class: Full C++20 chrono implementation (define `ZUC_DROP_CHRONO_TIME_ZONE` if unavailable)

## 🧪 Testing

The library includes comprehensive test coverage using the doctest framework:
- **String Operations** - Slicing, trimming, splitting, joining, replacing
- **File I/O** - Reading, writing, formatted output, error handling
- **Time Utilities** - Timers, stopwatches, DateTime, benchmarking
- **Span Operations** - Safe slicing, conversion, searching, concatenation
- **Random Generation** - Various random number generators

Run tests with:
```bash
cmake --build . && ctest
```

## ⚠️ Maintenance Note

This project is maintained in my limited free time. While I appreciate issues and questions, please understand that I may not be able to respond quickly or address all requests promptly. Thank you for your understanding!

## 🤝 Contributing

Contributions are welcome! Please ensure:
- All code follows the existing style and conventions
- New features include comprehensive tests
- Documentation is updated for any API changes
- C++20 best practices are followed

## 🎯 Design Philosophy

zuc is built with one primary goal: **making C++ development more convenient**. Every feature is designed to:

- **Reduce boilerplate** - Less code, more functionality
- **Handle complexity** - Let the library manage the details
- **Provide safety** - RAII, exceptions, and type-safety built-in
- **Stay modern** - Leverage C++20 features for cleaner code
- **Remain intuitive** - APIs that work the way you'd expect
- **Ensure correctness** - Comprehensive bounds checking and error handling

Whether you're manipulating strings, handling files, or working with time, zuc provides the most convenient interface possible.

## 📝 License

Copyright (c) 2026, Under Apache License 2.0
See [LICENSE](LICENSE) for details.

---
Developed by TSKR-Mike@github.com

## 📋 Compatibility

Tested and verified on:
- **GCC 13+** - Full support including all features
- **Clang 19+** - Full support except DateTime class (define `ZUC_DROP_CHRONO_TIME_ZONE` to work around, but it passed on my debian13's llvm19.If you download official llvm, the minium version is 22(with full chrono support))
- **MSVC 2022+** - Full support including all features

### Platform Support
- **Windows** - Primary development platform
- **Linux** - Full support
- **macOS** - Should work (community testing appreciated)

## 📚 API Reference

### Common Concepts (`common_concepts.hpp`)
- `Stringable<T>` - Types that can be converted to strings
- `OneOf<T, Types...>` - Type constraint for allowed types
- `UnaryPred<F, T>` - Unary predicate concept
- `BinaryPred<F, T1, T2>` - Binary predicate concept
- `InvocableReturns<F, ReturnType, Args...>` - Callable with specific return type
- `RangeLike<R>` - Container has begin() and end()
- `RangeLikeElemTypeSpecified<R, T>` - Range of specific element type

### String Operations (`string_kits.hpp`)
- `slice()`, `prefix()`, `suffix()` - Zero-allocation substring operations
- `trim()`, `trim_left()`, `trim_right()` - Whitespace removal
- `split()`, `split_to_string()` - String splitting
- `join()` - String concatenation
- `contains()`, `contains_any()`, `contains_all()` - Content checking
- `replace_all()`, `remove_all()` - String modification
- `repeat()` - String repetition
- `match_any()` - Pattern matching

### Span Operations (`span_kits.hpp`)
- `sub_span_safe()` - Bounds-checked span slicing
- `convert_span_to_vector()` - Span to vector conversion
- `contains()`, `contains_any()` - Element searching
- `find_subspan()` - Subspan location
- `ConcatSpan<T, N>` - Multiple span concatenation

### I/O Operations (`io_kits.hpp`)
- `FileMgr` - RAII file management class
- `write_a_line_to_console()` - Formatted console output
- `read_a_line_from_console()` - Interactive console input
- `open_file()` - Safe file opening with error handling

### Time Operations (`time_kits.hpp`)
- `Timer` - Simple timing utility
- `Stopwatch` - Pause/resume timing
- `DateTime` - Date/time manipulation (requires full C++20 chrono)
- `time_a_function()` - Function benchmarking
- `sleep()` - Convenient sleeping
- `format_duration()` - Time formatting

### Random Operations (`random_kits.hpp`)
- `random_int()`, `random_double()` - Basic random numbers
- `random_int32()`, `random_int64()` - Sized integer random numbers
- `uniform_random_*()` - Uniform distribution aliases
- `shuffle()` - Shuffle container in place
- `shuffled()` - Return shuffled copy of container

### Container Operations (`container_kits.hpp`)
- `contains()` - Check if container contains element
- `contains_any()` - Check if container contains any of specified elements
- `contains_if()` - Check if container contains element matching predicate
- `pop_back_value()` - Safely get and remove last element from vector
- `pop_front_value()` - Safely get and remove first element from list/deque
- `get_or_insert()` - Get or insert value in unordered_map
- `get_or_insert_default()` - Get or insert with default value
- `get_map_keys()` - Extract keys from map-like containers
- `get_map_values()` - Extract values from map-like containers
- `slice()` - Slice containers with step support
- `at()` - Safe element access returning optional
- `at_ref()` - Safe element access returning reference (throws on error)
- `erase_if()` - Remove elements matching predicate
- `merge()` - Merge two containers
- `transform_all_to_vector()` - Transform container elements to vector

### Numerics Operations (`numerics_kits.hpp`)
- `checked_add<T>()` - Safe addition with overflow detection
- `checked_sub<T>()` - Safe subtraction with underflow detection
- `checked_mul<T>()` - Safe multiplication with overflow detection
- `select_by_max_unsigned_t<MAX>` - Compile-time type selection by maximum value
- `select_by_range_t<MIN, MAX>` - Compile-time type selection by range
- `select_by_value_t<VALUE>` - Compile-time type selection by single value

### RAII Operations (`raii_kits.hpp`)
- `generate_scope_guard()` - Create cleanup action that runs on scope exit
- `generate_scope_guard_success()` - Cleanup only if no exception thrown
- `generate_scope_guard_failed()` - Cleanup only if exception thrown
- `ScopeGuard::dismiss()` - Prevent cleanup action from running

### String Conversion (`string_converts.hpp`)
- `convert_range_to_string()` - Convert containers to string representation
- `convert_to_bytes()` - Convert objects to byte spans
- `convert_to_hex()` - Convert data to hexadecimal string
- `try_convert_string_to_numerics<T>()` - Safe string to number conversion
- `is_numeric()` - Check if string represents a number

## ⚠️ Limitations
- For types that can't be stored in std::optional (e.g., references), zuc::find(and other functions that return optional) currently returns a copy. We're exploring C++26's std::optional improvements.
- C++20 modules are experimental and not yet fully supported.

*"Built for convenience, designed for developers who value their time."*