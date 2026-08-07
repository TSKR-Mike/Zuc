// tests/test_raii_kits.cpp
#include "doctest.h"
#include <functional>
#include <ostream>
#include "include/raii_kits.hpp"
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>

using namespace zuc;

/**
 * @test ScopeGuard Basic Functionality
 * Tests for basic scope guard operations including execution and dismissal.
 */
TEST_SUITE("ScopeGuard Basic") {
    TEST_CASE("ScopeGuard executes on destruction") {
        bool executed = false;
        {
            auto guard = generate_scope_guard([&executed]() { executed = true; });
            CHECK(!executed);
        }
        CHECK(executed);
    }

    TEST_CASE("ScopeGuard with multiple statements") {
        int counter = 0;
        {
            auto guard = generate_scope_guard([&counter]() {
                counter++;
                counter *= 2;
            });
            CHECK(counter == 0);
        }
        CHECK(counter == 2);
    }

    TEST_CASE("ScopeGuard dismiss prevents execution") {
        bool executed = false;
        {
            auto guard = generate_scope_guard([&executed]() { executed = true; });
            CHECK(!executed);
            guard.dismiss();
            CHECK(!executed);
        }
        CHECK(!executed);
    }

    TEST_CASE("ScopeGuard with exception still executes") {
        bool executed = false;
        try {
            auto guard = generate_scope_guard([&executed]() { executed = true; });
            throw std::runtime_error("test exception");
        } catch (const std::runtime_error&) {
            CHECK(executed);
        }
        CHECK(executed);
    }

    TEST_CASE("ScopeGuard move construction") {
        bool executed = false;
        {
            auto guard1 = generate_scope_guard([&executed]() { executed = true; });
            auto guard2 = std::move(guard1);
            CHECK(!executed);
        }
        CHECK(executed);
    }


    TEST_CASE("ScopeGuard with lambda capturing variables") {
        std::vector<int> vec = {1, 2, 3};
        {
            auto guard = generate_scope_guard([&vec]() { vec.push_back(4); });
            CHECK(vec.size() == 3);
        }
        CHECK(vec.size() == 4);
        CHECK(vec[3] == 4);
    }

    TEST_CASE("ScopeGuard with function pointer") {
        static bool executed = false;
        executed = false;
        
        void (*func)() = []() { executed = true; };
        
        {
            auto guard = generate_scope_guard(func);
            CHECK(!executed);
        }
        CHECK(executed);
    }

    TEST_CASE("ScopeGuard exception safety") {
        CHECK_NOTHROW([]() {
            auto guard = generate_scope_guard([]() {});
            throw std::runtime_error("test");
        }());
    }

    TEST_CASE("ScopeGuard with noexcept function") {
        bool executed = false;
        {
            auto guard = generate_scope_guard([&executed]() noexcept { executed = true; });
            CHECK(!executed);
        }
        CHECK(executed);
    }
}

/**
 * @test ScopeGuardSuccess
 * Tests for success-only scope guard operations.
 */
TEST_SUITE("ScopeGuardSuccess") {
    TEST_CASE("executes on normal exit") {
        bool executed = false;
        {
            auto guard = generate_scope_guard_success([&executed]() { executed = true; });
            CHECK(!executed);
        }
        CHECK(executed);
    }

    TEST_CASE("does not execute on exception") {
        bool executed = false;
        try {
            auto guard = generate_scope_guard_success([&executed]() { executed = true; });
            throw std::runtime_error("test exception");
        } catch (const std::runtime_error&) {
            CHECK(!executed);
        }
        CHECK(!executed);
    }

    TEST_CASE("dismiss prevents execution") {
        bool executed = false;
        {
            auto guard = generate_scope_guard_success([&executed]() { executed = true; });
            guard.dismiss();
        }
        CHECK(!executed);
    }

    TEST_CASE("multiple guards in sequence") {
        int counter = 0;
        {
            auto guard1 = generate_scope_guard_success([&counter]() { counter += 1; });
            auto guard2 = generate_scope_guard_success([&counter]() { counter += 10; });
            auto guard3 = generate_scope_guard_success([&counter]() { counter += 100; });
        }
        CHECK(counter == 111);
    }

    TEST_CASE("with std::exception") {
        bool executed = false;
        try {
            auto guard = generate_scope_guard_success([&executed]() { executed = true; });
            throw std::exception();
        } catch (const std::exception&) {
            CHECK(!executed);
        }
        CHECK(!executed);
    }

    TEST_CASE("move construction transfers responsibility") {
        bool executed = false;
        {
            auto guard1 = generate_scope_guard_success([&executed]() { executed = true; });
            auto guard2 = std::move(guard1);
        }
        CHECK(executed);
    }

    TEST_CASE("with nested scope") {
        bool outer_executed = false;
        bool inner_executed = false;
        
        {
            auto outer_guard = generate_scope_guard_success([&outer_executed]() { outer_executed = true; });
            {
                auto inner_guard = generate_scope_guard_success([&inner_executed]() { inner_executed = true; });
            }
            CHECK(inner_executed);
            CHECK(!outer_executed);
        }
        CHECK(outer_executed);
    }
}

/**
 * @test ScopeGuardFailed
 * Tests for failure-only scope guard operations.
 */
TEST_SUITE("ScopeGuardFailed") {
    TEST_CASE("does not execute on normal exit") {
        bool executed = false;
        {
            auto guard = generate_scope_guard_failed([&executed]() { executed = true; });
        }
        CHECK(!executed);
    }

    TEST_CASE("executes on exception") {
        bool executed = false;
        try {
            auto guard = generate_scope_guard_failed([&executed]() { executed = true; });
            throw std::runtime_error("test exception");
        } catch (const std::runtime_error&) {
            CHECK(executed);
        }
        CHECK(executed);
    }

    TEST_CASE("dismiss prevents execution") {
        bool executed = false;
        try {
            auto guard = generate_scope_guard_failed([&executed]() { executed = true; });
            guard.dismiss();
            throw std::runtime_error("test exception");
        } catch (const std::runtime_error&) {
            CHECK(!executed);
        }
        CHECK(!executed);
    }

    TEST_CASE("with different exception types") {
        bool executed = false;
        
        try {
            auto guard = generate_scope_guard_failed([&executed]() { executed = true; });
            throw std::logic_error("logic error");
        } catch (const std::logic_error&) {
            CHECK(executed);
        }
        
        executed = false;
        try {
            auto guard = generate_scope_guard_failed([&executed]() { executed = true; });
            throw std::invalid_argument("invalid argument");
        } catch (const std::invalid_argument&) {
            CHECK(executed);
        }
    }

    TEST_CASE("multiple guards with exception") {
        int counter = 0;
        try {
            auto guard1 = generate_scope_guard_failed([&counter]() { counter += 1; });
            auto guard2 = generate_scope_guard_failed([&counter]() { counter += 10; });
            throw std::runtime_error("test");
        } catch (const std::runtime_error&) {
            CHECK(counter == 11);
        }
    }

    TEST_CASE("mixed guard types with exception") {
        bool success_executed = false;
        bool failed_executed = false;
        
        try {
            auto success_guard = generate_scope_guard_success([&success_executed]() { success_executed = true; });
            auto failed_guard = generate_scope_guard_failed([&failed_executed]() { failed_executed = true; });
            throw std::runtime_error("test");
        } catch (const std::runtime_error&) {
            CHECK(!success_executed);
            CHECK(failed_executed);
        }
    }

    TEST_CASE("move construction with exception") {
        bool executed = false;
        try {
            auto guard1 = generate_scope_guard_failed([&executed]() { executed = true; });
            auto guard2 = std::move(guard1);
            throw std::runtime_error("test");
        } catch (const std::runtime_error&) {
            CHECK(executed);
        }
    }
}

/**
 * @test Practical Use Cases
 * Tests for real-world usage scenarios.
 */
TEST_SUITE("Practical Use Cases") {
    TEST_CASE("resource management with file simulation") {
        bool file_closed = false;
        bool file_deleted = false;
        
        {
            auto close_guard = generate_scope_guard([&file_closed]() { file_closed = true; });
            auto delete_guard = generate_scope_guard_failed([&file_deleted]() { file_deleted = true; });
            
            // Simulate file operations
            // If no exception, file is closed but not deleted
        }
        
        CHECK(file_closed);
        CHECK(!file_deleted);
    }

    TEST_CASE("resource management with exception") {
        bool file_closed = false;
        bool file_deleted = false;
        
        try {
            auto close_guard = generate_scope_guard([&file_closed]() { file_closed = true; });
            auto delete_guard = generate_scope_guard_failed([&file_deleted]() { file_deleted = true; });
            
            // Simulate file operations that fail
            throw std::runtime_error("file operation failed");
        } catch (const std::runtime_error&) {
            CHECK(file_closed);
            CHECK(file_deleted);
        }
    }

    TEST_CASE("transaction commit/rollback") {
        bool committed = false;
        bool rolled_back = false;
        
        {
            auto commit_guard = generate_scope_guard_success([&committed]() { committed = true; });
            auto rollback_guard = generate_scope_guard_failed([&rolled_back]() { rolled_back = true; });
            
            // Simulate successful transaction
        }
        
        CHECK(committed);
        CHECK(!rolled_back);
    }

    TEST_CASE("transaction rollback on exception") {
        bool committed = false;
        bool rolled_back = false;
        
        try {
            auto commit_guard = generate_scope_guard_success([&committed]() { committed = true; });
            auto rollback_guard = generate_scope_guard_failed([&rolled_back]() { rolled_back = true; });
            
            // Simulate failed transaction
            throw std::runtime_error("transaction failed");
        } catch (const std::runtime_error&) {
            CHECK(!committed);
            CHECK(rolled_back);
        }
    }

    TEST_CASE("state restoration") {
        std::vector<int> vec = {1, 2, 3};
        auto original_size = vec.size();
        
        {
            auto restore_guard = generate_scope_guard([&vec, original_size]() {
                vec.resize(original_size);
            });
            
            vec.push_back(4);
            vec.push_back(5);
            CHECK(vec.size() == 5);
        }
        
        CHECK(vec.size() == original_size);
    }

    TEST_CASE("state restoration on exception") {
        std::vector<int> vec = {1, 2, 3};
        auto original_size = vec.size();
        
        try {
            auto restore_guard = generate_scope_guard([&vec, original_size]() {
                vec.resize(original_size);
            });
            
            vec.push_back(4);
            throw std::runtime_error("operation failed");
        } catch (const std::runtime_error&) {
            CHECK(vec.size() == original_size);
        }
    }

    TEST_CASE("counter increment/decrement") {
        int counter = 0;
        
        {
            auto increment_guard = generate_scope_guard([&counter]() { counter++; });
            auto decrement_guard = generate_scope_guard([&counter]() { counter--; });
            
            counter += 10;
        }
        
        CHECK(counter == 10);
    }

    TEST_CASE("logging with scope exit") {
        std::vector<std::string> log;
        
        {
            auto entry_guard = generate_scope_guard([&log]() { log.push_back("entering scope"); });
            auto exit_guard = generate_scope_guard([&log]() { log.push_back("exiting scope"); });
            
            log.push_back("inside scope");
        }
        
        CHECK(log.size() == 3);
        CHECK(log[0] == "entering scope");
        CHECK(log[1] == "inside scope");
        CHECK(log[2] == "exiting scope");
    }
}

/**
 * @test Edge Cases and Boundary Conditions
 * Tests for unusual scenarios and edge cases.
 */
TEST_SUITE("Edge Cases") {
    TEST_CASE("empty lambda function") {
        CHECK_NOTHROW([]() {
            auto guard = generate_scope_guard([]() {});
        }());
    }

    TEST_CASE("lambda throwing exception") {
        bool executed = false;
        CHECK_THROWS([&executed]() {
            auto guard = generate_scope_guard([&executed]() {
                executed = true;
                throw std::runtime_error("cleanup failed");
            });
        }());
        CHECK(executed);
    }

    TEST_CASE("nested scope guards") {
        int counter = 0;
        
        {
            auto outer1 = generate_scope_guard([&counter]() { counter += 1; });
            {
                auto inner1 = generate_scope_guard([&counter]() { counter += 10; });
                {
                    auto inner2 = generate_scope_guard([&counter]() { counter += 100; });
                }
                CHECK(counter == 100);
            }
            CHECK(counter == 110);
        }
        CHECK(counter == 111);
    }

    TEST_CASE("scope guard with large captured data") {
        std::vector<int> large_data(1000, 42);
        bool executed = false;
        
        {
            auto guard = generate_scope_guard([&large_data, &executed]() {
                executed = true;
                CHECK(large_data.size() == 1000);
            });
        }
        
        CHECK(executed);
    }

    TEST_CASE("scope guard with move-only types") {
        auto ptr = std::make_unique<int>(42);
        bool executed = false;
        
        {
            auto guard = generate_scope_guard([&ptr, &executed]() {
                executed = true;
                CHECK(ptr != nullptr);
                CHECK(*ptr == 42);
            });
        }
        
        CHECK(executed);
    }

    TEST_CASE("dismiss called multiple times") {
        bool executed = false;
        {
            auto guard = generate_scope_guard([&executed]() { executed = true; });
            guard.dismiss();
            CHECK_NOTHROW(guard.dismiss());
        }
        CHECK(!executed);
    }

    TEST_CASE("scope guard with const lambda") {
        bool executed = false;
        auto lambda = [&executed]() { executed = true; };
        
        {
            auto guard = generate_scope_guard(lambda);
        }
        
        CHECK(executed);
    }

    TEST_CASE("scope guard lifetime with early return") {
        bool executed = false;
        
        auto func = [&executed]() -> int {
            auto guard = generate_scope_guard([&executed]() { executed = true; });
            return 42;
        };
        
        int result = func();
        CHECK(result == 42);
        CHECK(executed);
    }

    TEST_CASE("scope guard with std::function") {
        bool executed = false;
        std::function<void()> func = [&executed]() { executed = true; };
        
        {
            auto guard = generate_scope_guard(func);
        }
        
        CHECK(executed);
    }
}