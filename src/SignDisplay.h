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

#ifndef SRC_SIGNDISPLAY_H_
#define SRC_SIGNDISPLAY_H_

#include <vector>
#include "Bitmap.h"
#include "SignCellNode.h"
#include "SignPixel.h"
#include "SignTree.h"

class SignDisplay: public SignTree {
private:
    // TODO add some geometry info (margins, display size in cm/in, dpi...)
    unsigned int height;
    unsigned int width;

    enum SignPixelType displayType;  // LEDs (monochromatic, RGB) or flip discs.

    SignCellTree *rootCell;

public:
    SignDisplay(unsigned int height, unsigned int width,
            enum SignPixelType type);
    SignDisplay(unsigned int height, unsigned int width,
            enum SignPixelType type, SignCellTree *rootCell);
    virtual ~SignDisplay();

    SignTreeType getType() const;
    bool setParent(const SignTree *parent);
    void accept(SignTreeVisitor &visitor);

    bool setRootCell(SignCellTree *rootCell);
    SignCellTree* getRootCell();

    enum SignPixelType getDisplayType();
    bool setDisplayType(enum SignPixelType displayType);

    unsigned int getHeight();
    unsigned int getWidth();
    bool setHeight(const unsigned int height);
    bool setWidth(const unsigned int height);
    bool resize(const unsigned int height, const unsigned int width);

    std::ostream& serialize(std::ostream &strm) const;
};

std::ostream& operator<<(std::ostream &strm, const SignDisplay &s);

#endif /* SRC_SIGNDISPLAY_H_ */
