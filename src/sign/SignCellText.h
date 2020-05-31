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

#ifndef SRC_SIGNCELLTEXT_H_
#define SRC_SIGNCELLTEXT_H_

#include <iostream>
#include <unicode/utypes.h>
#include <unicode/unistr.h>

#include "SignCell.h"
#include "SignImage.h"
#include "SignTreeVisitor.h"

#include "../font/Font.h"

class SignCellText: public SignCell {
private:
    SignColor background;     // Background color
    SignColor foreground;     // Foreground color

    icu::UnicodeString* text; // Unicode-encoded text

    const Font* font;               // One font only per text cell

public:
    SignCellText(const Font* font, const icu::UnicodeString& text = "");
    virtual ~SignCellText();

    Type getType() const;
    virtual void accept(SignTreeVisitor &visitor);

    virtual unsigned int getHeight() const;
    virtual unsigned int getWidth() const;
    virtual unsigned int getChildHeight(const SignCell* child) const override;
    virtual unsigned int getChildWidth(const SignCell* child) const override;


    void setText(const icu::UnicodeString& text);
    icu::UnicodeString* getText() const;

    const Font* getFont() const;

    const SignColor getForegroundColor() const;
    const SignColor getBackgroundColor() const;

    bool setParent(const SignCell *parent);

    virtual std::ostream& serialize(std::ostream &strm) const;

};

std::ostream& operator<<(std::ostream &strm, const SignCellText &s);

#endif /* SRC_SIGNCELLTEXT_H_ */
