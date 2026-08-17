/**
 * @file time_kits.hpp
 * @brief Time utilities and timing functions for C++20
 * @date 2026-07-30
 * @copyright Copyright (c) 2026
 * @note Provides Timer, Stopwatch, and DateTime classes for performance
 * measurement, function benchmarking, and convenient time manipulation
 * operations
 */

#pragma once
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <functional>
#include <numeric>
#include <optional>
#include <string>
#include <thread>
#include <vector>

#include "io_kits.hpp"

#ifdef __APPLE__
#define ZUC_DROP_TIMEZONE
#endif

namespace zuc {
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::steady_clock;
using std::chrono::system_clock;
using std::chrono::time_point;
using seconds_unit = duration<double>;
using minutes_unit = duration<double, std::ratio<60>>;
using milli_seconds_unit = duration<double, std::milli>;
using micro_seconds_unit = duration<double, std::micro>;
using seconds_unit_type = duration<double>;
using minutes_unit_type = duration<double, std::chrono::minutes>;
using micro_seconds_unit_type = duration<double, std::chrono::microseconds>;
using milli_seconds_unit_type = duration<double, std::chrono::milliseconds>;
using std::chrono::day, std::chrono::year, std::chrono::month;
using std::chrono::days;
using std::chrono::hh_mm_ss;
using std::chrono::hours, std::chrono::minutes, std::chrono::seconds,
    std::chrono::microseconds;
using std::chrono::local_days;
using std::chrono::local_time;
using std::chrono::sys_days;
using std::chrono::year_month_day;

/**
 * @class Timer
 * @brief Simple timer for measuring elapsed time
 * @note Uses steady_clock for monotonic time measurement that won't be affected
 * by system clock changes. Supports start, reset, and elapsed time query
 * operations.
 * @example
 * Timer timer;
 * timer.start();
 * // ... do work ...
 * auto elapsed = timer.get_duration_seconds();
 * if (elapsed) {
 *     std::cout << "Time: " << elapsed->count() << "s" << std::endl;
 * }
 */
class Timer {
   public:
    Timer() = default;
    ~Timer() = default;
    Timer(const Timer&) = default;
    Timer(Timer&&) = default;
    Timer& operator=(const Timer& t) {
        start_timing_ = t.start_timing_;
        t_ = t.t_;
        return *this;
    }
    Timer& operator=(Timer&& t) noexcept {
        start_timing_ = t.start_timing_;
        t_ = std::move(t.t_);
        return *this;
    }

    /**
     * @brief Resets the timer to stopped state
     * @note Clears the start time and sets the timer to not running
     */
    void reset() { start_timing_ = false; }

    /**
     * @brief Starts the timer
     * @note If the timer is already running, this restarts it
     */
    void start() {
        start_timing_ = true;
        t_ = steady_clock::now();
    }

    /**
     * @brief Gets the elapsed time since the timer was started
     * @return std::optional<duration<double>> containing elapsed seconds, or
     * nullopt if timer not started
     * @note Returns the time elapsed since start() was called
     */
    std::optional<duration<double>> get_duration_seconds() {
        if (!start_timing_) {
            return std::nullopt;
        }
        return steady_clock::now() - t_;
    }

    /**
     * @brief Checks if the timer is currently running
     * @return true if the timer has been started and not reset, false otherwise
     */
    bool is_running() const { return start_timing_; }

   protected:
   private:
    time_point<steady_clock> t_;
    bool start_timing_ = false;
};

/**
 * @class Stopwatch
 * @brief Advanced stopwatch with pause/resume functionality
 * @note Extends Timer functionality with the ability to pause and resume
 * timing. Useful for measuring cumulative time while excluding certain periods.
 * @example
 * Stopwatch sw;
 * sw.start();
 * // ... work ...
 * sw.pause();  // Stop timing
 * // ... don't count this time ...
 * sw.resume(); // Continue timing
 * // ... more work ...
 * sw.stop();
 * auto total = sw.get_elapsed();
 */
class Stopwatch {
   public:
    Stopwatch() = default;
    ~Stopwatch() = default;
    Stopwatch(const Stopwatch&) = default;
    Stopwatch(Stopwatch&&) = default;
    Stopwatch& operator=(const Stopwatch&) = default;
    Stopwatch& operator=(Stopwatch&&) noexcept = default;

    /**
     * @brief Starts or resumes the stopwatch
     * @note If the stopwatch is not running, starts timing from the current
     * moment. If already running, this call has no effect.
     */
    void start() {
        if (!running_) {
            start_time_ = steady_clock::now();
            running_ = true;
        }
    }

    void pause() {
        if (running_) {
            accumulated_time_ += steady_clock::now() - start_time_;
            running_ = false;
            paused_ = true;
        }
    }

    void resume() {
        if (!running_) {
            start_time_ = steady_clock::now();
            running_ = true;
            paused_ = false;
        }
    }

    void reset() {
        accumulated_time_ = duration<double>(0);
        running_ = false;
        paused_ = false;
    }

    void stop() { pause(); }

    duration<double> get_elapsed() const {
        if (running_) {
            return accumulated_time_ + (steady_clock::now() - start_time_);
        }
        return accumulated_time_;
    }

    bool is_running() const { return running_; }
    bool is_paused() const { return paused_; }

   private:
    time_point<steady_clock> start_time_;
    duration<double> accumulated_time_{0};
    bool running_ = false;
    bool paused_ = false;
};

inline std::string get_today_str() {
    using namespace std::chrono;
    auto now = system_clock::now();
    return std::format("{:%Y-%m-%d}", now);
}

inline std::string get_today_time_detailed_str() {
    auto now = system_clock::now();
    return std::format("{:%Y-%m-%d %H:%M:%S}", now);
}

enum class TimePrecision {
    minute_precs,
    second_precs,
    millisecond_precs,
    microsecond_precs
};

struct TimingSetting {
    bool print_after_each_run = false;
    bool print_max_time = true;
    bool print_min_time = true;
    bool print_average_time = true;
    size_t times_to_run = 10;
    size_t decimal_places = 2;
    TimePrecision prec = TimePrecision::millisecond_precs;
};

const TimingSetting DEFAULT_TIMING_SETTING{
    true, true, true, true, 10, 2, TimePrecision::millisecond_precs};

inline double convert_second_to_precision_value(
    std::chrono::duration<double> sec, TimePrecision prec) {
    using namespace std::chrono;
    switch (prec) {
        case TimePrecision::minute_precs:
            return duration_cast<minutes_unit>(sec).count();
        case TimePrecision::second_precs:
            return sec.count();
        case TimePrecision::millisecond_precs:
            return duration_cast<milli_seconds_unit>(sec).count();
        case TimePrecision::microsecond_precs:
            return duration_cast<micro_seconds_unit>(sec).count();
        default:
            return sec.count();  // fallback
    }
}

inline std::string get_prec_unit(TimePrecision prec) {
    switch (prec) {
        case TimePrecision::minute_precs:
            return "min";
        case TimePrecision::second_precs:
            return "s";
        case TimePrecision::millisecond_precs:
            return "ms";
        case TimePrecision::microsecond_precs:
            return "micros";
        default:
            return "s";
    }
}

inline std::string format_duration(std::chrono::duration<double> sec,
                                   TimePrecision prec,
                                   size_t decimal_places = 2) {
    double value = convert_second_to_precision_value(sec, prec);
    std::string unit = get_prec_unit(prec);
    return std::format("{:.{}f} {}", value, decimal_places, unit);
}

/**
 * @brief Benchmark a function with detailed statistics
 * @param name Name/identifier for the function being timed
 * @param setting Configuration for timing behavior
 * @param f Function to benchmark (must be invocable with Args)
 * @param args Arguments to pass to the function
 *
 * Runs the function multiple times and collects statistics including:
 * - Total time across all runs
 * - Maximum execution time
 * - Minimum execution time
 * - Average execution time
 *
 * @example
 * void my_function() { // ... }
 * TimingSetting settings{.times_to_run = 5, .print_after_each_run = true};
 * time_a_function("my_function", settings, my_function);
 */
template <typename F, typename... Args>
    requires std::invocable<F, Args...>
inline void time_a_function(const std::string& name, TimingSetting setting,
                            F&& f, Args&&... args) {
    // Quit if time is set to 0
    if (setting.times_to_run == 0) {
        write_a_line_to_console("WARNING:0 times setted. Quitting.");
        return;
    }
    // Start to time it
    Timer timer;
    std::vector<duration<double>>
        running_data;  // Store all the data in sec unit
    running_data.reserve(setting.times_to_run);
    std::string curr_unit = get_prec_unit(setting.prec);
    write_a_line_to_console("Start timing {} at {}", name,
                            get_today_time_detailed_str());

    for (size_t i = 1; i <= setting.times_to_run; ++i) {
        timer.reset();  // Reset the timer before each run
        timer.start();
        // Call the function to be tested
        std::invoke(std::forward<F>(f), std::forward<Args>(args)...);

        auto curr_duration = timer.get_duration_seconds();
        if (!curr_duration) {
            write_a_line_to_console("ERROR:Failed to calculate time");
        }
        running_data.push_back(*curr_duration);

        if (setting.print_after_each_run) {
            write_a_line_to_console(
                "[{}/{}] Completed in {}", i, setting.times_to_run,
                format_duration(*curr_duration, setting.prec,
                                setting.decimal_places));
        }
    }
    // Calculate all the data statistics
    duration<double> total = std::accumulate(
        running_data.begin(), running_data.end(), duration<double>(0));
    double total_value = convert_second_to_precision_value(total, setting.prec);
    auto max = *std::max_element(running_data.begin(), running_data.end());
    auto min = *std::min_element(running_data.begin(), running_data.end());

    // Print all the data statistics
    write_a_line_to_console("All {} runs completed in {}{}",
                            setting.times_to_run, total_value, curr_unit);
    write_a_line_to_console(
        "Max:{}", format_duration(max, setting.prec, setting.decimal_places));
    write_a_line_to_console(
        "Min:{}", format_duration(min, setting.prec, setting.decimal_places));
    write_a_line_to_console(
        "Avg:{}", format_duration(total / setting.times_to_run, setting.prec,
                                  setting.decimal_places));
}

inline void sleep(double secs) {
    std::this_thread::sleep_for(duration<double>(secs));
}

// To support LLVM19(chrono not fully implemented)
class DateTime {
   public:
    using Duration = std::chrono::microseconds;
    using TimePoint = std::chrono::local_time<Duration>;
    // Define a error class
    class InvalidDateError : public std::runtime_error {
       public:
        using std::runtime_error::runtime_error;
    };
    // Factory methods
    static DateTime now() {
        auto sys_t_now = system_clock::now();
#ifndef ZUC_DROP_TIMEZONE
        auto local_tp = std::chrono::current_zone()->to_local(sys_t_now);
        return DateTime(std::chrono::time_point_cast<Duration>(local_tp));
#else
        // Fallback to UTC when timezone support is not available
        return DateTime(std::chrono::time_point_cast<Duration>(sys_t_now));
#endif
    }
    static DateTime from_timestamp(double seconds) {
        using namespace std::chrono;
        sys_time<Duration> sys_tp = sys_time<Duration>{
            duration_cast<Duration>(duration<double>(seconds))};
#ifndef ZUC_DROP_TIMEZONE
        // Convert to local time
        auto local_tp = current_zone()->to_local(sys_tp);
        return DateTime(local_tp);
#else
        // Fallback to UTC when timezone support is not available
        return DateTime(sys_tp);
#endif
    }

    static DateTime from_ymd(int year_v, int month_v, int day_v, int hour_v = 0,
                             int minute_v = 0, int second_v = 0,
                             int microsecond_v = 0) {
        // Check the validity of the input
        if (month_v < 1 || month_v > 12 || day_v < 1 || day_v > 31 ||
            hour_v < 0 || hour_v > 23 || minute_v < 0 || minute_v > 59 ||
            second_v < 0 || second_v > 59 || microsecond_v < 0 ||
            microsecond_v > 999999) {
            throw InvalidDateError("Invalid date" +
                                   std::format("{}-{}-{} {}:{}:{}:{}", year_v,
                                               month_v, day_v, hour_v, minute_v,
                                               second_v, microsecond_v));
        }
        // Create the time point
        year_month_day ymd{year{year_v},
                           month{static_cast<unsigned int>(month_v)},
                           day{static_cast<unsigned int>(day_v)}};
        if (!ymd.ok()) {
            throw InvalidDateError("Invalid date" + std::format("{}-{}-{}",
                                                                year_v, month_v,
                                                                day_v));
        }
        local_days curr_days = local_days{ymd};
        // Calculate hours, minutes, seconds, microseconds
        auto time_of_day = hours{hour_v} + minutes{minute_v} +
                           seconds{second_v} + microseconds{microsecond_v};
        auto local_tp = local_time<microseconds>{curr_days} + time_of_day;

        return DateTime(local_tp);
    }
    int get_year() const {
        auto curr_days = std::chrono::floor<days>(tp_);
        year_month_day ymd{curr_days};
        return static_cast<int>(ymd.year());
    }
    int get_month() const {
        auto curr_days = std::chrono::floor<days>(tp_);
        year_month_day ymd{curr_days};
        return static_cast<int>(static_cast<unsigned int>(ymd.month()));
    }
    int get_day() const {
        auto curr_days = std::chrono::floor<days>(tp_);
        year_month_day ymd{curr_days};
        return static_cast<int>(static_cast<unsigned int>(ymd.day()));
    }
    int get_hour() const {
        auto curr_days = std::chrono::floor<days>(tp_);
        hh_mm_ss hms{tp_ - curr_days};
        return hms.hours().count();
    }
    int get_minute() const {
        auto curr_days = std::chrono::floor<days>(tp_);
        hh_mm_ss hms{tp_ - curr_days};
        return hms.minutes().count();
    }
    int get_second() const {
        auto curr_days = std::chrono::floor<days>(tp_);
        hh_mm_ss hms{tp_ - curr_days};
        return hms.seconds().count();
    }
    int get_microsecond() const {
        auto curr_days = std::chrono::floor<days>(tp_);
        hh_mm_ss hms{tp_ - curr_days};
        return static_cast<int>(hms.subseconds().count());
    }
    double time_stamp() const {
        using namespace std::chrono;
#ifndef ZUC_DROP_TIMEZONE
        // Convert local_time to system_clock to get UTC seconds since epoch
        auto sys_tp = current_zone()->to_sys(tp_);
        auto total_duration =
            duration_cast<duration<double>>(sys_tp.time_since_epoch());
        return total_duration.count();
#else
        // When using UTC directly, just get the duration since epoch
        auto total_duration =
            duration_cast<duration<double>>(tp_.time_since_epoch());
        return total_duration.count();
#endif
    }
    DateTime& operator+=(const Duration& delta) {
        tp_ += delta;
        return *this;
    }
    DateTime& operator-=(const Duration& delta) {
        tp_ -= delta;
        return *this;
    }

    DateTime operator+(const Duration& delta) const {
        return DateTime(tp_ + delta);
    }
    DateTime operator-(const Duration& delta) const {
        return DateTime(tp_ - delta);
    }

    Duration operator-(const DateTime& other) const { return tp_ - other.tp_; }
    DateTime offset_days(int days) const {
        return *this + std::chrono::days{days};
    }

    std::string to_string() const {
        return std::format("{}-{}-{} {}:{}:{}:{}", get_year(), get_month(),
                           get_day(), get_hour(), get_minute(), get_second(),
                           get_microsecond());
    }

   protected:
   private:
    TimePoint tp_;
    explicit DateTime(TimePoint t) : tp_(t) {}
};

class CountDownTimer {
    using Duration = std::chrono::duration<double>;  // in seconds
    using Clock = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

   public:
    explicit CountDownTimer(double time_left)
        : total_duration_(Duration(time_left)),
          time_passed_before_pause_(Duration::zero()),
          started_(false),
          paused_(false) {
        assert(time_left >= 0);
    }
    ~CountDownTimer() = default;
    Duration get_total_count_down_time() const { return total_duration_; }
    void start() {
        if (!started_) {
            start_time_ = Clock::now();
            started_ = true;
            return;
        }
        if (!paused_) {
            restart();
            return;
        }
        if (paused_) {
            resume();
            return;
        }
    }
    std::optional<double> get_remaining_time() const {
        if (!started_) {
            return std::nullopt;
        }
        if (paused_) {
            double remaining =
                total_duration_.count() - time_passed_before_pause_.count();
            return remaining >= 0.0 ? remaining : 0.0;
        }

        // Calculate elapsed time as Duration (double seconds)
        Duration elapsed =
            std::chrono::duration_cast<Duration>(Clock::now() - start_time_);
        Duration remaining =
            total_duration_ - elapsed - time_passed_before_pause_;

        if (remaining.count() <= 0.0) {
            return 0.0;
        } else {
            return remaining.count();
        }
    }

    std::optional<bool> is_finished() const {
        if (!started_) {
            return std::nullopt;
        } else {
            if (get_remaining_time().value() <= 0) {
                return true;
            } else {
                return false;
            }
        }
    }

    void pause() {
        if (started_ && !paused_) {
            time_passed_before_pause_ += Clock::now() - start_time_;
            paused_ = true;
        }
    }

    void resume() {
        if (paused_) {
            start_time_ = Clock::now();
            paused_ = false;
        }
    }

    void restart() {
        start_time_ = Clock::now();
        time_passed_before_pause_ = Duration(0);
        paused_ = false;
        started_ = true;
    }

    void reset(double t) {
        assert(t >= 0);
        total_duration_ = Duration(t);
        time_passed_before_pause_ = Duration(0);
        started_ = false;
        paused_ = false;
    }
    std::optional<Duration> get_remaining_duration() const {
        if (!started_) return std::nullopt;

        Duration remaining;
        if (paused_) {
            remaining = total_duration_ - time_passed_before_pause_;
        } else {
            // Calculate elapsed time as Duration (double seconds) for
            // consistent arithmetic
            Duration elapsed = std::chrono::duration_cast<Duration>(
                Clock::now() - start_time_);
            remaining = total_duration_ - elapsed - time_passed_before_pause_;
        }

        // Protect the result to be positive or 0
        if (remaining.count() < 0.0) {
            remaining = Duration::zero();
        }
        return remaining;
    }

   protected:
   private:
    Duration total_duration_;            // in seconds
    Duration time_passed_before_pause_;  // saves the passed duration,
                                         // supporting the pause/resume
    TimePoint start_time_;
    bool started_ = false;
    bool paused_ = false;
};

}  // namespace zuc