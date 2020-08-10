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

#ifndef SRC_SIGN_CELLS_MARQUEEANIMATION_H_
#define SRC_SIGN_CELLS_MARQUEEANIMATION_H_

#include "Animation.h"

class MarqueeAnimation: public Animation {
public:
    enum Direction { LEFT, RIGHT };

private:
    enum Direction direction;
    unsigned int space;

public:
    MarqueeAnimation();
    MarqueeAnimation(SignCell* subject, unsigned int durationFrames,
            const unsigned int initialPhaseFrames,
            enum Direction direction, unsigned int space);
    MarqueeAnimation(const MarqueeAnimation* a);
    SignCell* copy();

    virtual ~MarqueeAnimation() {
    }

    virtual const char* CellTypeStr() const {
        return "Marquee Animation";
    }

    void setDurationFrames(unsigned int durationFrames);

    virtual void accept(SignTreeVisitor &visitor);
    virtual void accept(ConstSignTreeVisitor &visitor) const;
    virtual void callbackDispatch(ConstSignTreeDispatcher* s) const;
    virtual void callbackDispatch(SignTreeDispatcher* s);

    void setDirection(enum Direction direction);
    void setSpace(unsigned int space);

    enum Direction getDirection() const;
    unsigned int getSpace() const;

    virtual std::ostream& serialize(std::ostream &strm) const;
    json_object* toJSON() const;

};

std::ostream& operator<<(std::ostream &strm, const MarqueeAnimation &s);

#endif /* SRC_SIGN_CELLS_MARQUEEANIMATION_H_ */
