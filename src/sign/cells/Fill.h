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

#ifndef SRC_SIGN_CELLS_FILL_H_
#define SRC_SIGN_CELLS_FILL_H_

#include <iostream>
#include "../SignCell.h"
#include "../SignColor.h"

class Fill: public SignCell {
    SignColor color;
public:
    Fill(const SignColor& color);
    virtual ~Fill() {

    }

    virtual bool setParent(const SignCell* parent);

    virtual const char* CellTypeStr() const {
        return "Fill";
    }

    const SignColor getColor() const;
    void setColor(SignColor& color);

    virtual unsigned int getHeight() const;
    virtual unsigned int getWidth() const;
    virtual unsigned int getChildHeight(const SignCell* child) const;
    virtual unsigned int getChildWidth(const SignCell* child) const;

    void accept(SignTreeVisitor& visitor);
    void accept(ConstSignTreeVisitor& visitor) const;

    virtual std::ostream& serialize(std::ostream &strm) const;
};

std::ostream& operator<<(std::ostream &strm, const Fill &s);

#endif /* SRC_SIGN_CELLS_FILL_H_ */
