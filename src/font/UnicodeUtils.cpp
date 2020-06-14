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

