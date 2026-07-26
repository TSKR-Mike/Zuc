/**
 * @file io_kits.hpp
 * @brief I/O utilities for C++20
 * @date 2026-07-06
 * @copyright Copyright (c) 2026
 */
#pragma once

#include <filesystem>
#include <format>
#include <fstream>
#include <ios>
#include <iostream>
#include <optional>
#include <ostream>
#include <span>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "common_concepts.hpp"
#include "string_kits.hpp"
#include "container_kits.hpp"

#define _CRT_SECURE_NO_WARNINGS  // To ignore fopen warnings on windows

namespace zuc {

// ---- Exception Classes ----

/**
 * @class FileException
 * @brief Exception thrown for file-related errors
 * @note Used when file operations fail due to opening, reading, or writing issues
 */
class FileException : public std::runtime_error {
   public:
    explicit FileException(const std::string& message)
        : std::runtime_error(message) {}
};

/**
 * @class ReadLineException
 * @brief Exception thrown when line reading operations fail
 */
class ReadLineException : public std::runtime_error {
   public:
    explicit ReadLineException(const std::string& message)
        : std::runtime_error(message) {}
};

/**
 * @class WriteLineException
 * @brief Exception thrown when line writing operations fail
 */
class WriteLineException : public std::runtime_error {
   public:
    explicit WriteLineException(const std::string& message)
        : std::runtime_error(message) {}
};
/**
 * @enum ReadLineStatus
 * @brief Status codes for line reading operations
 * @var EndOfFile Successfully reached end of file
 * @var Failed     An error occurred during reading
 * @var Success    Successfully read a line
 */
enum class ReadLineStatus { EndOfFile, Failed, Success };

// ---- FileMgr class ----

/**
 * @class FileMgr
 * @brief RAII wrapper for file I/O operations with automatic resource management
 * @note Provides automatic file closing, move semantics, formatted writing, line-by-line reading, and pointer management
 * @note Copy operations are disabled; use move semantics for transferring ownership
 * @example
 * try {
 *     FileMgr file("data.txt", std::ios::out);
 *     file.write("Hello, World!\n");
 *     file.write("Number: {}\n", 42);
 * } catch (const FileException& e) {
 *     std::cerr << "Error: " << e.what() << std::endl;
 * }
 */
class FileMgr {
   private:
    std::fstream file_;                    // Underlying file stream
    std::string file_name_;               // Path/name of the opened file
    bool file_opened_successfully_ = false; // Track if file is valid and open
    std::ios::openmode mode_;              // File open mode (read/write/etc)

   protected:
   public:
    // ---- Constructors ----
    
    /**
     * @brief Construct FileMgr from C-style string path
     * @param path File path as C-string
     * @param mode File open mode (default: read+write)
     * @throws FileException if file cannot be opened
     */
    FileMgr(const char* path,
            std::ios::openmode mode = std::ios::in | std::ios::out) {
        file_.open(path, mode);
        if (!file_.is_open()) {
            throw FileException(
                std::format("Failed to open the file {}", path));
        } else {
            file_.clear();
            file_opened_successfully_ = true;
            file_name_ = path;
            mode_ = mode;
        }
    }

    /**
     * @brief Construct FileMgr from std::string path
     * @param path File path as std::string
     * @param mode File open mode (default: read+write)
     * @throws FileException if file cannot be opened
     */
    FileMgr(const std::string& path,
            std::ios::openmode mode = std::ios::in | std::ios::out) {
        file_.open(path, mode);
        if (!file_.is_open()) {
            throw FileException(
                std::format("Failed to open the file {}", path));
        } else {
            file_.clear();
            file_opened_successfully_ = true;
            file_name_ = path;
            mode_ = mode;
        }
    }

    /**
     * @brief Construct FileMgr from filesystem path
     * @param path File path as std::filesystem::path
     * @param mode File open mode (default: read+write)
     * @throws FileException if file cannot be opened
     */
    FileMgr(const std::filesystem::path& path,
            std::ios::openmode mode = std::ios::in | std::ios::out) {
        file_.open(path, mode);
        if (!file_.is_open()) {
            throw FileException(
                std::format("Failed to open the file {}", path.string()));
        } else {
            file_.clear();
            file_opened_successfully_ = true;
            file_name_ = path.string();
            mode_ = mode;
        }
    }

    /**
     * @brief Destructor - automatically closes the file
     * @note Ensures proper resource cleanup even if exceptions occur
     */
    ~FileMgr() {
    try {
        if (file_.is_open()) {
            file_.flush();  // Ensure all data is written
            file_.close();  // Close the file
        }
    } catch (...) {
        // Ignore exceptions in destructor
    }
}

    // Copy operations disabled - files cannot be safely copied
    FileMgr(const FileMgr&) = delete;
    FileMgr& operator=(const FileMgr&) = delete;

    /**
     * @brief Move constructor - transfers ownership of file handle
     * @param f FileMgr to move from
     * @note The source object is left in a valid but empty state
     */
    FileMgr(FileMgr&& f) noexcept {
        file_ = std::move(f.file_);
        file_name_ = std::move(f.file_name_);
        mode_ = f.mode_;
        file_opened_successfully_ = f.file_opened_successfully_;
        f.file_.close();
        f.file_name_.clear();
        f.mode_ = std::ios::in | std::ios::out;
        f.file_opened_successfully_ = false;
    };

    /**
     * @brief Move assignment operator - transfers ownership of file handle
     * @param f FileMgr to move from
     * @return Reference to this FileMgr
     * @note Closes current file before taking ownership of the source file
     */
    FileMgr& operator=(FileMgr&& f) noexcept {
        close();
        file_ = std::move(f.file_);
        file_name_ = std::move(f.file_name_);
        mode_ = f.mode_;
        file_opened_successfully_ = f.file_opened_successfully_;
        f.file_.close();
        f.file_name_.clear();
        f.mode_ = std::ios::in | std::ios::out;
        f.file_opened_successfully_ = false;
        return *this;
    };

    // ---- Public methods ----
    
    /**
     * @brief Check if file is successfully opened and valid
     * @return true if file is open and valid, false otherwise
     */
    bool is_valid() const { return file_opened_successfully_; }

    /**
     * @brief Close the file if it's open
     * @note Safe to call multiple times. Subsequent calls have no effect
     */
    void close() {
        if (file_opened_successfully_) {
            file_.close();
            file_opened_successfully_ = false;
        }
    }

    /**
     * @brief Read all lines from the file into a vector
     * @return Vector of strings containing each line
     * @throws FileException if file is not open or reading fails
     * @note Resets read pointer to beginning before reading. Handles both Unix (\n) and Windows (\r\n) line endings
     */
    std::vector<std::string> read_all() {
        if (!is_valid()) {
            throw FileException("File is not opened");
        }
        reset_read_pointer_to_front();
        ReadLineStatus status = ReadLineStatus::Success;
        std::vector<std::string> contents;
        std::optional<std::string> line;
        do {
            auto result = read_a_line();
            status = result.first;
            line = result.second;
            if (line.has_value()) {
                contents.push_back(line.value());
            }

        } while (status == ReadLineStatus::Success);
        if (status == ReadLineStatus::Failed) {
            throw FileException("Failed to read the file.");
        }
        return contents;
    }

    /**
     * @brief Read a single line from the file
     * @return Pair containing status and optional line content
     * @note Returns {Success, line} on successful read, {EndOfFile, nullopt} when end of file is reached, {Failed, nullopt} on read error
     * @note Handles Windows line endings (\r\n) by removing trailing \r
     */
    std::pair<ReadLineStatus, std::optional<std::string>> read_a_line() {
        if (!is_valid()) {
            return {ReadLineStatus::Failed, std::nullopt};
        }
        std::string line;
        std::getline(file_, line);
        
        if (file_.eof()) {
            file_.clear();
            // Reach the end of the file
            if (!line.empty()) {
                // It still read the final line, just hit eof in the end
                return {ReadLineStatus::Success, line};
            }
            // Empty line at EOF - don't count it as a valid line
            return {ReadLineStatus::EndOfFile, std::nullopt};
        } else if (file_.fail()) {
            // Something goes wrong
            return {ReadLineStatus::Failed, std::nullopt};
        }
        // Success
        return {ReadLineStatus::Success, line};
    }

    /**
     * @brief Write stringable content to file
     * @param content Content to write (must satisfy Stringable concept)
     * @throws FileException if file is not open
     * @note Automatically flushes after writing to ensure data is written
     */
    void write(const Stringable auto& content) {
        if (!is_valid()) {
            throw FileException("No file is opened");
        }
        std::string content_str = convert_stringable_to_string(content);
        file_ << content_str;
        file_ << std::flush;
    }

    /**
     * @brief Write multiple items to file, one per line
     * @param all_content Span of stringable items to write
     * @note Each item is written on its own line
     */
    template <Stringable T>
    void write_all(std::span<T> all_content) {
        if (!is_valid()) {
            return;
        }
        for (auto x : all_content) {
            write(x);
            write("\n");
        }
    }

    /**
     * @brief Write formatted content to file using std::format syntax
     * @param fmt Format string
     * @param args Arguments to format
     * @throws FileException if file is not open or formatting fails
     * @note Uses C++20 std::format for type-safe, flexible formatting
     */
    template <typename... Args>
    void write(const std::format_string<Args...> fmt, Args&&... args) {
        if (!is_valid()) {
            throw FileException("No file is opened");
        }
        try {
            std::string formatted_content =
                std::format(fmt, std::forward<Args>(args)...);
            write(formatted_content);
        } catch (const std::format_error& e) {
            throw FileException(std::format("Format error: {}", e.what()));
        }
    }

    /**
     * @brief Reset read pointer to the beginning of the file
     */
    void reset_read_pointer_to_front() {
        if (!is_valid()) {
            return;
        }
        file_.clear();
        file_.seekg(0, std::ios::beg);
    }

    /**
     * @brief Reset write pointer to the beginning of the file
     */
    void reset_write_pointer_to_front() {
        if (!is_valid()) {
            return;
        }
        file_.clear();
        file_.seekp(0, std::ios::beg);
    }

    /**
     * @brief Reset write pointer to the end of the file
     */
    void reset_write_pointer_to_last() {
        if (!is_valid()) {
            return;
        }
        file_.clear();
        file_.seekp(0, std::ios::end);
    }

    /**
     * @brief Get reference to underlying fstream
     * @return Reference to the internal std::fstream
     * @warning FOR DEBUG ONLY. DO NOT CLOSE THE STREAM RETURNED! THIS MAY LEAD TO SERIOUS ERRORS
     */
    std::fstream& get_stream() {
        return file_;
    }

    
};

// ---- Console I/O functions ----

/**
 * @brief Write formatted content to console
 * @tparam Args Variadic template for format arguments
 * @param fmt Format string using std::format syntax
 * @param args Arguments to format
 * @throws WriteLineException if formatting fails
 * @note Writes formatted output to stdout using C++20 std::format
 */
template <typename... Args>
inline void write_string_to_console(std::format_string<Args...> fmt,
                                    Args&&... args) {
    try {
        std::string formatted_content =
            std::format(fmt, std::forward<Args>(args)...);
        std::cout << formatted_content;
        std::cout << std::flush;
    } catch (const std::format_error& e) {
        throw WriteLineException(std::format("Format error: {}", e.what()));
    }
}

/**
 * @brief Write plain string to console
 * @param s String to write
 */
inline void write_string_to_console(const std::string& s) {
    write_string_to_console("{}", s);
}

/**
 * @brief Write formatted content to console followed by newline
 * @tparam Args Variadic template for format arguments
 * @param fmt_str Format string using std::format syntax
 * @param args Arguments to format
 * @note Writes formatted output to stdout with automatic newline termination
 */
template <typename... Args>
inline void write_a_line_to_console(const std::format_string<Args...>& fmt_str,
                                    Args&&... args) {
    write_string_to_console(fmt_str, std::forward<Args>(args)...);
    write_string_to_console("\n");
}

/**
 * @brief Write plain line to console (with newline)
 * @param line Line to write
 */
inline void write_a_line_to_console(const std::string& line) {
    write_string_to_console("{}", line);
    write_string_to_console("\n");
}

/**
 * @brief Read a line from console with optional prompt
 * @tparam Args Variadic template for prompt arguments
 * @param prompt_fmt_str Format string for the prompt
 * @param prompt_args Arguments for the prompt format string
 * @return String containing the user's input (without trailing newline)
 * @note Displays a formatted prompt and reads user input from stdin. Handles Windows line endings (\r\n) by removing the trailing \r
 */
template <typename... Args>
inline std::string read_a_line_from_console(
    std::format_string<Args...> prompt_fmt_str, Args&&... prompt_args) {
    // ----- Display prompt (same as before) -----
    try {
        std::string formatted_prompt =
            std::format(prompt_fmt_str, std::forward<Args>(prompt_args)...);
        write_string_to_console("{}", formatted_prompt);
    } catch (const std::format_error&) {
        write_string_to_console("{}", prompt_fmt_str.get());
    }
    
    std::cin.clear();
    std::string line;
    if (!std::getline(std::cin, line)) {
        if (std::cin.eof()) {
            std::cin.clear();
        } else {
            throw ReadLineException("Failed to read a line from console");
        }
    }
    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }
    return line;
}

/**
 * @brief Open file with error handling
 * @param path Path to the file
 * @return Optional FileMgr, or nullopt if opening fails
 * @note Provides a non-throwing alternative to FileMgr constructor. Errors are printed to console instead of being thrown
 */
inline std::optional<FileMgr> open_file(const std::string& path) {
    try {
        return FileMgr(path);
    } catch (const FileException& e) {
        write_string_to_console("{}", e.what());
        return std::nullopt;
    }
}

}  // namespace zuc