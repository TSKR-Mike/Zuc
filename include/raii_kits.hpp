/**
 * @file raii_kits.hpp
 * @brief RAII utilities and scope guards for C++20
 * @date 2026-07-30
 * @copyright Copyright (c) 2026
 * @note Provides scope guard classes for automatic resource management and cleanup
 *       Supports unconditional, success-only, and failure-only cleanup scenarios
 */

#pragma once
#include <concepts>
#include <exception>
#include <utility>

namespace zuc {

/**
 * @class ScopeGuard
 * @brief RAII wrapper that executes a function when going out of scope
 * @tparam Func Callable type to execute on destruction
 * @note The function is always called when the ScopeGuard is destroyed, unless dismissed.
 *       Supports move semantics but not copy semantics. Exception-safe.
 * @example
 * {
 *     auto guard = generate_scope_guard([]() { std::cout << "Cleanup!" << std::endl; });
 *     // Do work...
 * } // "Cleanup!" is printed automatically
 */
template <typename Func>
    requires std::invocable<Func>
class ScopeGuard {
   public:
    explicit ScopeGuard(Func&& f)
        : func_(std::forward<Func>(f)), is_active_(true) {}

    ScopeGuard(ScopeGuard&& sg)
        : func_(std::move<Func>(sg.func_)), is_active_(sg.is_active_) {
        sg.is_active_ = false;
    }

    ScopeGuard& operator=(ScopeGuard&& sg) {
        func_ = std::move<Func>(sg.func_);
        is_active_ = sg.is_active_;
        sg.is_active_ = false;
        return *this;
    }
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
    ~ScopeGuard() noexcept(noexcept(func_())) {
        if (is_active_) {
            func_();
        }
    }

    /**
     * @brief Dismisses the scope guard, preventing the function from being called
     * @note Once dismissed, the function will not execute when the ScopeGuard is destroyed
     */
    /**
     * @brief Dismisses the scope guard, preventing the function from being called
     * @note Once dismissed, the function will not execute even on successful scope exit
     */
    /**
     * @brief Dismisses the scope guard, preventing the function from being called
     * @note Once dismissed, the function will not execute even on failure scope exit
     */
    void dismiss() { is_active_ = false; }

   protected:
   private:
    Func func_;
    bool is_active_ = true;
};

/**
 * @brief Factory function to create a ScopeGuard with automatic type deduction
 * @tparam Func Callable type to execute on destruction
 * @param func The callable to wrap
 * @return ScopeGuard that will execute func when going out of scope
 * @note Uses perfect forwarding and decay to handle callable types efficiently
 * @example
 * auto guard = generate_scope_guard([&]() { cleanup_resource(); });
 */
template <typename Func> requires std::invocable<Func>
[[nodiscard]] ScopeGuard<std::decay_t<Func>> generate_scope_guard(Func&& func) {
    return ScopeGuard<std::decay_t<Func>>(std::forward<Func>(func));
}

/**
 * @class ScopeGuardSuccess
 * @brief RAII wrapper that executes a function only when leaving scope successfully (no exceptions)
 * @tparam Func Callable type to execute on successful destruction
 * @note The function is only called if no exceptions are thrown during the scope.
 *       Uses std::uncaught_exceptions() to detect exception state changes.
 *       Useful for commit operations that should only happen on success.
 * @example
 * {
 *     auto guard = generate_scope_guard_success([]() { commit_transaction(); });
 *     // Do work that might throw...
 * } // commit_transaction() only called if no exceptions were thrown
 */
template <typename Func>
    requires std::invocable<Func>
class ScopeGuardSuccess {
   public:
    explicit ScopeGuardSuccess(Func&& f)
        : func_(std::forward<Func>(f)), is_active_(true), uncaught_count_(std::uncaught_exceptions()) {}

    ScopeGuardSuccess(ScopeGuardSuccess&& sg)
        : func_(std::move<Func>(sg.func_)), is_active_(sg.is_active_) {
        sg.is_active_ = false;
    }

    ScopeGuardSuccess& operator=(ScopeGuardSuccess&& sg) {
        func_ = std::move<Func>(sg.func_);
        is_active_ = sg.is_active_;
        sg.is_active_ = false;
        return *this;
    }
    ScopeGuardSuccess(const ScopeGuardSuccess&) = delete;
    ScopeGuardSuccess& operator=(const ScopeGuardSuccess&) = delete;
    ~ScopeGuardSuccess() noexcept(noexcept(func_())) {
        if (is_active_ && std::uncaught_exceptions() == uncaught_count_) {
            func_();
        }
    }

    void dismiss() { is_active_ = false; }

   protected:
   private:
    Func func_;
    bool is_active_ = true;
    size_t uncaught_count_;
};

/**
 * @brief Factory function to create a ScopeGuardSuccess with automatic type deduction
 * @tparam Func Callable type to execute on successful destruction
 * @param func The callable to wrap
 * @return ScopeGuardSuccess that will execute func only when leaving scope successfully
 * @note Uses perfect forwarding and decay to handle callable types efficiently
 * @example
 * auto guard = generate_scope_guard_success([&]() { save_changes(); });
 */
template <typename Func> requires std::invocable<Func>
[[nodiscard]] ScopeGuardSuccess<std::decay_t<Func>> generate_scope_guard_success(Func&& func) {
    return ScopeGuardSuccess<std::decay_t<Func>>(std::forward<Func>(func));
}

/**
 * @class ScopeGuardFailed
 * @brief RAII wrapper that executes a function only when leaving scope due to an exception
 * @tparam Func Callable type to execute on failure destruction
 * @note The function is only called if an exception is thrown during the scope.
 *       Uses std::uncaught_exceptions() to detect exception state changes.
 *       Useful for rollback operations that should only happen on failure.
 * @example
 * {
 *     auto guard = generate_scope_guard_failed([]() { rollback_transaction(); });
 *     // Do work that might throw...
 * } // rollback_transaction() only called if an exception was thrown
 */
template <typename Func>
    requires std::invocable<Func>
class ScopeGuardFailed {
   public:
    explicit ScopeGuardFailed(Func&& f)
        : func_(std::forward<Func>(f)), is_active_(true), uncaught_count_(std::uncaught_exceptions()) {}

    ScopeGuardFailed(ScopeGuardFailed&& sg)
        : func_(std::move<Func>(sg.func_)), is_active_(sg.is_active_) {
        sg.is_active_ = false;
    }

    ScopeGuardFailed& operator=(ScopeGuardFailed&& sg) {
        func_ = std::move<Func>(sg.func_);
        is_active_ = sg.is_active_;
        sg.is_active_ = false;
        return *this;
    }
    ScopeGuardFailed(const ScopeGuardFailed&) = delete;
    ScopeGuardFailed& operator=(const ScopeGuardFailed&) = delete;
    ~ScopeGuardFailed() noexcept(noexcept(func_())) {
        if (is_active_ && std::uncaught_exceptions() != uncaught_count_) {
            func_();
        }
    }
    void dismiss() { is_active_ = false; }

   protected:
   private:
    Func func_;
    bool is_active_ = true;
    size_t uncaught_count_;
};

/**
 * @brief Factory function to create a ScopeGuardFailed with automatic type deduction
 * @tparam Func Callable type to execute on failure destruction
 * @param func The callable to wrap
 * @return ScopeGuardFailed that will execute func only when leaving scope due to exception
 * @note Uses perfect forwarding and decay to handle callable types efficiently
 * @example
 * auto guard = generate_scope_guard_failed([&]() { cleanup_on_error(); });
 */
template <typename Func> requires std::invocable<Func>
[[nodiscard]] ScopeGuardFailed<std::decay_t<Func>> generate_scope_guard_failed(Func&& func) {
    return ScopeGuardFailed<std::decay_t<Func>>(std::forward<Func>(func));
}

} // namespace zuc