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

#ifndef SRC_FONT_FONT_H_
#define SRC_FONT_FONT_H_

#include <map>
#include <exception>
#include <iostream>
#include <unicode/schriter.h>
#include "Character.h"

class Font {
public:
    class CharNotFoundException : public std::exception
    {
    private:
        const Font* f;
        UChar32 ucode;
        std::string message;
    public:
        CharNotFoundException(const Font* f, UChar32 ucode) :
            f(f), ucode(ucode) {
            this->message = "Character " + std::to_string(ucode) +
                    " not found in font " + f->getName();
        }

        const char* what() const throw () {
            return this->message.c_str();
        }
        const Font* getFont() const { return f; };
        const UChar32 getUTF8Code() const { return ucode; };
    };

private:
    std::map<UChar32, Character*> chars;
    std::string name;

public:
    Font(const std::string& name);
    Font(const std::string& name, std::initializer_list<Character*> chars);
    virtual ~Font();

    std::string getName() const;

    Character* get(const UChar32 index) const;
    void addCharacter(Character* character);
    bool removeCharacter(UChar32 index);
    bool removeCharacter(Character* character);
    unsigned int getNbCharacters();
};

#endif /* SRC_FONT_H_ */
