#pragma once

#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

#ifdef ZUC_USE_MINIZ
#include "miniz.h"
#endif

namespace zuc {
namespace detail {

#ifdef ZUC_USE_MINIZ
inline bool zip_add_file(mz_zip_archive& zip, const char* name,
                         const void* data, size_t size) {
    return mz_zip_writer_add_mem(&zip, name, data, size,
                                 MZ_DEFAULT_COMPRESSION) != 0;
}
inline bool zip_begin(mz_zip_archive& zip, const char* path) {
    return mz_zip_writer_init_file(&zip, path, 0) != 0;
}
inline bool zip_end(mz_zip_archive& zip) {
    return mz_zip_writer_finalize_archive(&zip) != 0 &&
           mz_zip_writer_end(&zip) != 0;
}
#else

struct ZipEntry {
    std::string name;
    uint32_t crc32;
    uint32_t size;
    uint32_t local_header_offset;
};

struct ZipWriter {
    FILE* file = nullptr;
    std::vector<ZipEntry> entries;

    static constexpr uint32_t LOCAL_FILE_HEADER_SIG = 0x04034B50;
    static constexpr uint32_t CENTRAL_DIR_HEADER_SIG = 0x02014B50;
    static constexpr uint32_t END_CENTRAL_DIR_SIG = 0x06054B50;
    static constexpr uint16_t VERSION_NEEDED = 20;
    static constexpr uint16_t METHOD_STORE = 0;

    static constexpr uint16_t DOS_TIME = 0x0000;
    static constexpr uint16_t DOS_DATE = 0x5C21;

    static void write_u16_le(FILE* f, uint16_t v) {
        uint8_t b[2] = {static_cast<uint8_t>(v & 0xFF),
                        static_cast<uint8_t>((v >> 8) & 0xFF)};
        std::fwrite(b, 1, 2, f);
    }

    static void write_u32_le(FILE* f, uint32_t v) {
        uint8_t b[4] = {static_cast<uint8_t>(v & 0xFF),
                        static_cast<uint8_t>((v >> 8) & 0xFF),
                        static_cast<uint8_t>((v >> 16) & 0xFF),
                        static_cast<uint8_t>((v >> 24) & 0xFF)};
        std::fwrite(b, 1, 4, f);
    }

    static uint32_t compute_crc32(const void* data, size_t size) {
        static constexpr uint32_t CRC_TABLE[256] = {
            0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419,
            0x706AF48F, 0xE963A535, 0x9E6495A3, 0x0EDB8832, 0x79DCB8A4,
            0xE0D5E91E, 0x97D2D988, 0x09B64C2B, 0x7EB17CBD, 0xE7B82D07,
            0x90BF1D91, 0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
            0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7, 0x136C9856,
            0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9,
            0xFA0F3D63, 0x8D080DF5, 0x3B6E20C8, 0x4C69105E, 0xD56041E4,
            0xA2677172, 0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
            0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940, 0x32D86CE3,
            0x45DF5C75, 0xDCD60DCF, 0xABD13D59, 0x26D930AC, 0x51DE003A,
            0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423, 0xCFBA9599,
            0xB8BDA50F, 0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
            0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D, 0x76DC4190,
            0x01DB7106, 0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F,
            0x9FBFE4A5, 0xE8B8D433, 0x7807C9A2, 0x0F00F934, 0x9609A88E,
            0xE10E9818, 0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
            0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E, 0x6C0695ED,
            0x1B01A57B, 0x8208F4C1, 0xF50FC457, 0x65B0D9C6, 0x12B7E950,
            0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3,
            0xFBD44C65, 0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
            0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB, 0x4369E96A,
            0x346ED9FC, 0xAD678846, 0xDA60B8D0, 0x44042D73, 0x33031DE5,
            0xAA0A4C5F, 0xDD0D7CC9, 0x5005713C, 0x270241AA, 0xBE0B1010,
            0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
            0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17,
            0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD, 0xEDB88320, 0x9ABFB3B6,
            0x03B6E20C, 0x74B1D29A, 0xEAD54739, 0x9DD277AF, 0x04DB2615,
            0x73DC1683, 0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
            0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1, 0xF00F9344,
            0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB,
            0x196C3671, 0x6E6B06E7, 0xFED41B76, 0x89D32BE0, 0x10DA7A5A,
            0x67DD4ACC, 0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
            0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1,
            0xA6BC5767, 0x3FB506DD, 0x48B2364B, 0xD80D2BDA, 0xAF0A1B4C,
            0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55, 0x316E8EEF,
            0x4669BE79, 0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
            0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F, 0xC5BA3BBE,
            0xB2BD0B28, 0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31,
            0x2CD99E8B, 0x5BDEAE1D, 0x9B64C2B0, 0xEC63F226, 0x756AA39C,
            0x026D930A, 0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
            0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38, 0x92D28E9B,
            0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21, 0x86D3D2D4, 0xF1D4E242,
            0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1,
            0x18B74777, 0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
            0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45, 0xA00AE278,
            0xD70DD2EE, 0x4E048354, 0x3903B3C2, 0xA7672661, 0xD06016F7,
            0x4969474D, 0x3E6E77DB, 0xAED16A4A, 0xD9D65ADC, 0x40DF0B66,
            0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
            0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605,
            0xCDD70693, 0x54DE5729, 0x23D967BF, 0xB3667A2E, 0xC4614AB8,
            0x5D681B02, 0x2A6F2B94, 0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B,
            0x2D02EF8D};

        auto* ptr = static_cast<const uint8_t*>(data);
        uint32_t crc = 0xFFFFFFFF;
        for (size_t i = 0; i < size; ++i) {
            crc = (crc >> 8) ^ CRC_TABLE[(crc ^ ptr[i]) & 0xFF];
        }
        return crc ^ 0xFFFFFFFF;
    }
};

inline bool zip_begin(ZipWriter& writer, const char* path) {
    writer.file = std::fopen(path, "wb");
    writer.entries.clear();
    return writer.file != nullptr;
}

inline bool zip_add_file(ZipWriter& writer, const char* name, const void* data,
                         size_t size) {
    if (!writer.file) return false;

    std::string entry_name(name);
    for (auto& c : entry_name) {
        if (c == '\\') c = '/';
    }

    uint32_t crc = ZipWriter::compute_crc32(data, size);
    uint32_t offset = static_cast<uint32_t>(std::ftell(writer.file));
    uint16_t name_len = static_cast<uint16_t>(entry_name.size());

    auto* f = writer.file;
    ZipWriter::write_u32_le(f, ZipWriter::LOCAL_FILE_HEADER_SIG);
    ZipWriter::write_u16_le(f, ZipWriter::VERSION_NEEDED);
    ZipWriter::write_u16_le(f, 0);
    ZipWriter::write_u16_le(f, ZipWriter::METHOD_STORE);
    ZipWriter::write_u16_le(f, ZipWriter::DOS_TIME);
    ZipWriter::write_u16_le(f, ZipWriter::DOS_DATE);
    ZipWriter::write_u32_le(f, crc);
    ZipWriter::write_u32_le(f, static_cast<uint32_t>(size));
    ZipWriter::write_u32_le(f, static_cast<uint32_t>(size));
    ZipWriter::write_u16_le(f, name_len);
    ZipWriter::write_u16_le(f, 0);
    std::fwrite(entry_name.data(), 1, name_len, f);

    if (size > 0) {
        std::fwrite(data, 1, size, f);
    }

    writer.entries.push_back(
        {entry_name, crc, static_cast<uint32_t>(size), offset});
    return true;
}

inline bool zip_end(ZipWriter& writer) {
    if (!writer.file) return false;

    auto* f = writer.file;
    uint32_t cd_offset = static_cast<uint32_t>(std::ftell(f));

    for (const auto& e : writer.entries) {
        uint16_t name_len = static_cast<uint16_t>(e.name.size());

        ZipWriter::write_u32_le(f, ZipWriter::CENTRAL_DIR_HEADER_SIG);
        ZipWriter::write_u16_le(f, ZipWriter::VERSION_NEEDED);
        ZipWriter::write_u16_le(f, ZipWriter::VERSION_NEEDED);
        ZipWriter::write_u16_le(f, 0);
        ZipWriter::write_u16_le(f, ZipWriter::METHOD_STORE);
        ZipWriter::write_u16_le(f, ZipWriter::DOS_TIME);
        ZipWriter::write_u16_le(f, ZipWriter::DOS_DATE);
        ZipWriter::write_u32_le(f, e.crc32);
        ZipWriter::write_u32_le(f, e.size);
        ZipWriter::write_u32_le(f, e.size);
        ZipWriter::write_u16_le(f, name_len);
        ZipWriter::write_u16_le(f, 0);
        ZipWriter::write_u16_le(f, 0);
        ZipWriter::write_u16_le(f, 0);
        ZipWriter::write_u16_le(f, 0);
        ZipWriter::write_u32_le(f, 0);
        ZipWriter::write_u32_le(f, e.local_header_offset);
        std::fwrite(e.name.data(), 1, name_len, f);
    }

    uint32_t cd_size = static_cast<uint32_t>(std::ftell(f)) - cd_offset;
    uint16_t entry_count = static_cast<uint16_t>(writer.entries.size());

    ZipWriter::write_u32_le(f, ZipWriter::END_CENTRAL_DIR_SIG);
    ZipWriter::write_u16_le(f, 0);
    ZipWriter::write_u16_le(f, 0);
    ZipWriter::write_u16_le(f, entry_count);
    ZipWriter::write_u16_le(f, entry_count);
    ZipWriter::write_u32_le(f, cd_size);
    ZipWriter::write_u32_le(f, cd_offset);
    ZipWriter::write_u16_le(f, 0);

    std::fclose(f);
    writer.file = nullptr;
    writer.entries.clear();
    return true;
}

#endif

}  // namespace detail

class DocxWriter {
   public:
    void add_line(std::string text) { lines_.push_back(std::move(text)); }

    void add_table(std::vector<std::vector<std::string>> rows) {
        tables_.push_back(std::move(rows));
    }

    void set_font(std::string name) { font_name_ = std::move(name); }
    void set_font_size(int pt) { font_size_pt_ = pt; }

    bool save(const char* path) const {
#ifdef ZUC_USE_MINIZ
        mz_zip_archive zip{};
#else
        detail::ZipWriter zip{};
#endif
        if (!detail::zip_begin(zip, path)) return false;

        auto add = [&](const char* name, const std::string& s) {
            return detail::zip_add_file(zip, name, s.data(), s.size());
        };

        if (!add("[Content_Types].xml", CT)) return false;
        if (!add("_rels/.rels", RELS)) return false;
        if (!add("word/document.xml", build_document())) return false;
        if (!add("word/_rels/document.xml.rels", DOC_RELS)) return false;
        if (!add("word/styles.xml", build_styles())) return false;
        return detail::zip_end(zip);
    }

   private:
    std::vector<std::string> lines_;
    std::vector<std::vector<std::vector<std::string>>> tables_;
    std::string font_name_ = "Calibri";
    int font_size_pt_ = 11;

    static std::string xml_escape(std::string_view s) {
        std::string r;
        r.reserve(s.size());
        for (char c : s) {
            switch (c) {
                case '&':
                    r.append("&amp;");
                    break;
                case '<':
                    r.append("&lt;");
                    break;
                case '>':
                    r.append("&gt;");
                    break;
                default:
                    r.push_back(c);
                    break;
            }
        }
        return r;
    }

    static constexpr auto CT =
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<Types "
        "xmlns=\"http://schemas.openxmlformats.org/package/2006/"
        "content-types\">\n"
        "<Override PartName=\"/word/document.xml\" "
        "ContentType=\"application/"
        "vnd.openxmlformats-officedocument.wordprocessingml.document.main+"
        "xml\"/>\n"
        "<Override PartName=\"/word/styles.xml\" "
        "ContentType=\"application/"
        "vnd.openxmlformats-officedocument.wordprocessingml.styles+xml\"/>\n"
        "<Default Extension=\"rels\" "
        "ContentType=\"application/"
        "vnd.openxmlformats-package.relationships+xml\"/>\n"
        "<Default Extension=\"xml\" ContentType=\"application/xml\"/>\n"
        "</Types>";

    static constexpr auto RELS =
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<Relationships "
        "xmlns=\"http://schemas.openxmlformats.org/package/2006/"
        "relationships\">\n"
        "<Relationship Id=\"rId1\" "
        "Type=\"http://schemas.openxmlformats.org/officeDocument/2006/"
        "relationships/officeDocument\" Target=\"word/document.xml\"/>\n"
        "</Relationships>";

    static constexpr auto DOC_RELS =
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
        "<Relationships "
        "xmlns=\"http://schemas.openxmlformats.org/package/2006/"
        "relationships\">\n"
        "<Relationship Id=\"rId1\" "
        "Type=\"http://schemas.openxmlformats.org/officeDocument/2006/"
        "relationships/styles\" Target=\"styles.xml\"/>\n"
        "</Relationships>";

    std::string build_styles() const {
        int half_pt = font_size_pt_ * 2;
        auto val = std::to_string(half_pt);
        auto& fn = font_name_;
        return "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
               "<w:styles "
               "xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/"
               "2006/main\">\n"
               "<w:docDefaults><w:rPrDefault><w:rPr>"
               "<w:rFonts w:ascii=\"" +
               fn + "\" w:hAnsi=\"" + fn + "\" w:eastAsia=\"" + fn +
               "\" w:cs=\"" + fn +
               "\"/>"
               "<w:sz w:val=\"" +
               val + "\"/><w:szCs w:val=\"" + val +
               "\"/>"
               "</w:rPr></w:rPrDefault></w:docDefaults>\n"
               "</w:styles>";
    }

    static constexpr auto TBL_BORDERS =
        "<w:tblBorders>"
        "<w:top w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"auto\"/>"
        "<w:left w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"auto\"/>"
        "<w:bottom w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"auto\"/>"
        "<w:right w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"auto\"/>"
        "<w:insideH w:val=\"single\" w:sz=\"4\" w:space=\"0\" "
        "w:color=\"auto\"/>"
        "<w:insideV w:val=\"single\" w:sz=\"4\" w:space=\"0\" "
        "w:color=\"auto\"/>"
        "</w:tblBorders>";

    std::string build_document() const {
        std::string doc =
            "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
            "<w:document "
            "xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/"
            "main\">\n"
            "<w:body>\n";
        for (const auto& line : lines_) {
            doc.append("<w:p><w:r><w:t");
            if (!line.empty() && (line.front() == ' ' || line.back() == ' '))
                doc.append(" xml:space=\"preserve\"");
            doc.push_back('>');
            doc.append(xml_escape(line));
            doc.append("</w:t></w:r></w:p>\n");
        }
        for (const auto& table : tables_) {
            doc.append("<w:tbl><w:tblPr><w:tblW w:w=\"0\" w:type=\"auto\"/>");
            doc.append(TBL_BORDERS);
            doc.append("</w:tblPr>");
            for (const auto& row : table) {
                doc.append("<w:tr>");
                for (const auto& cell : row) {
                    doc.append("<w:tc><w:p><w:r><w:t");
                    if (!cell.empty() &&
                        (cell.front() == ' ' || cell.back() == ' '))
                        doc.append(" xml:space=\"preserve\"");
                    doc.push_back('>');
                    doc.append(xml_escape(cell));
                    doc.append("</w:t></w:r></w:p></w:tc>");
                }
                doc.append("</w:tr>");
            }
            doc.append("</w:tbl>\n");
        }
        doc.append("</w:body>\n</w:document>");
        return doc;
    }
};

}  // namespace zuc