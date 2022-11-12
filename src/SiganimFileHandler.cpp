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
#include "sign/SiganimJSONSignParser.h"

SiganimFileHandler::SiganimFileHandler() : sign(nullptr) {
    this->fonts = new FontSet();
    this->rasterizers = new RasterizerSet();
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
    delete this->rasterizers;
}

void SiganimFileHandler::setLocation(const std::string &location) {
    this->location = location;
}

std::string SiganimFileHandler::getLocation() const {
    return this->location;
}

Sign* SiganimFileHandler::getSign() const {
    return this->sign;
}

void SiganimFileHandler::setSign(Sign* s) {
    this->sign = s;
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

                json_object* json_sign =
                        json_object_object_get(document, "sign");
                if(json_sign != nullptr) {
                    this->sign =
                            SiganimJSONSignParser::parseJSON(json_sign);
                }
            }

            json_object_put(document);
        }
    }
}

bool SiganimFileHandler::save() {
    if(this->location.empty()) {
        throw EmptyFileLocationException();
    }
    json_object* document = json_object_new_object();
    if(!this->fonts->getFonts().empty())
        json_object_object_add(document, "fonts", this->fonts->toJSON());
    if(this->sign != nullptr)
        json_object_object_add(document, "sign", this->sign->toJSON());

    int save_status = json_object_to_file_ext(this->location.c_str(), document,
            JSON_C_TO_STRING_PRETTY);

    json_object_put(document);
    if(save_status != 0) {
        return false;
    } else return true;
}

RasterizerSet* SiganimFileHandler::getRasterizerSet() const {
    return this->rasterizers;
}
