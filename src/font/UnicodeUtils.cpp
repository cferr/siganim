// Siganim - Sign Editor and Animator
// Copyright (C) 2020 Corentin Ferry
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <cstdint>
#include "UnicodeUtils.h"

struct UnicodeBlock unicodeBlocks[NB_UNICODE_BLOCKS] = {
    { 0x0, 0x0, "Invalid Block" },
    { 0x0, 0x7F, "Basic Latin" },
    { 0x80, 0xFF, "Latin-1 Supplement" },
    { 0x100, 0x17F, "Latin Extended-A" },
    { 0x180, 0x24F, "Latin Extended-B" },
    { 0x250, 0x2AF, "IPA Extensions" },
    { 0x2B0, 0x2FF, "Spacing Modifier Letters" },
    { 0x300, 0x36F, "Combining Diacritical Marks" },
    { 0x370, 0x3FF, "Greek and Coptic" },
    { 0x400, 0x4FF, "Cyrillic" },
    { 0x500, 0x527, "Cyrillic Supplement" },
    { 0x531, 0x58A, "Armenian" },
    { 0x591, 0x5F4, "Hebrew" },
    { 0x600, 0x6FF, "Arabic" },
    { 0x700, 0x74F, "Syriac" },
    { 0x750, 0x77F, "Arabic Supplement" },
    { 0x780, 0x7B1, "Thaana" },
    { 0x7C0, 0x7FA, "NKo" },
    { 0x800, 0x83E, "Samaritan" },
    { 0x840, 0x85E, "Mandaic" },
    { 0x900, 0x97F, "Devanagari" },
    { 0x981, 0x9FB, "Bengali" },
    { 0xA01, 0xA75, "Gurmukhi" },
    { 0xA81, 0xAF1, "Gujarati" },
    { 0xB01, 0xB77, "Oriya" },
    { 0xB82, 0xBFA, "Tamil" },
    { 0xC01, 0xC7F, "Telugu" },
    { 0xC82, 0xCF2, "Kannada" },
    { 0xD02, 0xD7F, "Malayalam" },
    { 0xD82, 0xDF4, "Sinhala" },
    { 0xE01, 0xE5B, "Thai" },
    { 0xE81, 0xEDD, "Lao" },
    { 0xF00, 0xFDA, "Tibetan" },
    { 0x1000, 0x109F, "Myanmar" },
    { 0x10A0, 0x10FC, "Georgian" },
    { 0x1100, 0x11FF, "Hangul Jamo" },
    { 0x1200, 0x137C, "Ethiopic" },
    { 0x1380, 0x1399, "Ethiopic Supplement" },
    { 0x13A0, 0x13F4, "Cherokee" },
    { 0x1400, 0x167F, "Unified Canadian Aboriginal Syllabics" },
    { 0x1680, 0x169C, "Ogham" },
    { 0x16A0, 0x16F0, "Runic" },
    { 0x1700, 0x1714, "Tagalog" },
    { 0x1720, 0x1736, "Hanunoo" },
    { 0x1740, 0x1753, "Buhid" },
    { 0x1760, 0x1773, "Tagbanwa" },
    { 0x1780, 0x17F9, "Khmer" },
    { 0x1800, 0x18AA, "Mongolian" },
    { 0x18B0, 0x18F5, "Unified Canadian Aboriginal Syllabics Extended" },
    { 0x1900, 0x194F, "Limbu" },
    { 0x1950, 0x1974, "Tai Le" },
    { 0x1980, 0x19DF, "New Tai Lue" },
    { 0x19E0, 0x19FF, "Khmer Symbols" },
    { 0x1A00, 0x1A1F, "Buginese" },
    { 0x1A20, 0x1AAD, "Tai Tham" },
    { 0x1B00, 0x1B7C, "Balinese" },
    { 0x1B80, 0x1BB9, "Sundanese" },
    { 0x1BC0, 0x1BFF, "Batak" },
    { 0x1C00, 0x1C4F, "Lepcha" },
    { 0x1C50, 0x1C7F, "Ol Chiki" },
    { 0x1CD0, 0x1CF2, "Vedic Extensions" },
    { 0x1D00, 0x1D7F, "Phonetic Extensions" },
    { 0x1D80, 0x1DBF, "Phonetic Extensions Supplement" },
    { 0x1DC0, 0x1DFF, "Combining Diacritical Marks Supplement" },
    { 0x1E00, 0x1EFF, "Latin Extended Additional" },
    { 0x1F00, 0x1FFE, "Greek Extended" },
    { 0x2000, 0x206F, "General Punctuation" },
    { 0x2070, 0x209C, "Superscripts and Subscripts" },
    { 0x20A0, 0x20B9, "Currency Symbols" },
    { 0x20D0, 0x20F0, "Combining Diacritical Marks for Symbols" },
    { 0x2100, 0x214F, "Letterlike Symbols" },
    { 0x2150, 0x2189, "Number Forms" },
    { 0x2190, 0x21FF, "Arrows" },
    { 0x2200, 0x22FF, "Mathematical Operators" },
    { 0x2300, 0x23F3, "Miscellaneous Technical" },
    { 0x2400, 0x2426, "Control Pictures" },
    { 0x2440, 0x244A, "Optical Character Recognition" },
    { 0x2460, 0x24FF, "Enclosed Alphanumerics" },
    { 0x2500, 0x257F, "Box Drawing" },
    { 0x2580, 0x259F, "Block Elements" },
    { 0x25A0, 0x25FF, "Geometric Shapes" },
    { 0x2600, 0x26FF, "Miscellaneous Symbols" },
    { 0x2701, 0x27BF, "Dingbats" },
    { 0x27C0, 0x27EF, "Miscellaneous Mathematical Symbols-A" },
    { 0x27F0, 0x27FF, "Supplemental Arrows-A" },
    { 0x2800, 0x28FF, "Braille Patterns" },
    { 0x2900, 0x297F, "Supplemental Arrows-B" },
    { 0x2980, 0x29FF, "Miscellaneous Mathematical Symbols-B" },
    { 0x2A00, 0x2AFF, "Supplemental Mathematical Operators" },
    { 0x2B00, 0x2B59, "Miscellaneous Symbols and Arrows" },
    { 0x2C00, 0x2C5E, "Glagolitic" },
    { 0x2C60, 0x2C7F, "Latin Extended-C" },
    { 0x2C80, 0x2CFF, "Coptic" },
    { 0x2D00, 0x2D25, "Georgian Supplement" },
    { 0x2D30, 0x2D7F, "Tifinagh" },
    { 0x2D80, 0x2DDE, "Ethiopic Extended" },
    { 0x2DE0, 0x2DFF, "Cyrillic Extended-A" },
    { 0x2E00, 0x2E31, "Supplemental Punctuation" },
    { 0x2E80, 0x2EF3, "CJK Radicals Supplement" },
    { 0x2F00, 0x2FD5, "Kangxi Radicals" },
    { 0x2FF0, 0x2FFB, "Ideographic Description Characters" },
    { 0x3000, 0x303F, "CJK Symbols and Punctuation" },
    { 0x3041, 0x309F, "Hiragana" },
    { 0x30A0, 0x30FF, "Katakana" },
    { 0x3105, 0x312D, "Bopomofo" },
    { 0x3131, 0x318E, "Hangul Compatibility Jamo" },
    { 0x3190, 0x319F, "Kanbun" },
    { 0x31A0, 0x31BA, "Bopomofo Extended" },
    { 0x31C0, 0x31E3, "CJK Strokes" },
    { 0x31F0, 0x31FF, "Katakana Phonetic Extensions" },
    { 0x3200, 0x32FE, "Enclosed CJK Letters and Months" },
    { 0x3300, 0x33FF, "CJK Compatibility" },
    { 0x3400, 0x4DB5, "CJK Unified Ideographs Extension A" },
    { 0x4DC0, 0x4DFF, "Yijing Hexagram Symbols" },
    { 0x4E00, 0x9FCB, "CJK Unified Ideographs" },
    { 0xA000, 0xA48C, "Yi Syllables" },
    { 0xA490, 0xA4C6, "Yi Radicals" },
    { 0xA4D0, 0xA4FF, "Lisu" },
    { 0xA500, 0xA62B, "Vai" },
    { 0xA640, 0xA697, "Cyrillic Extended-B" },
    { 0xA6A0, 0xA6F7, "Bamum" },
    { 0xA700, 0xA71F, "Modifier Tone Letters" },
    { 0xA720, 0xA7FF, "Latin Extended-D" },
    { 0xA800, 0xA82B, "Syloti Nagri" },
    { 0xA830, 0xA839, "Common Indic Number Forms" },
    { 0xA840, 0xA877, "Phags-pa" },
    { 0xA880, 0xA8D9, "Saurashtra" },
    { 0xA8E0, 0xA8FB, "Devanagari Extended" },
    { 0xA900, 0xA92F, "Kayah Li" },
    { 0xA930, 0xA95F, "Rejang" },
    { 0xA960, 0xA97C, "Hangul Jamo Extended-A" },
    { 0xA980, 0xA9DF, "Javanese" },
    { 0xAA00, 0xAA5F, "Cham" },
    { 0xAA60, 0xAA7B, "Myanmar Extended-A" },
    { 0xAA80, 0xAADF, "Tai Viet" },
    { 0xAB01, 0xAB2E, "Ethiopic Extended-A" },
    { 0xABC0, 0xABF9, "Meetei Mayek" },
    { 0xAC00, 0xD7A3, "Hangul Syllables" },
    { 0xD7B0, 0xD7FB, "Hangul Jamo Extended-B" },
    { 0xD800, 0xDB7F, "High Surrogates" },
    { 0xDB80, 0xDBFF, "High Private Use Surrogates" },
    { 0xDC00, 0xDFFF, "Low Surrogates" },
    { 0xE000, 0xF8FF, "Private Use Area" },
    { 0xF900, 0xFAD9, "CJK Compatibility Ideographs" },
    { 0xFB00, 0xFB4F, "Alphabetic Presentation Forms" },
    { 0xFB50, 0xFDFD, "Arabic Presentation Forms-A" },
    { 0xFE00, 0xFE0F, "Variation Selectors" },
    { 0xFE10, 0xFE19, "Vertical Forms" },
    { 0xFE20, 0xFE26, "Combining Half Marks" },
    { 0xFE30, 0xFE4F, "CJK Compatibility Forms" },
    { 0xFE50, 0xFE6B, "Small Form Variants" },
    { 0xFE70, 0xFEFF, "Arabic Presentation Forms-B" },
    { 0xFF01, 0xFFEE, "Halfwidth and Fullwidth Forms" },
    { 0xFFF9, 0xFFFD, "Specials" },
    { 0x10000, 0x1005D, "Linear B Syllabary" },
    { 0x10080, 0x100FA, "Linear B Ideograms" },
    { 0x10100, 0x1013F, "Aegean Numbers" },
    { 0x10140, 0x1018A, "Ancient Greek Numbers" },
    { 0x10190, 0x1019B, "Ancient Symbols" },
    { 0x101D0, 0x101FD, "Phaistos Disc" },
    { 0x10280, 0x1029C, "Lycian" },
    { 0x102A0, 0x102D0, "Carian" },
    { 0x10300, 0x10323, "Old Italic" },
    { 0x10330, 0x1034A, "Gothic" },
    { 0x10380, 0x1039F, "Ugaritic" },
    { 0x103A0, 0x103D5, "Old Persian" },
    { 0x10400, 0x1044F, "Deseret" },
    { 0x10450, 0x1047F, "Shavian" },
    { 0x10480, 0x104A9, "Osmanya" },
    { 0x10800, 0x1083F, "Cypriot Syllabary" },
    { 0x10840, 0x1085F, "Imperial Aramaic" },
    { 0x10900, 0x1091F, "Phoenician" },
    { 0x10920, 0x1093F, "Lydian" },
    { 0x10A00, 0x10A58, "Kharoshthi" },
    { 0x10A60, 0x10A7F, "Old South Arabian" },
    { 0x10B00, 0x10B3F, "Avestan" },
    { 0x10B40, 0x10B5F, "Inscriptional Parthian" },
    { 0x10B60, 0x10B7F, "Inscriptional Pahlavi" },
    { 0x10C00, 0x10C48, "Old Turkic" },
    { 0x10E60, 0x10E7E, "Rumi Numeral Symbols" },
    { 0x11000, 0x1106F, "Brahmi" },
    { 0x11080, 0x110C1, "Kaithi" },
    { 0x12000, 0x1236E, "Cuneiform" },
    { 0x12400, 0x12473, "Cuneiform Numbers and Punctuation" },
    { 0x13000, 0x1342E, "Egyptian Hieroglyphs" },
    { 0x16800, 0x16A38, "Bamum Supplement" },
    { 0x1B000, 0x1B001, "Kana Supplement" },
    { 0x1D000, 0x1D0F5, "Byzantine Musical Symbols" },
    { 0x1D100, 0x1D1DD, "Musical Symbols" },
    { 0x1D200, 0x1D245, "Ancient Greek Musical Notation" },
    { 0x1D300, 0x1D356, "Tai Xuan Jing Symbols" },
    { 0x1D360, 0x1D371, "Counting Rod Numerals" },
    { 0x1D400, 0x1D7FF, "Mathematical Alphanumeric Symbols" },
    { 0x1F000, 0x1F02B, "Mahjong Tiles" },
    { 0x1F030, 0x1F093, "Domino Tiles" },
    { 0x1F0A0, 0x1F0DF, "Playing Cards" },
    { 0x1F100, 0x1F1FF, "Enclosed Alphanumeric Supplement" },
    { 0x1F200, 0x1F251, "Enclosed Ideographic Supplement" },
    { 0x1F300, 0x1F5FF, "Miscellaneous Symbols And Pictographs" },
    { 0x1F601, 0x1F64F, "Emoticons" },
    { 0x1F680, 0x1F6C5, "Transport And Map Symbols" },
    { 0x1F700, 0x1F773, "Alchemical Symbols" }
};


std::string UnicodeCharName(UChar32 code) {
    uint32_t charNameLength = 30;
    char* buffer = (char*)malloc((charNameLength + 1) * sizeof(char));
    buffer[charNameLength] = 0;
    UErrorCode err = U_ZERO_ERROR;
    do {
        err = U_ZERO_ERROR;
        int32_t nameLen = u_charName(code, U_UNICODE_CHAR_NAME, buffer,
                charNameLength, &err);
        if(err == U_BUFFER_OVERFLOW_ERROR) {
            charNameLength = nameLen + 1;
            buffer = (char*)realloc(buffer,
                    (charNameLength + 1) * sizeof(char));
            buffer[charNameLength] = 0;
        }

    } while(err == U_BUFFER_OVERFLOW_ERROR);

    std::string ret = "";
    if(U_SUCCESS(err)) {
        ret = std::string(buffer);
    }
    free(buffer);

    return ret; // An exception or error code would be better than an
    // empty string.
}

unsigned int UnicodeBlockPointCount(unsigned int block) {
    return unicodeBlocks[block].end - unicodeBlocks[block].start;
}

UChar32 UnicodeBlockStart(unsigned int block) {
    return unicodeBlocks[block].start;
}

std::string UnicodeBlockName(unsigned int block) {
    return std::string(unicodeBlocks[block].name);
}

UChar32 UnicodeNextPrintableChar(UChar32 from) {
    UChar32 i = from;
    while(!u_isgraph(i))
        i++;
    return i;
}

UChar32 UnicodeNthPrintableChar(unsigned int block, unsigned int n) {
    UChar32 i = unicodeBlocks[block].start;
    unsigned int printable = 0;
    do {
        if(u_isgraph(i))
            printable++;
        i++;
    } while(i < unicodeBlocks[block].end && printable <= n);

    return i-1;
}
