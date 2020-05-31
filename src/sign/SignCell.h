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

#ifndef SRC_SIGNCELL_H_
#define SRC_SIGNCELL_H_

#include <iostream>
#include "SignTreeVisitor.h"

class SignCell {
protected:
    const SignCell* parent;

public:
    enum Type {
        DISPLAY, CELL_SPLIT, TEXT
    };

    typedef enum Type Type;
    virtual Type getType() const = 0;

    virtual ~SignCell();

    virtual unsigned int getHeight() const = 0;
    virtual unsigned int getWidth() const = 0;
    virtual unsigned int getChildHeight(const SignCell* child) const = 0;
    virtual unsigned int getChildWidth(const SignCell* child) const = 0;

    virtual bool setParent(const SignCell* parent) = 0;
    const SignCell* getParent() const;

    virtual void accept(SignTreeVisitor& visitor) = 0;

    virtual std::ostream& serialize(std::ostream &strm) const = 0;

};

std::ostream& operator<<(std::ostream &strm, const SignCell &s);

#endif /* SRC_SIGNCELL_H_ */
