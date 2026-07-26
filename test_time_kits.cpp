#include "doctest.h"
#include "include/time_kits.hpp"
#include "include/io_kits.hpp"
#include <thread>
#include <chrono>
#include <vector>
#include <string>

using namespace zuc;

/**
 * @test Timer Class
 * Tests for Timer class functionality including start, reset, and duration measurement.
 */
TEST_SUITE("Timer Class") {
    TEST_CASE("Timer default construction") {
        Timer timer;
        CHECK(timer.is_running() == false);
    }

    TEST_CASE("Timer start and duration") {
        Timer timer;
        timer.start();
        CHECK(timer.is_running() == true);
        
        auto duration = timer.get_duration_seconds();
        CHECK(duration.has_value());
        CHECK(duration.value().count() >= 0.0);
    }

    TEST_CASE("Timer reset") {
        Timer timer;
        timer.start();
        CHECK(timer.is_running() == true);
        
        timer.reset();
        CHECK(timer.is_running() == false);
        
        auto duration = timer.get_duration_seconds();
        CHECK(!duration.has_value());
    }

    TEST_CASE("Timer duration measurement") {
        Timer timer;
        timer.start();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        auto duration = timer.get_duration_seconds();
        REQUIRE(duration.has_value());
        CHECK(duration.value().count() >= 0.09); // Allow some tolerance
        CHECK(duration.value().count() < 0.2);    // Should not be too long
    }

    TEST_CASE("Timer copy construction") {
        Timer timer1;
        timer1.start();
        
        Timer timer2 = timer1;
        CHECK(timer2.is_running() == true);
    }

    TEST_CASE("Timer copy assignment") {
        Timer timer1;
        timer1.start();
        
        Timer timer2;
        timer2 = timer1;
        CHECK(timer2.is_running() == true);
    }

    TEST_CASE("Timer move construction") {
        Timer timer1;
        timer1.start();
        
        Timer timer2 = std::move(timer1);
        CHECK(timer2.is_running() == true);
    }

    TEST_CASE("Timer move assignment") {
        Timer timer1;
        timer1.start();
        
        Timer timer2;
        timer2 = std::move(timer1);
        CHECK(timer2.is_running() == true);
    }
}

/**
 * @test Stopwatch Class
 * Tests for Stopwatch class including start, pause, resume, reset, and elapsed time.
 */
TEST_SUITE("Stopwatch Class") {
    TEST_CASE("Stopwatch default construction") {
        Stopwatch sw;
        CHECK(sw.is_running() == false);
        CHECK(sw.is_paused() == false);
    }

    TEST_CASE("Stopwatch start") {
        Stopwatch sw;
        sw.start();
        CHECK(sw.is_running() == true);
        CHECK(sw.is_paused() == false);
    }

    TEST_CASE("Stopwatch pause") {
        Stopwatch sw;
        sw.start();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        
        sw.pause();
        CHECK(sw.is_running() == false);
        CHECK(sw.is_paused() == true);
    }

    TEST_CASE("Stopwatch resume") {
        Stopwatch sw;
        sw.start();
        sw.pause();
        
        sw.resume();
        CHECK(sw.is_running() == true);
        CHECK(sw.is_paused() == false);
    }

    TEST_CASE("Stopwatch reset") {
        Stopwatch sw;
        sw.start();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        
        sw.reset();
        CHECK(sw.is_running() == false);
        CHECK(sw.is_paused() == false);
        CHECK(sw.get_elapsed().count() == 0.0);
    }

    TEST_CASE("Stopwatch elapsed time") {
        Stopwatch sw;
        sw.start();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        auto elapsed = sw.get_elapsed();
        CHECK(elapsed.count() >= 0.09);
        CHECK(elapsed.count() < 0.2);
    }

    TEST_CASE("Stopwatch pause and resume timing") {
        Stopwatch sw;
        sw.start();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        sw.pause();
        
        auto paused_time = sw.get_elapsed();
        CHECK(paused_time.count() >= 0.04);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        
        auto still_paused_time = sw.get_elapsed();
        CHECK(still_paused_time.count() >= paused_time.count() - 0.01);
        CHECK(still_paused_time.count() <= paused_time.count() + 0.01);
        
        sw.resume();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        
        auto total_time = sw.get_elapsed();
        CHECK(total_time.count() >= 0.09);
    }

    TEST_CASE("Stopwatch multiple start calls") {
        Stopwatch sw;
        sw.start();
        sw.start(); // Should not start again
        CHECK(sw.is_running() == true);
    }

    TEST_CASE("Stopwatch stop alias") {
        Stopwatch sw;
        sw.start();
        sw.stop(); // Should be same as pause
        CHECK(sw.is_running() == false);
        CHECK(sw.is_paused() == true);
    }

    TEST_CASE("Stopwatch copy construction") {
        Stopwatch sw1;
        sw1.start();
        
        Stopwatch sw2 = sw1;
        CHECK(sw2.is_running() == true);
    }

    TEST_CASE("Stopwatch copy assignment") {
        Stopwatch sw1;
        sw1.start();
        
        Stopwatch sw2;
        sw2 = sw1;
        CHECK(sw2.is_running() == true);
    }

    TEST_CASE("Stopwatch move construction") {
        Stopwatch sw1;
        sw1.start();
        
        Stopwatch sw2 = std::move(sw1);
        CHECK(sw2.is_running() == true);
    }

    TEST_CASE("Stopwatch move assignment") {
        Stopwatch sw1;
        sw1.start();
        
        Stopwatch sw2;
        sw2 = std::move(sw1);
        CHECK(sw2.is_running() == true);
    }
}

/**
 * @test Time Utility Functions
 * Tests for time-related utility functions.
 */
TEST_SUITE("Time Utility Functions") {
    TEST_CASE("get_today_str format") {
        std::string today = get_today_str();
        CHECK(today.size() == 10); // YYYY-MM-DD format
        CHECK(today[4] == '-');
        CHECK(today[7] == '-');
    }

    TEST_CASE("get_today_time_detailed_str format") {
        std::string detailed = get_today_time_detailed_str();
        CHECK(detailed.size() == 27); // YYYY-MM-DD HH:MM:SS format
        CHECK(detailed[4] == '-');
        CHECK(detailed[7] == '-');
        CHECK(detailed[10] == ' ');
        CHECK(detailed[13] == ':');
        CHECK(detailed[16] == ':');
    }

    TEST_CASE("sleep function") {
        Timer timer;
        timer.start();
        
        sleep(0.1); // Sleep for 100ms
        
        auto duration = timer.get_duration_seconds();
        REQUIRE(duration.has_value());
        CHECK(duration.value().count() >= 0.09);
        CHECK(duration.value().count() < 0.2);
    }

    TEST_CASE("sleep zero duration") {
        Timer timer;
        timer.start();
        
        sleep(0.0);
        
        auto duration = timer.get_duration_seconds();
        REQUIRE(duration.has_value());
        CHECK(duration.value().count() < 0.05); // Should be very fast
    }
}

/**
 * @test Timing Precision Functions
 * Tests for time precision conversion and formatting functions.
 */
TEST_SUITE("Timing Precision Functions") {
    TEST_CASE("convert_second_to_precision_value minute") {
        std::chrono::duration<double> sec(120.0);
        double minutes = convert_second_to_precision_value(sec, TimePrecision::minute_precs);
        CHECK(minutes == 2.0);
    }

    TEST_CASE("convert_second_to_precision_value second") {
        std::chrono::duration<double> sec(45.5);
        double seconds = convert_second_to_precision_value(sec, TimePrecision::second_precs);
        CHECK(seconds == 45.5);
    }

    TEST_CASE("convert_second_to_precision_value millisecond") {
        std::chrono::duration<double> sec(1.5);
        double milliseconds = convert_second_to_precision_value(sec, TimePrecision::millisecond_precs);
        CHECK(milliseconds == 1500.0);
    }

    TEST_CASE("convert_second_to_precision_value microsecond") {
        std::chrono::duration<double> sec(0.001);
        double microseconds = convert_second_to_precision_value(sec, TimePrecision::microsecond_precs);
        CHECK(microseconds == 1000.0);
    }

    TEST_CASE("get_prec_unit") {
        CHECK(get_prec_unit(TimePrecision::minute_precs) == "min");
        CHECK(get_prec_unit(TimePrecision::second_precs) == "s");
        CHECK(get_prec_unit(TimePrecision::millisecond_precs) == "ms");
        CHECK(get_prec_unit(TimePrecision::microsecond_precs) == "mircos");
    }

    TEST_CASE("format_duration") {
        std::chrono::duration<double> sec(1.5);
        
        std::string formatted = format_duration(sec, TimePrecision::second_precs, 2);
        CHECK(formatted == "1.50 s");
        
        formatted = format_duration(sec, TimePrecision::millisecond_precs, 1);
        CHECK(formatted == "1500.0 ms");
    }

    TEST_CASE("format_duration different precisions") {
        std::chrono::duration<double> sec(0.123456);
        
        auto min_str = format_duration(sec, TimePrecision::minute_precs, 4);
        auto sec_str = format_duration(sec, TimePrecision::second_precs, 4);
        auto ms_str = format_duration(sec, TimePrecision::millisecond_precs, 2);
        auto us_str = format_duration(sec, TimePrecision::microsecond_precs, 0);
        
        CHECK(min_str.find("min") != std::string::npos);
        CHECK(sec_str.find("s") != std::string::npos);
        CHECK(ms_str.find("ms") != std::string::npos);
        CHECK(us_str.find("mircos") != std::string::npos);
    }
}

/**
 * @test Timing Setting and Benchmarking
 * Tests for timing configuration and function benchmarking.
 */
TEST_SUITE("Timing Setting and Benchmarking") {
    TEST_CASE("TimingSetting default values") {
        TimingSetting setting;
        CHECK(setting.print_after_each_run == false);
        CHECK(setting.print_max_time == true);
        CHECK(setting.print_min_time == true);
        CHECK(setting.print_average_time == true);
        CHECK(setting.times_to_run == 10);
        CHECK(setting.decimal_places == 2);
        CHECK(setting.prec == TimePrecision::millisecond_precs);
    }

    TEST_CASE("DEFAULT_TIMING_SETTING") {
        CHECK(DEFAULT_TIMING_SETTING.print_after_each_run == true);
        CHECK(DEFAULT_TIMING_SETTING.times_to_run == 10);
    }

    TEST_CASE("time_a_function basic") {
        TimingSetting setting;
        setting.times_to_run = 3;
        setting.print_after_each_run = false;
        setting.print_max_time = false;
        setting.print_min_time = false;
        setting.print_average_time = false;
        
        CHECK_NOTHROW(time_a_function("test_func", setting, []() {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }));
    }

    TEST_CASE("time_a_function with arguments") {
        TimingSetting setting;
        setting.times_to_run = 2;
        setting.print_after_each_run = false;
        setting.print_max_time = false;
        setting.print_min_time = false;
        setting.print_average_time = false;
        
        auto func = [](int x, int y) { return x + y; };
        CHECK_NOTHROW(time_a_function("add_func", setting, func, 5, 3));
    }

    TEST_CASE("time_a_function zero times") {
        TimingSetting setting;
        setting.times_to_run = 0;
        
        CHECK_NOTHROW(time_a_function("should_not_run", setting, []() {
            FAIL("Function should not be called");
        }));
    }

    TEST_CASE("time_a_function with return value") {
        TimingSetting setting;
        setting.times_to_run = 2;
        setting.print_after_each_run = false;
        setting.print_max_time = false;
        setting.print_min_time = false;
        setting.print_average_time = false;
        
        auto func = []() { return 42; };
        CHECK_NOTHROW(time_a_function("return_func", setting, func));
    }

    TEST_CASE("time_a_function different precisions") {
        TimingSetting setting;
        setting.times_to_run = 2;
        setting.print_after_each_run = false;
        setting.print_max_time = false;
        setting.print_min_time = false;
        setting.print_average_time = false;
        
        setting.prec = TimePrecision::second_precs;
        CHECK_NOTHROW(time_a_function("sec_test", setting, []() {}));
        
        setting.prec = TimePrecision::millisecond_precs;
        CHECK_NOTHROW(time_a_function("ms_test", setting, []() {}));
        
        setting.prec = TimePrecision::microsecond_precs;
        CHECK_NOTHROW(time_a_function("us_test", setting, []() {}));
    }
}

/**
 * @test DateTime Class
 * Tests for DateTime class functionality (when available).
 */
#ifndef ZUC_DROP_CHRONO_TIME_ZONE
TEST_SUITE("DateTime Class") {
    TEST_CASE("DateTime now") {
        auto now = DateTime::now();
        CHECK(now.get_year() >= 2020);
        CHECK(now.get_month() >= 1);
CHECK(now.get_month() <= 12);
CHECK(now.get_day() >= 1);
CHECK(now.get_day() <= 31);
CHECK(now.get_hour() >= 0);
CHECK(now.get_hour() <= 23);
CHECK(now.get_minute() >= 0);
CHECK(now.get_minute() <= 59);
CHECK(now.get_second() >= 0);
CHECK(now.get_second() <= 59);
    }

    TEST_CASE("DateTime from_ymd basic") {
        auto dt = DateTime::from_ymd(2024, 6, 15, 12, 30, 45, 123456);
        CHECK(dt.get_year() == 2024);
        CHECK(dt.get_month() == 6);
        CHECK(dt.get_day() == 15);
        CHECK(dt.get_hour() == 12);
        CHECK(dt.get_minute() == 30);
        CHECK(dt.get_second() == 45);
        CHECK(dt.get_microsecond() == 123456);
    }

    TEST_CASE("DateTime from_ymd invalid date") {
        CHECK_THROWS_AS(DateTime::from_ymd(2024, 13, 1), DateTime::InvalidDateError);
        CHECK_THROWS_AS(DateTime::from_ymd(2024, 2, 30), DateTime::InvalidDateError);
        CHECK_THROWS_AS(DateTime::from_ymd(2024, 6, 15, 25, 0, 0), DateTime::InvalidDateError);
    }

    TEST_CASE("DateTime from_timestamp") {
        auto dt = DateTime::from_timestamp(0.0);
        CHECK(dt.get_year() == 1970);
        CHECK(dt.get_month() == 1);
        CHECK(dt.get_day() == 1);
    }

    TEST_CASE("DateTime time_stamp") {
        auto dt = DateTime::from_ymd(2024, 6, 15, 12, 0, 0);
        double ts = dt.time_stamp();
        CHECK(ts > 0.0);
    }

    TEST_CASE("DateTime addition") {
        auto dt = DateTime::from_ymd(2024, 6, 15, 12, 0, 0);
        auto dt_plus = dt + std::chrono::hours(1);
        CHECK(dt_plus.get_hour() == 13);
    }

    TEST_CASE("DateTime subtraction") {
        auto dt = DateTime::from_ymd(2024, 6, 15, 12, 0, 0);
        auto dt_minus = dt - std::chrono::hours(1);
        CHECK(dt_minus.get_hour() == 11);
    }

    TEST_CASE("DateTime difference") {
        auto dt1 = DateTime::from_ymd(2024, 6, 15, 12, 0, 0);
        auto dt2 = DateTime::from_ymd(2024, 6, 15, 13, 0, 0);
        auto diff = dt2 - dt1;
        CHECK(diff.count() == 3600000000.0); // 1 hour in microseconds
    }

    TEST_CASE("DateTime offset_days") {
        auto dt = DateTime::from_ymd(2024, 6, 15, 12, 0, 0);
        auto dt_plus_day = dt.offset_days(1);
        CHECK(dt_plus_day.get_day() == 16);
        
        auto dt_minus_day = dt.offset_days(-1);
        CHECK(dt_minus_day.get_day() == 14);
    }

    TEST_CASE("DateTime to_string") {
        auto dt = DateTime::from_ymd(2024, 6, 15, 12, 30, 45, 123456);
        std::string str = dt.to_string();
        CHECK(str.find("2024") != std::string::npos);
        CHECK(str.find("6") != std::string::npos);
        CHECK(str.find("15") != std::string::npos);
    }

    TEST_CASE("DateTime compound assignment") {
        auto dt = DateTime::from_ymd(2024, 6, 15, 12, 0, 0);
        dt += std::chrono::hours(1);
        CHECK(dt.get_hour() == 13);
        
        dt -= std::chrono::hours(2);
        CHECK(dt.get_hour() == 11);
    }

    TEST_CASE("DateTime copy construction") {
        auto dt1 = DateTime::from_ymd(2024, 6, 15, 12, 0, 0);
        DateTime dt2 = dt1;
        CHECK(dt2.get_year() == 2024);
        CHECK(dt2.get_month() == 6);
        CHECK(dt2.get_day() == 15);
    }

    TEST_CASE("DateTime copy assignment") {
        auto dt1 = DateTime::from_ymd(2024, 6, 15, 12, 0, 0);
        DateTime dt2 = dt1;
        CHECK(dt2.get_year() == 2024);
        CHECK(dt2.get_month() == 6);
        CHECK(dt2.get_day() == 15);
    }
}
#endif

/**
 * @test Time Edge Cases
 * Tests for edge cases and error handling in time operations.
 */
TEST_SUITE("Time Edge Cases") {
    TEST_CASE("Timer without start") {
        Timer timer;
        auto duration = timer.get_duration_seconds();
        CHECK(!duration.has_value());
    }

    TEST_CASE("Timer multiple starts") {
        Timer timer;
        timer.start();
        timer.start(); // Should reset and start again
        CHECK(timer.is_running() == true);
    }

    TEST_CASE("Stopwatch without start") {
        Stopwatch sw;
        auto elapsed = sw.get_elapsed();
        CHECK(elapsed.count() == 0.0);
    }

    TEST_CASE("Stopwatch multiple pauses") {
        Stopwatch sw;
        sw.start();
        sw.pause();
        sw.pause(); // Should remain paused
        CHECK(sw.is_paused() == true);
    }

    TEST_CASE("Stopwatch resume without pause") {
        Stopwatch sw;
        sw.start();
        sw.resume(); // Should have no effect
        CHECK(sw.is_running() == true);
    }

    TEST_CASE("format_duration zero duration") {
        std::chrono::duration<double> sec(0.0);
        auto formatted = format_duration(sec, TimePrecision::second_precs, 2);
        CHECK(formatted == "0.00 s");
    }

    TEST_CASE("format_duration negative duration") {
        std::chrono::duration<double> sec(-1.5);
        auto formatted = format_duration(sec, TimePrecision::second_precs, 2);
        CHECK(formatted.find("-") != std::string::npos);
    }

    TEST_CASE("time_a_function exception handling") {
        TimingSetting setting;
        setting.times_to_run = 1;
        setting.print_after_each_run = false;
        setting.print_max_time = false;
        setting.print_min_time = false;
        setting.print_average_time = false;
        
        CHECK_THROWS(time_a_function("throwing_func", setting, []() {
            throw std::runtime_error("Test exception");
        }));
    }
}