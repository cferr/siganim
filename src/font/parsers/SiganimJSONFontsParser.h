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

#ifndef FONT_PARSERS_SIGANIMJSONFONTSPARSER_H_
#define FONT_PARSERS_SIGANIMJSONFONTSPARSER_H_

#include <vector>
#include <json-c/json.h>
#include "../Font.h"

class SiganimJSONFontsParser {
public:
    virtual ~SiganimJSONFontsParser() {};

    static std::vector<Font*> parseJSON(json_object* obj);
};

#endif /* FONT_PARSERS_SIGANIMJSONFONTSPARSER_H_ */
