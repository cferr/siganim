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

#ifndef SRC_SIGN_H_
#define SRC_SIGN_H_

#include <vector>
#include "SignDisplay.h"
#include "SignPixel.h"
#include "SignTree.h"
#include "Bitmap.h"

class Sign: public SignTree {
private:
    std::vector<SignDisplay*> displays;

    unsigned int height;
    unsigned int width;

public:
    typedef std::vector<SignDisplay*>::iterator SignDisplayVectIt;
    typedef std::vector<SignDisplay*>::const_iterator SignDisplayVectConstIt;

    Sign(unsigned int height, unsigned int width);
    Sign(unsigned int height, unsigned int width,
            std::vector<SignDisplay*> displays);
    Sign(unsigned int height, unsigned int width,
            std::initializer_list<SignDisplay*> displays);
    virtual ~Sign();

    unsigned int getHeight();
    unsigned int getWidth();

    SignTreeType getType() const;
    bool setParent(const SignTree *parent);
    void accept(SignTreeVisitor &visitor);

    bool addDisplay(SignDisplay *display);
    bool removeDisplay(SignDisplay *display);
    std::vector<SignDisplay*> getDisplays();

    std::ostream& serialize(std::ostream &strm) const;
};

std::ostream& operator<<(std::ostream &strm, const Sign &s);

#endif /* SRC_SIGN_H_ */
