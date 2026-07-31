# zuc (Zero-overhead Utility Collection) - Documentation

## Table of Contents
- [Overview](#overview)
- [Installation](#installation)
- [Module Documentation](#module-documentation)
  - [String Utilities](#string-utilities-string_kitshpp)
  - [Span Utilities](#span-utilities-span_kitshpp)
  - [I/O Operations](#io-operations-io_kitshpp)
  - [Time Utilities](#time-utilities-time_kitshpp)
  - [Random Generation](#random-generation-random_kitshpp)
  - [Container Utilities](#container-utilities-container_kitshpp)
  - [Common Concepts](#common-concepts-common_conceptshpp)
  - [String Conversion](#string-conversion-string_convertshpp)
- [API Reference](#api-reference)
- [Examples](#examples)
- [Requirements](#requirements)
- [Best Practices](#best-practices)

## Overview

zuc is a modern, header-only C++20 library that provides fast, intuitive utilities for everyday programming tasks. Built with zero-overhead philosophy, it offers:

- **Zero-allocation views** for string operations
- **Safe I/O operations** with RAII resource management
- **Comprehensive time utilities** including benchmarking
- **Type-safe operations** using C++20 concepts
- **Exception safety** throughout the library

## Installation

### Quick Start
1. Copy the `include/` directory to your project
2. Include desired headers in your source files
3. Use `namespace zuc;` for convenience

### Building Tests
```bash
mkdir build && cd build
cmake ..
cmake --build .
./zuc_test
```

## Module Documentation

### String Utilities (`string_kits.hpp`)

String utilities provide zero-allocation operations using `std::string_view` for maximum performance.

**Key Features:**
- Zero-allocation substring operations
- Efficient trimming with view-based variants
- Flexible splitting and joining
- Pattern matching and content checking
- Safe in-place and copy-based modifications

#### Substring Operations

```cpp
// Get a substring view (zero allocation)
std::string_view text = "Hello, World!";
auto result = string_slice(text, 7, 5);  // "World"

// Get prefix/suffix views
auto first = prefix(text, 5);    // "Hello"
auto last = suffix(text, 6);     // "World!"

// Remove prefix/suffix views
auto without_first = remove_prefix(text, 7);  // "World!"
auto without_last = remove_suffix(text, 7);   // "Hello,"
```

#### Trimming Operations

```cpp
// Copy-based trimming (returns new std::string)
std::string text = "  Hello, World!  ";
auto trimmed = get_trimmed(text);           // "Hello, World!"
auto left_trimmed = get_trimmed_left(text); // "Hello, World!  "
auto right_trimmed = get_trimmed_right(text); // "  Hello, World!"

// In-place trimming (modifies original)
std::string mutable_text = "  Hello, World!  ";
trim(mutable_text);        // "Hello, World!"
trim_left(mutable_text);   // "Hello, World!  "
trim_right(mutable_text);  // "  Hello, World!"

// View-based trimming (zero allocation)
std::string_view view_text = "  Hello, World!  ";
auto trimmed_view = trim_view(view_text);  // "Hello, World!"
```

#### Splitting Operations

```cpp
// Split by single character (returns views)
std::string_view csv = "apple,banana,cherry";
auto parts = split(csv, ',');  // ["apple", "banana", "cherry"]

// Split by string delimiter
std::string_view text = "apple::banana::cherry";
auto parts = split(text, "::");  // ["apple", "banana", "cherry"]

// Split by any of multiple characters (returns strings)
std::string_view data = "apple,banana;cherry|date";
auto parts = split_by_any(data, ",;|");  // ["apple", "banana", "cherry", "date"]

// Split and convert to strings
auto string_parts = split_to_string(csv, ',');
```

#### Joining Operations

```cpp
std::vector<std::string_view> words = {"Hello", "World", "!"};
auto joined = join(words, " ");  // "Hello World !"
auto concatenated = join(words); // "HelloWorld!"
```

#### Content Checking

```cpp
std::string_view text = "Hello, World!";
bool has_world = contains(text, "World");  // true
bool has_any = contains_any(text, {"Hello", "Hi"});  // true
bool has_all = contains_all(text, {"Hello", "World"});  // true
```

#### Replace and Remove Operations

```cpp
// In-place replacement
std::string text = "Hello, World!";
replace_all(text, "World", "C++");  // "Hello, C++!"

// Copy-based replacement
auto replaced = get_all_replaced(text, "World", "C++");

// Remove substrings
std::string data = "Hello, World! World!";
remove_all(data, "World");  // "Hello, ! !"

// Remove multiple substrings
std::string messy = "<b>Hello</b><i>World</i>";
remove_all(messy, {"<b>", "</b>", "<i>", "</i>"});  // "HelloWorld"
```

#### String Repetition

```cpp
std::string base = "abc";
auto repeated = repeat(base, 3);  // "abcabcabc"
```

#### Pattern Matching

```cpp
std::string value = "apple";
bool matches = match_any(value, {"apple", "banana", "cherry"});  // true
```

### Span Utilities (`span_kits.hpp`)

Span utilities provide safe, bounds-checked operations on `std::span`.

**Key Features:**
- Bounds-checked span slicing
- Efficient span-to-vector conversion
- Content searching and subspan finding
- Multi-span concatenation with random access
- Complete iterator support for range-based loops

#### Safe Span Operations

```cpp
std::vector<int> data = {1, 2, 3, 4, 5};
auto result = sub_span_safe(std::span(data), 1, 3);
if (result) {
    // result.value() contains {2, 3, 4}
}
```

#### Span Conversion

```cpp
std::array<int, 4> arr = {10, 20, 30, 40};
auto vec = convert_span_to_vector(std::span(arr));
// vec is now {10, 20, 30, 40}
```

#### Content Search

```cpp
std::span<const int> numbers = {1, 2, 3, 4, 5};
bool found = contains(numbers, 3);  // true
bool found_any = contains_any(numbers, std::span{7, 8, 3});  // true
```

#### Subspan Finding

```cpp
std::span<int> main_span = {1, 2, 3, 4, 5, 6};
std::span<int> target = {3, 4, 5};
auto found = find_subspan(main_span, target);
// found contains {3, 4, 5}
```

#### Span Concatenation

```cpp
std::array<int, 3> part1 = {1, 2, 3};
std::vector<int> part2 = {4, 5, 6};
ConcatSpan<int, 2> concatenated({std::span(part1), std::span(part2)});

for (int val : concatenated) {
    // Iterates over 1, 2, 3, 4, 5, 6
}

// Random access
int third = concatenated[2];  // 3
int fifth = concatenated[4];  // 5
```

### I/O Operations (`io_kits.hpp`)

I/O operations provide safe, RAII-based file handling and convenient console I/O.

**Key Features:**
- RAII-based file management with automatic cleanup
- Formatted console I/O with type safety
- Efficient line-by-line file reading
- Comprehensive exception handling
- File pointer management utilities

#### File Management

```cpp
try {
    // Open file (automatically closed when out of scope)
    FileMgr file("data.txt", std::ios::in | std::ios::out);
    
    // Check if file is valid
    if (file.is_valid()) {
        // Read all lines
        auto lines = file.read_all();
        
        // Write content
        file.write("Hello, World!\n");
        
        // Formatted writing
        file.write("Processing: {} items\n", lines.size());
    }
} catch (const FileException& e) {
    std::cerr << "Error: " << e.what() << std::endl;
}
```

#### Console I/O

```cpp
// Formatted output
write_string_to_console("Hello, {}!\n", "World");
write_a_line_to_console("Number: {}", 42);

// Interactive input
auto name = read_a_line_from_console("Enter your name: ");
write_a_line_to_console("Hello, {}!", name);
```

#### File Operations

```cpp
// Safe file opening with error handling
auto file = open_file("data.txt");
if (file) {
    // Use file.value()
    auto lines = file.value().read_all();
}

// File pointer management
FileMgr file("data.txt", std::ios::in);
file.reset_read_pointer_to_front();  // Reset to beginning
file.reset_write_pointer_to_front(); // Reset write pointer
file.reset_write_pointer_to_last();  // Reset to end
```

### Time Utilities (`time_kits.hpp`)

Time utilities provide easy-to-use timing, date manipulation, and benchmarking.

**Key Features:**
- Simple timer and stopwatch utilities
- DateTime class with arithmetic operations
- Function benchmarking with detailed statistics
- Flexible time formatting
- Convenient sleep functions

**Note:** DateTime class requires full C++20 chrono implementation. Define `ZUC_DROP_CHRONO_TIME_ZONE` if unavailable.

#### Timer and Stopwatch

```cpp
// Simple timer
Timer timer;
timer.start();
// ... do work ...
auto duration = timer.get_duration_seconds();
if (duration) {
    write_a_line_to_console("Time: {} seconds", duration->count());
}

// Stopwatch with pause/resume
Stopwatch sw;
sw.start();
// ... work ...
sw.pause();
// ... do something else ...
sw.resume();
// ... more work ...
sw.stop();
write_a_line_to_console("Total: {} seconds", sw.get_elapsed().count());
```

#### DateTime Operations

```cpp
// Current time
auto now = DateTime::now();
write_a_line_to_console("Today: {}", now.to_string());

// Create specific date
auto birthday = DateTime::from_ymd(2000, 6, 15, 14, 30, 0);

// Date arithmetic
auto tomorrow = now.offset_days(1);
auto next_week = now + std::chrono::days{7};

// Time differences
auto age = DateTime::now() - birthday;
auto days = std::chrono::duration_cast<std::chrono::days>(age).count();

// Get components
int year = now.get_year();
int month = now.get_month();
int day = now.get_day();
```

#### Function Benchmarking

```cpp
void my_function() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

TimingSetting settings{
    .print_after_each_run = true,
    .times_to_run = 5,
    .prec = TimePrecision::millisecond_precs
};

time_a_function("my_function", settings, my_function);
```

#### Time Formatting

```cpp
auto duration = std::chrono::duration<double>(1.234);
auto formatted = format_duration(duration, TimePrecision::millisecond_precs);
// "1.23 ms"

// Get current time strings
auto today = get_today_str();  // "2026-07-26"
auto detailed = get_today_time_detailed_str();  // "2026-07-26 14:30:45"
```

#### Sleep Function

```cpp
sleep(2.5);  // Sleep for 2.5 seconds
```

### Random Generation (`random_kits.hpp`)

Random generation utilities provide convenient functions for generating random numbers.

**Key Features:**
- Basic random number generation (int, double, long long)
- Sized integer generation (int32, int64)
- Uniform distribution semantic aliases
- Modern C++ random engine with proper seeding
- Thread-safe operations

#### Basic Random Numbers

```cpp
// Random integer in range [min, max]
int random_value = random_int(1, 100);

// Random double in range [min, max)
double random_double = random_double(0.0, 1.0);

// Random long long
auto large_random = random_long_long(1000, 999999);
```

#### Sized Random Numbers

```cpp
// 32-bit random integer
int32_t random_32 = random_int32(INT32_MIN, INT32_MAX);

// 64-bit random integer
int64_t random_64 = random_int64(INT64_MIN, INT64_MAX);
```

#### Uniform Distribution Aliases

```cpp
// Semantic aliases for uniform distribution
auto uniform_int = uniform_random_int(1, 100);
auto uniform_double = uniform_random_double(0.0, 1.0);
```

### Container Utilities (`container_kits.hpp`)

Container utilities provide safe operations on standard containers.

**Key Features:**
- Content checking with predicates
- Safe value extraction from containers
- Convenient map operations
- Container slicing with step support
- Safe container modification
- Container transformation utilities

#### Content Checking

```cpp
std::vector<int> numbers = {1, 2, 3, 4, 5};
bool found = contains(numbers, 3);  // true

std::vector<int> search = {2, 4, 6};
bool found_any = contains_any(numbers, search);  // true

bool found_even = contains_if(numbers, [](int x) { return x % 2 == 0; });  // true
```

#### Value Extraction

```cpp
std::vector<int> vec = {1, 2, 3, 4, 5};
auto value = pop_back_value(vec);  // Optional<int> containing 5

std::list<int> lst = {1, 2, 3};
auto front = pop_front_value(lst);  // Optional<int> containing 1
```

#### Map Operations

```cpp
std::unordered_map<std::string, int> map;
auto& value = get_or_insert(map, "key", 42);  // Insert if not exists
auto& default_val = get_or_insert_default(map, "key", 0);  // Insert with default

// Extract keys or values
std::unordered_set<std::string> keys = get_map_keys(map);
std::unordered_set<int> values = get_map_values(map);
```

#### Container Slicing

```cpp
std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9};
auto slice = container_slice(data, 2, 7, 2);  // {3, 5, 7}
```

#### Container Modification

```cpp
std::vector<int> numbers = {1, 2, 3, 4, 5, 6};
erase_if(numbers, [](int x) { return x % 2 == 0; });  // {1, 3, 5}

std::vector<int> more = {7, 8, 9};
auto merged = merge(numbers, more);  // {1, 3, 5, 7, 8, 9}
```

#### Container Transformation

```cpp
std::vector<int> numbers = {1, 2, 3, 4, 5};
auto squared = transform_all_to_vector<int>(numbers, [](int x) { return x * x; });
// {1, 4, 9, 16, 25}
```

### Common Concepts (`common_concepts.hpp`)

Common concepts provide type constraints for template programming.

**Key Features:**
- Stringable concept for string conversion
- Type constraint concepts (OneOf, UnaryPred, BinaryPred)
- Callable concepts with return type specification
- Range concepts for container operations
- Compile-time type safety for templates

#### Stringable Concept

```cpp
template <Stringable T>
void print(const T& value) {
    std::cout << convert_stringable_to_string(value);
}

// Works with int, double, std::string, const char*, etc.
print(42);           // "42"
print(3.14);         // "3.14"
print("hello");      // "hello"
```

#### Type Constraint Concepts

```cpp
template<OneOf<int, float, double> T>
void process(T value) {
    // Only accepts int, float, or double
}

template<UnaryPred<int> F>
void filter(F predicate) {
    // F must be a callable that takes const int& and returns bool
}

template<InvocableReturns<size_t, std::string> F>
void get_length(F func) {
    // F must be callable with std::string and return size_t
}
```

#### Range Concepts

```cpp
template<RangeLike R>
void process_range(const R& range) {
    // R must have begin() and end()
}

template<RangeLikeElemTypeSpecified<R, int>>
void process_int_range(const R& range) {
    // R must be a range of int values
}
```

### Numerics Utilities (`numerics_kits.hpp`)

Numerics utilities provide compile-time type selection and overflow-checked arithmetic operations.

**Key Features:**
- Compile-time type selection for optimal memory usage
- Overflow-checked arithmetic operations
- Platform-specific optimizations using intrinsics
- Safe numeric operations with optional return types

#### Compile-time Type Selection

```cpp
// Select smallest unsigned type that can hold the value
using Type1 = select_by_max_unsigned_t<255>;   // uint8_t
using Type2 = select_by_max_unsigned_t<256>;   // uint16_t
using Type3 = select_by_max_unsigned_t<65535>; // uint16_t
using Type4 = select_by_max_unsigned_t<65536>; // uint32_t

// Select smallest type that can hold the range
using SignedType = select_by_range_t<-100, 100>;    // int8_t
using UnsignedType = select_by_range_t<0, 255>;     // uint8_t
using LargeType = select_by_range_t<0, 100000>;    // uint32_t

// Select type for single value
using ValueType = select_by_value_t<42>;  // int8_t
```

#### Overflow-Checked Arithmetic

```cpp
// Safe addition with overflow detection
auto result1 = checked_add<uint8_t>(200, 50);  // nullopt (overflow)
auto result2 = checked_add<uint8_t>(200, 55);  // 255
auto result3 = checked_add<int>(-100, 50);     // -50

// Safe subtraction with underflow detection
auto result1 = checked_sub<uint8_t>(100, 150); // nullopt (underflow)
auto result2 = checked_sub<uint8_t>(200, 50);  // 150
auto result3 = checked_sub<int>(-100, 50);     // -150

// Safe multiplication with overflow detection
auto result1 = checked_mul<uint8_t>(100, 3);   // nullopt (overflow: 300 > 255)
auto result2 = checked_mul<uint8_t>(50, 4);    // 200
auto result3 = checked_mul<int>(-100, 2);     // -200

// Check for success
if (auto sum = checked_add<int>(INT_MAX, 1)) {
    // Safe to use the result
    int value = *sum;
} else {
    // Handle overflow
    std::cout << "Addition overflowed!" << std::endl;
}
```

#### Platform-Specific Optimizations

```cpp
// The library automatically uses platform-specific intrinsics:
// - MSVC: _addcarry_u64, _subborrow_u64, _umul128
// - GCC/Clang: __builtin_add_overflow, __builtin_sub_overflow, __builtin_mul_overflow
// - Smaller types are promoted to larger types for safe calculation
```

### RAII Utilities (`raii_kits.hpp`)

RAII utilities provide scope guards for automatic resource management and cleanup.

**Key Features:**
- Automatic cleanup on scope exit
- Success/failure conditional execution
- Dismissable guards for flexible control
- Exception-safe resource management

#### Basic Scope Guard

```cpp
{
    FILE* file = fopen("data.txt", "r");
    auto guard = generate_scope_guard([file]() {
        fclose(file);
        std::cout << "File closed automatically" << std::endl;
    });
    
    // Use file...
    // File will be closed automatically when leaving scope
}
```

#### Success-Only Scope Guard

```cpp
{
    std::vector<int> data = {1, 2, 3};
    
    auto guard = generate_scope_guard_success([&data]() {
        // This only runs if no exception is thrown
        std::cout << "Operation succeeded, saving data..." << std::endl;
        save_to_database(data);
    });
    
    // Process data...
    process_data(data);
    
    // If no exception, the guard will save the data
}
```

#### Failure-Only Scope Guard

```cpp
{
    auto guard = generate_scope_guard_failed([]() {
        // This only runs if an exception is thrown
        std::cout << "Operation failed, rolling back..." << std::endl;
        rollback_transaction();
    });
    
    // Perform transaction...
    update_database();
    
    // If exception thrown, rollback happens automatically
}
```

#### Dismissing Scope Guards

```cpp
{
    auto guard = generate_scope_guard([]() {
        std::cout << "This won't run" << std::endl;
    });
    
    // Do some work...
    if (success_condition) {
        guard.dismiss();  // Prevent the guard from running
    }
}
```

#### Practical Examples

```cpp
// File handling with automatic cleanup
void process_file(const std::string& path) {
    FILE* file = fopen(path.c_str(), "r");
    if (!file) throw std::runtime_error("Cannot open file");
    
    auto cleanup = generate_scope_guard([file]() { fclose(file); });
    
    // Process file...
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file)) {
        // Process line
    }
    // File closed automatically
}

// Transaction management
void update_records() {
    begin_transaction();
    
    auto rollback = generate_scope_guard_failed([]() {
        rollback_transaction();
    });
    
    auto commit = generate_scope_guard_success([]() {
        commit_transaction();
    });
    
    // Perform updates...
    update_record(1, "new value");
    update_record(2, "another value");
    
    // Transaction committed or rolled back automatically
}
```

### String Conversion (`string_converts.hpp`)

String conversion utilities provide convenient range-to-string conversion.

#### Range to String Conversion

```cpp
std::vector<int> numbers = {1, 2, 3, 4, 5};
auto str = convert_range_to_string(numbers);  // "[1, 2, 3, 4, 5]"

std::set<std::string> words = {"apple", "banana", "cherry"};
auto str = convert_range_to_string(words);  // "{apple, banana, cherry}"

std::map<std::string, int> map = {{"a", 1}, {"b", 2}};
auto str = convert_range_to_string(map);  // "{a: 1, b: 2}"
```

## API Reference

### String Functions

| Function | Description | Returns |
|----------|-------------|---------|
| `string_slice(s, pos, count)` | Get substring view | `std::string_view` |
| `prefix(s, n)` | Get first n characters | `std::string_view` |
| `suffix(s, n)` | Get last n characters | `std::string_view` |
| `remove_prefix(s, n)` | Remove first n characters | `std::string_view` |
| `remove_suffix(s, n)` | Remove last n characters | `std::string_view` |
| `trim(s)` | In-place trim whitespace | `std::string&` |
| `trim_view(s)` | View-based trim | `std::string_view` |
| `split(s, delim)` | Split by delimiter | `vector<string_view>` |
| `join(strings, delim)` | Join strings with delimiter | `std::string` |
| `contains(s, substr)` | Check if contains substring | `bool` |
| `replace_all(s, old, new)` | Replace all occurrences | `void` |
| `remove_all(s, substr)` | Remove all occurrences | `std::string&` |

### Span Functions

| Function | Description | Returns |
|----------|-------------|---------|
| `sub_span_safe(s, offset, count)` | Bounds-checked subspan | `optional<span>` |
| `convert_span_to_vector(s)` | Convert span to vector | `vector<T>` |
| `contains(s, value)` | Check if contains value | `bool` |
| `find_subspan(s, target)` | Find subspan within span | `span<T>` |

### I/O Classes and Functions

| Class/Function | Description |
|----------------|-------------|
| `FileMgr` | RAII file management class |
| `write_string_to_console(fmt, ...)` | Formatted console output |
| `read_a_line_from_console(fmt, ...)` | Interactive console input |
| `open_file(path)` | Safe file opening |

### Time Classes and Functions (`time_kits.hpp`)

| Class/Function | Description |
|----------------|-------------|
| `Timer` | Simple timing utility |
| `Stopwatch` | Pause/resume timing |
| `DateTime` | Date/time manipulation |
| `time_a_function(name, settings, f, ...)` | Function benchmarking |
| `sleep(seconds)` | Convenient sleep function |

### Random Functions (`random_kits.hpp`)

| Function | Description | Returns |
|----------|-------------|---------|
| `random_int(min, max)` | Random integer in range [min, max] | `int` |
| `random_double(min, max)` | Random double in range [min, max) | `double` |
| `random_long_long(min, max)` | Random long long in range | `long long` |
| `random_int32(min, max)` | Random 32-bit integer | `int32_t` |
| `random_int64(min, max)` | Random 64-bit integer | `int64_t` |
| `uniform_random_int(min, max)` | Uniform random integer | `int` |
| `uniform_random_double(min, max)` | Uniform random double | `double` |

### Container Functions (`container_kits.hpp`)

| Function | Description | Returns |
|----------|-------------|---------|
| `contains(container, value)` | Check if contains element | `bool` |
| `contains_any(container, values)` | Check if contains any of values | `bool` |
| `contains_if(container, pred)` | Check if contains element matching predicate | `bool` |
| `pop_back_value(vector)` | Remove and return last element | `Optional<T>` |
| `pop_front_value(list)` | Remove and return first element | `Optional<T>` |
| `get_or_insert(map, key, value)` | Get or insert value | `T&` |
| `get_or_insert_default(map, key, default)` | Get or insert with default | `T&` |
| `get_map_keys(map)` | Extract keys from map | `unordered_set<Key>` |
| `get_map_values(map)` | Extract values from map | `unordered_set<Value>` |
| `container_slice(c, start, end, step)` | Slice container with step | `vector<T>` |
| `erase_if(container, pred)` | Remove elements matching predicate | `void` |
| `merge(a, b)` | Merge two containers | `vector<T>` |
| `transform_all_to_vector<T>(c, f)` | Transform elements | `vector<T>` |

### Numerics Functions and Types (`numerics_kits.hpp`)

| Function/Type | Description | Returns |
|---------------|-------------|---------|
| `checked_add<T>(a, b)` | Overflow-checked addition | `Optional<T>` |
| `checked_sub<T>(a, b)` | Overflow-checked subtraction | `Optional<T>` |
| `checked_mul<T>(a, b)` | Overflow-checked multiplication | `Optional<T>` |
| `select_by_max_unsigned_t<MAX>` | Smallest unsigned type for max value | Type alias |
| `select_by_range_t<MIN, MAX>` | Smallest type for range | Type alias |
| `select_by_value_t<VALUE>` | Smallest type for value | Type alias |

### RAII Classes and Functions (`raii_kits.hpp`)

| Class/Function | Description | Returns |
|----------------|-------------|---------|
| `generate_scope_guard(func)` | Create cleanup action on scope exit | `ScopeGuard` |
| `generate_scope_guard_success(func)` | Cleanup only if no exception | `ScopeGuard` |
| `generate_scope_guard_failed(func)` | Cleanup only if exception | `ScopeGuard` |
| `ScopeGuard::dismiss()` | Prevent cleanup from running | `void` |

### String Conversion Functions (`string_converts.hpp`)

| Function | Description | Returns |
|----------|-------------|---------|
| `convert_range_to_string(range)` | Convert container to string | `std::string` |
| `convert_to_bytes(value)` | Convert object to byte span | `span<const std::byte>` |
| `convert_to_hex(data)` | Convert to hexadecimal string | `std::string` |
| `try_convert_string_to_numerics<T>(str)` | Safe string to number conversion | `Optional<T>` |
| `is_numeric(str)` | Check if string is numeric | `bool` |

## Examples

### Complete File Processing Example

```cpp
#include "io_kits.hpp"
#include "string_kits.hpp"
#include "time_kits.hpp"

using namespace zuc;

int main() {
    try {
        // Open file for reading
        FileMgr input("data.txt", std::ios::in);
        auto lines = input.read_all();
        
        // Process each line
        std::vector<std::string> processed;
        for (const auto& line : lines) {
            auto trimmed = trim_view(line);
            if (!trimmed.empty() && !trimmed.starts_with("#")) {
                processed.push_back(std::string(trimmed));
            }
        }
        
        // Write processed data
        FileMgr output("processed.txt", std::ios::out);
        for (const auto& line : processed) {
            output.write("{}\n", line);
        }
        
        write_a_line_to_console("Processed {} lines", processed.size());
        
    } catch (const FileException& e) {
        write_a_line_to_console("Error: {}", e.what());
        return 1;
    }
    
    return 0;
}
```

### Performance Benchmarking Example

```cpp
#include "time_kits.hpp"
#include "vector"
#include "algorithm"

using namespace zuc;

void sort_vector() {
    std::vector<int> data(1000000);
    for (auto& val : data) {
        val = random_int(1, 1000000);
    }
    std::sort(data.begin(), data.end());
}

int main() {
    TimingSetting settings{
        .print_after_each_run = true,
        .print_max_time = true,
        .print_min_time = true,
        .print_average_time = true,
        .times_to_run = 10,
        .decimal_places = 2,
        .prec = TimePrecision::millisecond_precs
    };
    
    time_a_function("sort_vector", settings, sort_vector);
    
    return 0;
}
```

## Requirements

- **C++20** compatible compiler
- **Standard library** with C++20 support
- **DateTime class** requires full C++20 chrono implementation

### Compiler Support

- **GCC 13+**: Full support
- **Clang 19+**: Full support (except DateTime without `ZUC_DROP_CHRONO_TIME_ZONE`, but it passed on my debian13's clang19. If you download an official llvm, the minium version is 22(with full chrono support))
- **MSVC 2022+**: Full support

## Best Practices

### Performance

1. **Use views over copies**: Prefer `string_view` and `span` for zero-allocation operations
2. **Leverage noexcept**: Functions are noexcept where no dynamic allocation occurs
3. **Batch operations**: Use `join()` and `split()` for efficient bulk operations

### Safety

1. **RAII management**: Always use `FileMgr` for automatic resource cleanup
2. **Bounds checking**: Use `sub_span_safe()` instead of manual bounds checking
3. **Exception handling**: Catch `FileException`, `ReadLineException`, `WriteLineException`

### Code Style

1. **Use concepts**: Leverage type constraints for compile-time safety
2. **Prefer in-place operations**: Use `trim()` over `get_trimmed()` when possible
3. **Chain operations**: Combine view operations for efficiency

### Common Patterns

```cpp
// Efficient string processing pipeline
auto result = trim_view(slice(raw_data, 10, 50));
if (contains(result, "important")) {
    // Process without allocations
}

// Safe file processing
try {
    FileMgr file("data.txt");
    for (const auto& line : file.read_all()) {
        // Process line
    }
} catch (const FileException& e) {
    // Handle error
}

// Performance measurement
Timer timer;
timer.start();
// ... work ...
auto duration = timer.get_duration_seconds();
```

---

For more information, see the [README.md](README.md) and example files in the repository.