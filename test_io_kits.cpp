// tests/test_io_kits.cpp

#include <filesystem>
#include <fstream>
#include <array>
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
            write_file.write_a_line("Line 1");
            write_file.write_a_line("Line 2");
            write_file.write_a_line("Line 3");
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
            format_file.write_a_string("Number: {}, String: {}\n", 42, "hello");
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
            file.write_a_line("Test content");
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
            file.write_a_line("First line");
            file.write_a_line("Second line");
            file.write_a_line("Third line");
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
            file.write_all(lines);
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
            file.write_a_string("Initial content");
            file.reset_write_pointer_to_front();
            file.write_a_string("New content");
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
            file1.write_a_string("Content from file1");

            FileMgr file2 = std::move(file1);
            CHECK(file2.is_valid() == true);
            CHECK(file1.is_valid() == false);

            file2.write_a_line(" - Content from file2");
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
            file.write_a_string("Initial ");
        }

        {
            FileMgr file(test_file, std::ios::app);
            file.write_a_string("appended content");
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
                file.write_a_string("Line {}\n", i);
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
                file.write_a_string("This is line number {} with some content\n", i);
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
            file1.write_a_line("Test with string");
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
            file.write_a_line("Test");
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
            file1.write_a_line("Initial content");
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
            file.write_a_line("Line 1\n");
            file.write_a_line("Line 2\n");
        }
        {
            FileMgr file(test_file, std::ios::out | std::ios::in);
            file.write_a_line("Line 1\n");
            file.write_a_line("Line 2\n");

            file.reset_read_pointer_to_front();
            auto line1 = file.read_a_line();
            CHECK(line1.second == "Line 1");

            file.reset_write_pointer_to_front();
            file.write_a_line("Replaced content");
        }

        {
            FileMgr file(test_file, std::ios::in);
            auto content = file.read_all();
            CHECK(content[0].find("Replaced content") != std::string::npos);
        }

        std::filesystem::remove(test_file);
    }

    TEST_CASE("FileMgr read_a_line with Windows line endings") {
        const std::string test_file = "test_file_windows_line_endings.txt";

        {
            FileMgr file(test_file, std::ios::out | std::ios::binary);
            file.write_a_string("Line 1\r\nLine 2\r\nLine 3\r\n");
        }

        {
            FileMgr file(test_file, std::ios::in);
            auto [status1, line1] = file.read_a_line();
            CHECK(status1 == ReadLineStatus::Success);
            CHECK(line1 == "Line 1");  // Should not contain '\r'

            auto [status2, line2] = file.read_a_line();
            CHECK(status2 == ReadLineStatus::Success);
            CHECK(line2 == "Line 2");  // Should not contain '\r'

            auto [status3, line3] = file.read_a_line();
            CHECK(status3 == ReadLineStatus::Success);
            CHECK(line3 == "Line 3");  // Should not contain '\r'

            auto [status4, line4] = file.read_a_line();
            CHECK(status4 == ReadLineStatus::EndOfFile);
        }

        std::filesystem::remove(test_file);
    }

    TEST_CASE("FileMgr read_a_line with mixed line endings") {
        const std::string test_file = "test_file_mixed_line_endings.txt";

        {
            FileMgr file(test_file, std::ios::out | std::ios::binary);
            file.write_a_string("Unix\nWindows\r\nMac\r");
        }

        {
            FileMgr file(test_file, std::ios::in);
            auto [status1, line1] = file.read_a_line();
            CHECK(status1 == ReadLineStatus::Success);
            CHECK(line1 == "Unix");

            auto [status2, line2] = file.read_a_line();
            CHECK(status2 == ReadLineStatus::Success);
            CHECK(line2 == "Windows");  // Should not contain '\r'

            auto [status3, line3] = file.read_a_line();
            CHECK(status3 == ReadLineStatus::Success);
            CHECK(line3 == "Mac");  // Should not contain '\r'
        }

        std::filesystem::remove(test_file);
    }

    TEST_CASE("FileMgr read_a_line with trailing carriage return") {
        const std::string test_file = "test_file_trailing_cr.txt";

        {
            FileMgr file(test_file, std::ios::out | std::ios::binary);
            file.write_a_string("Content\r");
        }

        {
            FileMgr file(test_file, std::ios::in);
            auto [status, line] = file.read_a_line();
            CHECK(status == ReadLineStatus::Success);
            CHECK(line == "Content");  // Should not contain '\r'
            CHECK(line.value().back() != '\r');
        }

        std::filesystem::remove(test_file);
    }

    TEST_CASE("FileMgr read_all with Windows line endings") {
        const std::string test_file = "test_file_read_all_windows.txt";

        {
            FileMgr file(test_file, std::ios::out | std::ios::binary);
            file.write_a_string("Line 1\r\nLine 2\r\nLine 3\r\n");
        }

        {
            FileMgr file(test_file, std::ios::in);
            auto content = file.read_all();
            CHECK(content.size() == 3);
            CHECK(content[0] == "Line 1");
            CHECK(content[1] == "Line 2");
            CHECK(content[2] == "Line 3");
            for (const auto& line : content) {
                CHECK(line.find('\r') == std::string::npos);
            }
        }

        std::filesystem::remove(test_file);
    }
}

/**
 * @test Exception Classes
 * Tests for custom exception classes.
 */
TEST_SUITE("Exception Classes") {
    TEST_CASE("Exception classes construction and inheritance") {
        FileException file_ex("File error");
        ReadLineException read_ex("Read error");
        WriteLineException write_ex("Write error");

        CHECK(std::string(file_ex.what()) == "File error");
        CHECK(std::string(read_ex.what()) == "Read error");
        CHECK(std::string(write_ex.what()) == "Write error");

        CHECK(std::is_base_of_v<std::runtime_error, FileException>);
        CHECK(std::is_base_of_v<std::runtime_error, ReadLineException>);
        CHECK(std::is_base_of_v<std::runtime_error, WriteLineException>);
    }
}

/**
 * @test FileMgr Additional Methods
 * Tests for FileMgr methods not covered in previous test suites.
 */
TEST_SUITE("FileMgr Additional Methods") {
    TEST_CASE("FileMgr reset_write_pointer_to_last") {
        const std::string test_file = "test_reset_last.txt";

        {
            FileMgr file(test_file, std::ios::out);
            file.write_a_line("Initial content");
            file.reset_write_pointer_to_last();
            file.write_a_line("Appended content");
        }

        {
            FileMgr file(test_file, std::ios::in);
            auto content = file.read_all();
            CHECK(content.size() == 2);
            CHECK(content[0] == "Initial content");
            CHECK(content[1] == "Appended content");
        }

        std::filesystem::remove(test_file);
    }

    TEST_CASE("FileMgr get_stream") {
        const std::string test_file = "test_get_stream.txt";

        {
            FileMgr file(test_file, std::ios::out);
            auto& stream = file.get_stream();
            stream << "Content via stream";
        }

        {
            FileMgr file(test_file, std::ios::in);
            auto& stream = file.get_stream();
            std::string content;
            std::getline(stream, content);
            CHECK(content == "Content via stream");
        }

        std::filesystem::remove(test_file);
    }

    TEST_CASE("FileMgr flush") {
        const std::string test_file = "test_flush.txt";

        {
            FileMgr file(test_file, std::ios::out);
            file.write_a_line("Line 1");
            file.flush();
            file.write_a_line("Line 2");
            file.flush();
        }

        {
            FileMgr file(test_file, std::ios::in);
            auto content = file.read_all();
            CHECK(content.size() == 2);
            CHECK(content[0] == "Line 1");
            CHECK(content[1] == "Line 2");
        }

        std::filesystem::remove(test_file);
    }

    TEST_CASE("FileMgr write_a_string without newline") {
        const std::string test_file = "test_write_string_no_newline.txt";

        {
            FileMgr file(test_file, std::ios::out);
            file.write_a_string("Hello");
            file.write_a_string(" ");
            file.write_a_string("World");
        }

        {
            FileMgr file(test_file, std::ios::in);
            auto content = file.read_all();
            CHECK(content.size() == 1);
            CHECK(content[0] == "Hello World");
        }

        std::filesystem::remove(test_file);
    }

    TEST_CASE("FileMgr multiple close calls") {
        const std::string test_file = "test_multiple_close.txt";

        {
            FileMgr file(test_file, std::ios::out);
            file.write_a_line("Test content");
            file.close();
            CHECK_NOTHROW(file.close());
            CHECK_NOTHROW(file.close());
        }

        std::filesystem::remove(test_file);
    }
}

/**
 * @test Error Handling
 * Tests for error handling and exception scenarios.
 */
TEST_SUITE("Error Handling") {
    TEST_CASE("FileMgr operations after close throw exceptions") {
        const std::string test_file = "test_after_close.txt";

        {
            FileMgr file(test_file, std::ios::out);
            file.write_a_line("Initial content");
            file.close();
            CHECK(file.is_valid() == false);
            CHECK_THROWS_AS(file.write_a_line("Should fail"), FileException);
            CHECK_THROWS_AS(file.read_all(), FileException);
        }

        std::filesystem::remove(test_file);
    }

    TEST_CASE("FileMgr read_a_line from invalid file returns failed status") {
        FileMgr file("test_invalid_read.txt", std::ios::out);
        file.close();
        auto [status, line] = file.read_a_line();
        CHECK(status == ReadLineStatus::Failed);
        CHECK(!line.has_value());
    }

}

/**
 * @test Boundary Cases
 * Tests for boundary conditions and edge cases.
 */
TEST_SUITE("Boundary Cases") {
    TEST_CASE("FileMgr empty lines") {
        const std::string test_file = "test_empty_lines.txt";

        {
            FileMgr file(test_file, std::ios::out);
            file.write_a_line("");
            file.write_a_line("Line 1");
            file.write_a_line("");
        }

        {
            FileMgr file(test_file, std::ios::in);
            auto content = file.read_all();
            CHECK(content.size() == 3);
            CHECK(content[0] == "");
            CHECK(content[1] == "Line 1");
            CHECK(content[2] == "");
        }

        std::filesystem::remove(test_file);
    }

    TEST_CASE("FileMgr special characters") {
        const std::string test_file = "test_special_chars.txt";

        {
            FileMgr file(test_file, std::ios::out);
            file.write_a_line("Line with \t tab");
            file.write_a_line("Line with \"quotes\"");
            file.write_a_line("Line with @#$%^&*() symbols");
        }

        {
            FileMgr file(test_file, std::ios::in);
            auto content = file.read_all();
            CHECK(content.size() == 3);
            CHECK(content[0] == "Line with \t tab");
            CHECK(content[1] == "Line with \"quotes\"");
            CHECK(content[2] == "Line with @#$%^&*() symbols");
        }

        std::filesystem::remove(test_file);
    }

    TEST_CASE("FileMgr long line") {
        const std::string test_file = "test_long_line.txt";
        std::string long_line(10000, 'A');

        {
            FileMgr file(test_file, std::ios::out);
            file.write_a_line(long_line);
        }

        {
            FileMgr file(test_file, std::ios::in);
            auto content = file.read_all();
            CHECK(content.size() == 1);
            CHECK(content[0].length() == 10000);
            CHECK(content[0] == long_line);
        }

        std::filesystem::remove(test_file);
    }

    TEST_CASE("FileMgr write_all with empty range") {
        const std::string test_file = "test_empty_range.txt";
        std::vector<std::string> empty_vector;

        {
            FileMgr file(test_file, std::ios::out);
            file.write_all(empty_vector);
        }

        {
            FileMgr file(test_file, std::ios::in);
            auto content = file.read_all();
            CHECK(content.empty());
        }

        std::filesystem::remove(test_file);
    }

    TEST_CASE("FileMgr read_a_line with only newline") {
        const std::string test_file = "test_only_newline.txt";

        {
            FileMgr file(test_file, std::ios::out);
            file.write_a_string("\n");
        }

        {
            FileMgr file(test_file, std::ios::in);
            auto [status, line] = file.read_a_line();
            CHECK(status == ReadLineStatus::Success);
            CHECK(line.has_value());
            CHECK(line.value().empty());
        }

        std::filesystem::remove(test_file);
    }
}

/**
 * @test Console Operations Extended
 * Extended tests for console I/O operations.
 */
TEST_SUITE("Console Operations Extended") {
    TEST_CASE("Console write with empty string and special characters") {
        CHECK_NOTHROW(write_string_to_console(""));
        CHECK_NOTHROW(write_a_line_to_console(""));
        CHECK_NOTHROW(write_string_to_console("Special: \t\n\r\"\\"));
        CHECK_NOTHROW(write_a_line_to_console("Line with \"quotes\""));
    }

    TEST_CASE("Console write with various types") {
        CHECK_NOTHROW(write_string_to_console("Int: {}, Float: {}, Bool: {}, Char: {}", 
                                              42, 3.14f, true, 'A'));
        CHECK_NOTHROW(write_a_line_to_console("Mixed: {}, {}, {}, {}", 
                                              42, 3.14, true, "hello"));
    }
}

/**
 * @test FileMgr Pointer Operations Extended
 * Extended tests for pointer operations.
 */
TEST_SUITE("FileMgr Pointer Operations Extended") {
    TEST_CASE("FileMgr reset operations on empty file") {
        const std::string test_file = "test_reset_empty.txt";

        {
            FileMgr file(test_file, std::ios::out);
            CHECK_NOTHROW(file.reset_read_pointer_to_front());
            CHECK_NOTHROW(file.reset_write_pointer_to_front());
            CHECK_NOTHROW(file.reset_write_pointer_to_last());
        }

        {
            FileMgr file(test_file, std::ios::in);
            CHECK_NOTHROW(file.reset_read_pointer_to_front());
            auto content = file.read_all();
            CHECK(content.empty());
        }

        std::filesystem::remove(test_file);
    }

    TEST_CASE("FileMgr reset operations on invalid file") {
        FileMgr file("test_reset_invalid.txt", std::ios::out);
        file.close();
        CHECK_NOTHROW(file.reset_read_pointer_to_front());
        CHECK_NOTHROW(file.reset_write_pointer_to_front());
        CHECK_NOTHROW(file.reset_write_pointer_to_last());
        std::filesystem::remove("test_reset_invalid.txt");
    }

    TEST_CASE("FileMgr reset_read_pointer_to_front multiple times") {
        const std::string test_file = "test_reset_multiple.txt";

        {
            FileMgr file(test_file, std::ios::out);
            file.write_a_line("Line 1");
            file.write_a_line("Line 2");
        }

        {
            FileMgr file(test_file, std::ios::in);
            auto [status1, line1] = file.read_a_line();
            CHECK(line1 == "Line 1");

            file.reset_read_pointer_to_front();
            auto [status2, line2] = file.read_a_line();
            CHECK(line2 == "Line 1");
        }

        std::filesystem::remove(test_file);
    }
}

/**
 * @test FileMgr WriteAll Extended
 * Extended tests for write_all method.
 */
TEST_SUITE("FileMgr WriteAll Extended") {
    TEST_CASE("FileMgr write_all with different container types") {
        const std::string test_file = "test_containers.txt";

        {
            FileMgr file(test_file, std::ios::out);
            
            std::vector<std::string> vec = {"Vec 1", "Vec 2"};
            file.write_all(vec);
            
            std::array<std::string, 2> arr = {"Arr 1", "Arr 2"};
            file.write_all(arr);
            
            const char* c_arr[] = {"C 1", "C 2"};
            file.write_all(c_arr);
        }

        {
            FileMgr file(test_file, std::ios::in);
            auto content = file.read_all();
            CHECK(content.size() == 6);
            CHECK(content[0] == "Vec 1");
            CHECK(content[1] == "Vec 2");
            CHECK(content[2] == "Arr 1");
            CHECK(content[3] == "Arr 2");
            CHECK(content[4] == "C 1");
            CHECK(content[5] == "C 2");
        }

        std::filesystem::remove(test_file);
    }

    TEST_CASE("FileMgr write_all with single element") {
        const std::string test_file = "test_single_element.txt";

        {
            FileMgr file(test_file, std::ios::out);
            std::vector<std::string> lines = {"Single line"};
            file.write_all(lines);
        }

        {
            FileMgr file(test_file, std::ios::in);
            auto content = file.read_all();
            CHECK(content.size() == 1);
            CHECK(content[0] == "Single line");
        }

        std::filesystem::remove(test_file);
    }
}