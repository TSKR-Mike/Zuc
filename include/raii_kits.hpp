#pragma once
#include <concepts>
#include <exception>
#include <utility>

namespace zuc {

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

    void dismiss() { is_active_ = false; }

   protected:
   private:
    Func func_;
    bool is_active_ = true;
};

template <typename Func> requires std::invocable<Func>
[[nodiscard]] ScopeGuard<std::decay_t<Func>> generate_scope_guard(Func&& func) {
    return ScopeGuard<std::decay_t<Func>>(std::forward<Func>(func));
}

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
    size_t uncaught_count_=0;
};

template <typename Func> requires std::invocable<Func>
[[nodiscard]] ScopeGuardSuccess<std::decay_t<Func>> generate_scope_guard_success(Func&& func) {
    return ScopeGuardSuccess<std::decay_t<Func>>(std::forward<Func>(func));
}

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
    size_t uncaught_count_=0;
};

template <typename Func> requires std::invocable<Func>
[[nodiscard]] ScopeGuardFailed<std::decay_t<Func>> generate_scope_guard_failed(Func&& func) {
    return ScopeGuardFailed<std::decay_t<Func>>(std::forward<Func>(func));
}

} // namespace zuc
