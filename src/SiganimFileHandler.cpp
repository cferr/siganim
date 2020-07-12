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

#include <json-c/json.h>
#include "SiganimFileHandler.h"
#include "font/parsers/SiganimJSONFontsParser.h"

SiganimFileHandler::SiganimFileHandler() {
    this->fonts = new FontSet();
}

SiganimFileHandler::SiganimFileHandler(const char *location) :
        SiganimFileHandler() {
    this->location = std::string(location);
    this->parseInputFile();
}

SiganimFileHandler::SiganimFileHandler(const std::string &location) :
        SiganimFileHandler() {
    this->location = location;
    this->parseInputFile();
}

SiganimFileHandler::~SiganimFileHandler() {
    delete this->fonts;
}

void SiganimFileHandler::setLocation(const std::string &location) {
    this->location = location;
}

std::string SiganimFileHandler::getLocation() const {
    return this->location;
}

Sign* SiganimFileHandler::getSign(unsigned int number) const {
    return this->signs.at(number);
}

void SiganimFileHandler::addSign(Sign *s) {
    this->signs.push_back(s);
}

void SiganimFileHandler::removeSign(unsigned int number) {
    // TODO
}

void SiganimFileHandler::removeSign(Sign *s) {
    for(auto i = this->signs.begin(); i < this->signs.end(); ++i)
        if(*i == s) {
            i = this->signs.erase(i);
        }
}

FontSet* SiganimFileHandler::getFontSet() const {
    return this->fonts;
}

void SiganimFileHandler::parseInputFile() {
    if(!this->location.empty()) {
        json_object* document = json_object_from_file(this->location.c_str());
        if(document != nullptr) {
            enum json_type type = json_object_get_type(document);
            if(type == json_type::json_type_object) {
                // Parse fonts
                json_object* json_fonts =
                        json_object_object_get(document, "fonts");
                if(json_fonts != nullptr) {
                    std::vector<Font*> fontsParsed =
                            SiganimJSONFontsParser::parseJSON(json_fonts);
                    this->fonts->addFonts(fontsParsed);
                }

                // TODO: parse sign
            }

            json_object_put(document);
        }
    }
}

bool SiganimFileHandler::save() {
    json_object* document = json_object_new_object();
    json_object_object_add(document, "fonts", this->fonts->toJSON());

    // TODO add signs

    int save_status = json_object_to_file_ext(this->location.c_str(), document,
            JSON_C_TO_STRING_PRETTY);

    json_object_put(document);
    if(save_status != 0) {
        return false;
    } else return true;
}
