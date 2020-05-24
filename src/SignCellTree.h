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

#ifndef SRC_SIGNCELLTREE_H_
#define SRC_SIGNCELLTREE_H_

#include <cstdint>
#include <vector>
#include <iostream>
#include "SignImage.h"
#include "SignTree.h"

class SignCellTree: public SignTree {
protected:

    unsigned int height;
    unsigned int width;

    virtual bool checkResize() const = 0;

public:
    SignCellTree(unsigned int height, unsigned int width);
    virtual ~SignCellTree();

    unsigned int getHeight() const;
    unsigned int getWidth() const;
    bool setHeight(const unsigned int height);
    bool setWidth(const unsigned int width);
    bool resize(const unsigned int height, const unsigned int width);

    bool setParent(const SignTree *parent);

    virtual std::ostream& serialize(std::ostream &strm) const = 0;

};

std::ostream& operator<<(std::ostream &strm, const SignCellTree &s);

#endif /* SRC_SIGNCELLTREE_H_ */
