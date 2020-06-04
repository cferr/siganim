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

#ifndef SRC_SIGN_CELLS_TEXT_H_
#define SRC_SIGN_CELLS_TEXT_H_

#include <iostream>
#include <unicode/utypes.h>
#include <unicode/unistr.h>

#include "../SignCell.h"
#include "../SignImage.h"
#include "../SignTreeVisitor.h"

#include "../../font/Font.h"

class Text: public SignCell {
public:
    enum HorizontalAlignment {
        HALIGN_LEFT, HALIGN_CENTER, HALIGN_RIGHT, HALIGN_JUSTIFY
    };

    enum VerticalAlignment {
        // biggest letter on top
        VALIGN_TOP_TOP, VALIGN_TOP_CENTER, VALIGN_TOP_BOTTOM,
        // biggest letter centered
        VALIGN_CENTER_TOP, VALIGN_CENTER_CENTER, VALIGN_CENTER_BOTTOM,
        // biggest letter bottom
        VALIGN_BOTTOM_TOP, VALIGN_BOTTOM_CENTER, VALIGN_BOTTOM_BOTTOM
    };

private:
    SignColor foreground;     // Foreground color

    icu::UnicodeString* text; // Unicode-encoded text
    const Font* font;         // One font only per text cell

    enum HorizontalAlignment hAlign;
    enum VerticalAlignment vAlign;

public:
    Text(const Font* font, const enum HorizontalAlignment hAlign,
            const enum VerticalAlignment vAlign,
            const icu::UnicodeString& text = "");
    Text(const Font* font, const enum HorizontalAlignment hAlign,
                const enum VerticalAlignment vAlign,
                const icu::UnicodeString& text,
                const SignColor& color);
    virtual ~Text();

    virtual const char* CellTypeStr() const {
        return "Text";
    }

    virtual void accept(SignTreeVisitor &visitor);
    virtual void accept(ConstSignTreeVisitor &visitor) const;
    virtual void callbackDispatch(SignTreeStructureObserver* s) const;

    virtual unsigned int getHeight() const;
    virtual unsigned int getWidth() const;
    virtual unsigned int getChildHeight(const SignCell* child) const override;
    virtual unsigned int getChildWidth(const SignCell* child) const override;

    void setText(const icu::UnicodeString& text);
    icu::UnicodeString* getText() const;

    const Font* getFont() const;

    void setForegroundColor(const SignColor& foreground);
    const SignColor getForegroundColor() const;

    enum HorizontalAlignment getHAlign() const;
    enum VerticalAlignment getVAlign() const;
    void setHAlign(enum HorizontalAlignment hAlign);
    void setVAlign(enum VerticalAlignment vAlign);

    virtual std::ostream& serialize(std::ostream &strm) const;

};

std::ostream& operator<<(std::ostream &strm, const Text &s);

#endif /* SRC_SIGN_CELLS_TEXT_H_ */
