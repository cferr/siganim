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

#ifndef SRC_FONT_UNICODEUTILS_H_
#define SRC_FONT_UNICODEUTILS_H_

#include <unicode/uchar.h>
#include <string>

#define NB_UNICODE_BLOCKS 202

struct UnicodeBlock {
    UChar32 start;
    UChar32 end;
    const char* name;
};

extern struct UnicodeBlock unicodeBlocks[NB_UNICODE_BLOCKS];

std::string UnicodeCharName(UChar32 code);
std::string UnicodeBlockName(unsigned int block);
unsigned int UnicodeBlockPointCount(unsigned int block);
UChar32 UnicodeBlockStart(unsigned int block);
UChar32 UnicodeNextPrintableChar(UChar32 from);
UChar32 UnicodeNthPrintableChar(unsigned int block, unsigned int n);

#endif /* SRC_FONT_UNICODEUTILS_H_ */
