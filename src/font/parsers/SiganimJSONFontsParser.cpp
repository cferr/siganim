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

extern "C" {
#include "../../../base64.c/base64.h"
}
#include "SiganimJSONFontsParser.h"
#include "../Font.h"
#include "../Character.h"

std::vector<Font*> SiganimJSONFontsParser::parseJSON(json_object *obj) {
    std::vector<Font*> ret;

    if(json_object_is_type(obj, json_type::json_type_array) == 1) {
        int nbFonts = json_object_array_length(obj);
        for(int i = 0; i < nbFonts; ++i) {
            json_object* font_obj = json_object_array_get_idx(obj, i);
            if(font_obj == nullptr)
               break;
            json_object* family_obj;
            if(!json_object_object_get_ex(font_obj, "family", &family_obj))
                break;
            const char* family = json_object_get_string(family_obj);
            json_object* style_obj;
            Font* font;
            if(!json_object_object_get_ex(font_obj, "style", &style_obj))
                font = new Font(family);
            else {
                const char* style = json_object_get_string(style_obj);
                font = new Font(family, style);
            }

            json_object* chars_obj;
            if(!json_object_object_get_ex(font_obj, "chars", &chars_obj)||
                json_object_is_type(chars_obj, json_type::json_type_array) == 0)
                break;
            int nbChars = json_object_array_length(chars_obj);
            for(int j = 0; j < nbChars; ++j) {
                json_object* char_obj = json_object_array_get_idx(chars_obj, j);
                if(char_obj == nullptr)
                    break;
                json_object* height_obj;
                if(!json_object_object_get_ex(char_obj, "height", &height_obj))
                    break;
                json_object* width_obj;
                if(!json_object_object_get_ex(char_obj, "width", &width_obj))
                    break;
                json_object* UTF8Code_obj;
                if(!json_object_object_get_ex(char_obj, "UTF8Code",
                        &UTF8Code_obj))
                    break;
                unsigned int width = json_object_get_int(width_obj);
                unsigned int height = json_object_get_int(height_obj);
                UChar32 UTF8Code = json_object_get_int(UTF8Code_obj);

                json_object* map_obj;
                if(!json_object_object_get_ex(char_obj, "map", &map_obj))
                    break;
                const unsigned char* map_b64 =
                        (const unsigned char*)json_object_get_string(map_obj);
                unsigned int map_b64_len = json_object_get_string_len(map_obj);

                unsigned int map_len = b64d_size(map_b64_len);
                unsigned char* map = (unsigned char*)calloc((1 + map_len),
                        sizeof(unsigned char));

                b64_decode(map_b64, map_b64_len, map);

                Character* c = new Character(UTF8Code, width, height,
                        (enum Character::Bit*)map);
                free(map);

                font->addCharacter(c);
            }

            ret.push_back(font);
        }
    }

    return ret;

}
