#include <filesystem>
#include <string>

#include "doctest.h"
#include "include/docx_writer.hpp"

namespace fs = std::filesystem;

static void remove_if_exists(const char* path) {
    if (fs::exists(path)) fs::remove(path);
}

TEST_SUITE("DocxWriter") {
    TEST_CASE("basic save") {
        const char* out = "docx_test_basic.docx";
        remove_if_exists(out);

        zuc::DocxWriter w;
        w.add_line("Hello");
        w.add_line("World");
        CHECK(w.save(out));
        CHECK(fs::exists(out));
        CHECK(fs::file_size(out) > 0);
    }

    TEST_CASE("font and size") {
        const char* out = "docx_test_font.docx";
        remove_if_exists(out);

        zuc::DocxWriter w;
        w.set_font("SimSun");
        w.set_font_size(14);
        w.add_line("test");
        CHECK(w.save(out));
        CHECK(fs::exists(out));
    }

    TEST_CASE("xml escape") {
        const char* out = "docx_test_escape.docx";
        remove_if_exists(out);

        zuc::DocxWriter w;
        w.add_line("a < b & c > d");
        CHECK(w.save(out));
        CHECK(fs::exists(out));

        remove_if_exists(out);
    }

    TEST_CASE("empty lines") {
        const char* out = "docx_test_empty.docx";
        remove_if_exists(out);

        zuc::DocxWriter w;
        w.add_line("");
        w.add_line("only text");
        w.add_line("");
        CHECK(w.save(out));
        CHECK(fs::exists(out));

        remove_if_exists(out);
    }

    TEST_CASE("spaces preserve") {
        const char* out = "docx_test_spaces.docx";
        remove_if_exists(out);

        zuc::DocxWriter w;
        w.add_line("  leading");
        w.add_line("trailing  ");
        w.add_line("  both  ");
        w.add_line("no spaces");
        CHECK(w.save(out));
        CHECK(fs::exists(out));

        remove_if_exists(out);
    }

    TEST_CASE("chinese") {
        const char* out = "docx_test_chinese.docx";
        remove_if_exists(out);

        zuc::DocxWriter w;
        w.set_font("SimSun");
        w.set_font_size(12);
        w.add_line("你好世界");
        w.add_line("第二行");
        CHECK(w.save(out));
        CHECK(fs::exists(out));
    }

    TEST_CASE("default font") {
        const char* out = "docx_test_default.docx";
        remove_if_exists(out);

        zuc::DocxWriter w;
        w.add_line("default");
        CHECK(w.save(out));
        CHECK(fs::exists(out));

        remove_if_exists(out);
    }

    TEST_CASE("many lines") {
        const char* out = "docx_test_many.docx";
        remove_if_exists(out);

        zuc::DocxWriter w;
        for (int i = 0; i < 100; ++i) {
            w.add_line("Line " + std::to_string(i));
        }
        CHECK(w.save(out));
        CHECK(fs::exists(out));
        CHECK(fs::file_size(out) > 0);

    }

    TEST_CASE("basic table") {
        const char* out = "docx_test_table.docx";
        remove_if_exists(out);

        zuc::DocxWriter w;
        w.add_table({
            {"Name", "Age", "City"},
            {"Alice", "30", "Beijing"},
            {"Bob", "25", "Shanghai"},
        });
        CHECK(w.save(out));
        CHECK(fs::exists(out));
        CHECK(fs::file_size(out) > 0);

    }

    TEST_CASE("table with xml escape") {
        const char* out = "docx_test_table_escape.docx";
        remove_if_exists(out);

        zuc::DocxWriter w;
        w.add_table({
            {"a < b", "c & d"},
            {"x > y", "normal"},
        });
        CHECK(w.save(out));
        CHECK(fs::exists(out));

    }

    TEST_CASE("table and lines mixed") {
        const char* out = "docx_test_mixed.docx";
        remove_if_exists(out);

        zuc::DocxWriter w;
        w.add_line("Report Title");
        w.add_table({
            {"Item", "Count"},
            {"Apple", "5"},
            {"Banana", "3"},
        });
        w.add_line("End of report");
        CHECK(w.save(out));
        CHECK(fs::exists(out));
        CHECK(fs::file_size(out) > 0);

    }

    TEST_CASE("single cell table") {
        const char* out = "docx_test_single_cell.docx";
        remove_if_exists(out);

        zuc::DocxWriter w;
        w.add_table({{"only cell"}});
        CHECK(w.save(out));
        CHECK(fs::exists(out));

    }

    TEST_CASE("table with spaces") {
        const char* out = "docx_test_table_spaces.docx";
        remove_if_exists(out);

        zuc::DocxWriter w;
        w.add_table({
            {"  leading", "trailing  "},
            {"  both  ", "no spaces"},
        });
        CHECK(w.save(out));
        CHECK(fs::exists(out));

    }

    TEST_CASE("chinese table") {
        const char* out = "docx_test_table_chinese.docx";
        remove_if_exists(out);

        zuc::DocxWriter w;
        w.set_font("SimSun");
        w.set_font_size(12);
        w.add_table({
            {"姓名", "年龄", "城市"},
            {"张三", "30", "北京"},
            {"李四", "25", "上海"},
        });
        CHECK(w.save(out));
        CHECK(fs::exists(out));

    }
}