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

#ifndef SRC_FONT_FONTSET_H_
#define SRC_FONT_FONTSET_H_

#include <vector>
#include "Font.h"

class FontSet {
public:
    class FontNotFoundException : public std::exception
    {
    private:
        const FontSet* fset;
        std::string family;
        std::string style;
        std::string message;
    public:
        FontNotFoundException(const FontSet* fset, std::string family,
                std::string style) :
            fset(fset), family(family), style(style) {
            this->message = "Font " + family + " " + style + " not found";
        }

        const char* what() const throw () {
            return this->message.c_str();
        }

        const FontSet* getFontSet() const { return fset; };
        const std::string getFamily() const { return family; };
        const std::string getStyle() const { return style; };
    };

private:
    std::vector<Font*> fonts;

public:
    FontSet();
    FontSet(std::initializer_list<Font*> fonts);
    virtual ~FontSet();

    void addFont(Font* font);
    void addFonts(std::vector<Font*> fonts);
    bool removeFont(std::string family, std::string style);
    bool removeFont(Font* f);
    std::vector<Font*> getFonts();

    std::vector<Font*> multiLookup(std::string family, std::string style);
    Font* lookup(std::string family, std::string style);
};

#endif /* SRC_FONT_FONTSET_H_ */
