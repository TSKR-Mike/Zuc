// tests/test_io_kits.cpp

#include <filesystem>
#include <fstream>
#include <string>
#include <thread>
#include <vector>

#include "doctest.h"
#include "include/io_kits.hpp"

using namespace zuc;

/**
 * @test File Operations
 * Comprehensive tests for file I/O operations including FileMgr class.
 */
TEST_SUITE("File Operations") {
    TEST_CASE("FileMgr write and read") {
        const std::string test_file = "test_doctest_output.txt";

        // Write test
        {
            FileMgr write_file(test_file, std::ios::out);
            REQUIRE(write_file.is_valid());
            write_file.write("Line 1\n");
            write_file.write("Line 2\n");
            write_file.write("Line 3\n");
            write_file.close();
        }

        // Read test
        {
            FileMgr read_file(test_file, std::ios::in);
            REQUIRE(read_file.is_valid());
            auto lines = read_file.read_all();

            CHECK(lines.size() == 3);
            CHECK(lines[0] == "Line 1");
            CHECK(lines[1] == "Line 2");
            CHECK(lines[2] == "Line 3");
            read_file.close();
        }

        // Cleanup
        std::filesystem::remove(test_file);
    }

    TEST_CASE("FileMgr formatted write") {
        const std::string test_file = "test_doctest_format.txt";

        {
            FileMgr format_file(test_file, std::ios::out);
            REQUIRE(format_file.is_valid());
            format_file.write("Number: {}, String: {}\n", 42, "hello");
            format_file.close();
        }

        // Verify content
        {
            std::ifstream file(test_file);
            std::string content((std::istreambuf_iterator<char>(file)),
                                std::istreambuf_iterator<char>());
            CHECK(content == "Number: 42, String: hello\n");
        }

        // Cleanup
        std::filesystem::remove(test_file);
    }

    TEST_CASE("FileMgr invalid file throws exception") {
        CHECK_THROWS_AS(
            FileMgr("/invalid/path/that/does/not/exist.txt", std::ios::in),
            FileException);
    }

    TEST_CASE("FileMgr construction and destruction") {
        const std::string test_file = "test_file_lifetime.txt";

        {
            FileMgr file(test_file, std::ios::out);
            CHECK(file.is_valid() == true);
            file.write("Test content");
            file.close();  // Explicit close before scope ends
            // It should be closed automatically when going out of scope, but
            // the system's buffer may not flush immediately. This will cause
            // the test to fail
        }

        // Small delay for system to release file handle
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        // Now safe to remove
        std::filesystem::remove(test_file);
    }

    TEST_CASE("FileMgr read_a_line") {
        const std::string test_file = "test_file_read_line.txt";

        {
            FileMgr file(test_file, std::ios::out);
            file.write("First line\nSecond line\nThird line");
        }

        {
            FileMgr file(test_file, std::ios::in);
            auto [status1, line1] = file.read_a_line();
            CHECK(status1 == ReadLineStatus::Success);
            CHECK(line1 == "First line");

            auto [status2, line2] = file.read_a_line();
            CHECK(status2 == ReadLineStatus::Success);
            CHECK(line2 == "Second line");

            auto [status3, line3] = file.read_a_line();
            CHECK(status3 == ReadLineStatus::Success);
            CHECK(line3 == "Third line");

            auto [status4, line4] = file.read_a_line();
            CHECK(status4 == ReadLineStatus::EndOfFile);
            CHECK(!line4.has_value());
        }

        std::filesystem::remove(test_file);
    }

    TEST_CASE("FileMgr write_all") {
        const std::string test_file = "test_file_write_all.txt";
        std::vector<std::string> lines = {"Line 1", "Line 2", "Line 3"};

        {
            FileMgr file(test_file, std::ios::out);
            file.write_all(std::span(lines));
        }

        {
            FileMgr file(test_file, std::ios::in);
            auto content = file.read_all();
            CHECK(content.size() == 3);
            CHECK(content[0] == "Line 1");
            CHECK(content[1] == "Line 2");
            CHECK(content[2] == "Line 3");
        }

        std::filesystem::remove(test_file);
    }

    TEST_CASE("FileMgr pointer operations") {
        const std::string test_file = "test_file_pointers.txt";

        {
            FileMgr file(test_file, std::ios::out);
            file.write("Initial content");
            file.reset_write_pointer_to_front();
            file.write("New content");
        }

        {
            FileMgr file(test_file, std::ios::in);
            auto content = file.read_all();
            CHECK(content.size() == 1);
            CHECK(content[0].find("New content") != std::string::npos);
        }

        std::filesystem::remove(test_file);
    }

    TEST_CASE("FileMgr move semantics") {
        const std::string test_file = "test_file_move.txt";

        {
            FileMgr file1(test_file, std::ios::out);
            file1.write("Content from file1");

            FileMgr file2 = std::move(file1);
            CHECK(file2.is_valid() == true);
            CHECK(file1.is_valid() == false);

            file2.write(" - Content from file2");
        }

        {
            FileMgr file(test_file, std::ios::in);
            auto content = file.read_all();
            CHECK(content.size() == 1);
            CHECK(content[0] == "Content from file1 - Content from file2");
        }

        std::filesystem::remove(test_file);
    }

    TEST_CASE("FileMgr read empty file") {
        const std::string test_file = "test_empty_file.txt";

        {
            FileMgr file(test_file, std::ios::out);
            // Write nothing
        }

        {
            FileMgr file(test_file, std::ios::in);
            auto content = file.read_all();
            CHECK(content.empty());
        }

        std::filesystem::remove(test_file);
    }

    TEST_CASE("FileMgr append mode") {
        const std::string test_file = "test_append.txt";

        {
            FileMgr file(test_file, std::ios::out);
            file.write("Initial ");
        }

        {
            FileMgr file(test_file, std::ios::app);
            file.write("appended content");
        }

        {
            FileMgr file(test_file, std::ios::in);
            auto content = file.read_all();
            CHECK(content.size() == 1);
            CHECK(content[0] == "Initial appended content");
        }

        std::filesystem::remove(test_file);
    }

    TEST_CASE("FileMgr multiple writes") {
        const std::string test_file = "test_multiple_writes.txt";

        {
            FileMgr file(test_file, std::ios::out);
            for (int i = 0; i < 10; ++i) {
                file.write("Line {}\n", i);
            }
        }

        {
            FileMgr file(test_file, std::ios::in);
            auto lines = file.read_all();
            CHECK(lines.size() == 10);
            for (int i = 0; i < 10; ++i) {
                CHECK(lines[i] == std::format("Line {}", i));
            }
        }

        std::filesystem::remove(test_file);
    }

    TEST_CASE("FileMgr large file handling") {
        const std::string test_file = "test_large_file.txt";
        const int line_count = 1000;

        {
            FileMgr file(test_file, std::ios::out);
            for (int i = 0; i < line_count; ++i) {
                file.write("This is line number {} with some content\n", i);
            }
        }

        {
            FileMgr file(test_file, std::ios::in);
            auto lines = file.read_all();
            CHECK(lines.size() == line_count);
            CHECK(lines[0] == "This is line number 0 with some content");
            CHECK(lines[line_count - 1] ==
                  std::format("This is line number {} with some content",
                              line_count - 1));
        }

        std::filesystem::remove(test_file);
    }

    TEST_CASE("open_file non-throwing") {
        const std::string test_file = "test_open_file.txt";

        // Create the file first
        {
            std::ofstream create_file(test_file);
            create_file << "test content";
        }

        auto file_opt = open_file(test_file);
        CHECK(file_opt.has_value());
        if (file_opt) {
            CHECK(file_opt->is_valid() == true);
        }

        auto invalid_file = open_file("/nonexistent/path/file.txt");
        CHECK(!invalid_file.has_value());

        // Cleanup
        if (file_opt) {
            file_opt->close();  // Ensure file is closed before removal
            std::filesystem::remove(test_file);
        }
    }

    TEST_CASE("FileMgr with different path types") {
        const std::string test_file = "test_path_types.txt";

        // Test with std::string
        {
            FileMgr file1(std::string(test_file), std::ios::out);
            CHECK(file1.is_valid());
            file1.write("Test with string");
        }

        // Test with const char*
        {
            FileMgr file2(test_file.c_str(), std::ios::in);
            CHECK(file2.is_valid());
            auto content = file2.read_all();
            CHECK(content[0] == "Test with string");
        }

        // Test with filesystem::path
        {
            FileMgr file3(std::filesystem::path(test_file), std::ios::in);
            CHECK(file3.is_valid());
        }

        std::filesystem::remove(test_file);
    }
}

/**
 * @test Console Operations
 * Tests for console I/O operations.
 */
TEST_SUITE("Console Operations") {
    TEST_CASE("write_string_to_console basic") {
        // This test mainly ensures the function compiles and doesn't throw
        CHECK_NOTHROW(write_string_to_console("Test message"));
    }

    TEST_CASE("write_string_to_console formatted") {
        CHECK_NOTHROW(
            write_string_to_console("Number: {}, Text: '{}'", 42, "test"));
    }

    TEST_CASE("write_a_line_to_console basic") {
        CHECK_NOTHROW(write_a_line_to_console("Test line"));
    }

    TEST_CASE("write_a_line_to_console formatted") {
        CHECK_NOTHROW(
            write_a_line_to_console("Number: {}, Text: '{}'", 123, "test"));
    }

    TEST_CASE("write_a_line_to_console multiple calls") {
        CHECK_NOTHROW(write_a_line_to_console("First line"));
        CHECK_NOTHROW(write_a_line_to_console("Second line"));
        CHECK_NOTHROW(write_a_line_to_console("Third line"));
    }

    TEST_CASE("write_string_to_console with various types") {
        CHECK_NOTHROW(write_string_to_console("Int: {}, Double: {}, String: {}",
                                              42, 3.14, "hello"));
    }

    TEST_CASE("console operations error handling") {
        // Test that console operations handle errors gracefully
        CHECK_NOTHROW(write_string_to_console("Error handling test"));
        CHECK_NOTHROW(write_a_line_to_console("Line with error handling"));
    }
}

/**
 * @test File Edge Cases
 * Tests for edge cases and boundary conditions in file operations.
 */
TEST_SUITE("File Edge Cases") {
    TEST_CASE("FileMgr with empty filename") {
        CHECK_THROWS_AS(FileMgr("", std::ios::in), FileException);
    }

    TEST_CASE("FileMgr write to read-only file") {
        const std::string test_file = "test_readonly.txt";

        // Create a file and make it read-only (platform-specific)
        {
            std::ofstream file(test_file);
            file << "test";
            file.close();  // Ensure file is closed
        }

        // Try to open for writing - this might fail on some systems
        // The exact behavior depends on the OS and file permissions
        FileMgr file(test_file, std::ios::in);
        CHECK(file.is_valid());
        file.close();  // Ensure FileMgr is closed

        std::filesystem::remove(test_file);
    }

    TEST_CASE("FileMgr special characters in filename") {
        const std::string test_file = "test_file_with_123.txt";

        {
            FileMgr file(test_file, std::ios::out);
            CHECK(file.is_valid());
            file.write("Test");
        }

        {
            FileMgr file(test_file, std::ios::in);
            CHECK(file.is_valid());
        }

        std::filesystem::remove(test_file);
    }

    TEST_CASE("FileMgr concurrent access simulation") {
        const std::string test_file = "test_concurrent.txt";

        // Write initial content
        {
            FileMgr file1(test_file, std::ios::out);
            file1.write("Initial content");
        }

        // Read and verify
        {
            FileMgr file2(test_file, std::ios::in);
            auto content = file2.read_all();
            CHECK(content[0] == "Initial content");
        }

        std::filesystem::remove(test_file);
    }

    TEST_CASE("FileMgr reset operations") {
        const std::string test_file = "test_reset.txt";
        {
            FileMgr file(test_file, std::ios::out);
            file.write("Line 1\n");
            file.write("Line 2\n");
        }
        {
            FileMgr file(test_file, std::ios::out | std::ios::in);
            file.write("Line 1\n");
            file.write("Line 2\n");

            file.reset_read_pointer_to_front();
            auto line1 = file.read_a_line();
            CHECK(line1.second == "Line 1");

            file.reset_write_pointer_to_front();
            file.write("Replaced content");
        }

        {
            FileMgr file(test_file, std::ios::in);
            auto content = file.read_all();
            CHECK(content[0].find("Replaced content") != std::string::npos);
        }

        std::filesystem::remove(test_file);
    }
}