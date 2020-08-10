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

#ifndef SRC_SIGN_CELLS_BLINKANIMATION_H_
#define SRC_SIGN_CELLS_BLINKANIMATION_H_

#include "Animation.h"

class BlinkAnimation: public Animation {
private:
    unsigned int framesOn;
    unsigned int framesOff;

public:
    BlinkAnimation();
    BlinkAnimation(SignCell* subject, const unsigned int framesOn,
            const unsigned int framesOff,
            const unsigned int initialPhaseFrames);
    BlinkAnimation(const BlinkAnimation* a);
    SignCell* copy();

    virtual ~BlinkAnimation() {

    }

    virtual const char* CellTypeStr() const {
        return "Blink Animation";
    }

    unsigned int getFramesOn() const;
    unsigned int getFramesOff() const;
    void setFramesOn(unsigned int framesOn);
    void setFramesOff(unsigned int framesOff);

    virtual void accept(SignTreeVisitor &visitor);
    virtual void accept(ConstSignTreeVisitor &visitor) const;
    virtual void callbackDispatch(ConstSignTreeDispatcher* s) const;
    virtual void callbackDispatch(SignTreeDispatcher* s);

    virtual std::ostream& serialize(std::ostream &strm) const;
    json_object* toJSON() const;

};

std::ostream& operator<<(std::ostream &strm, const BlinkAnimation &s);

#endif /* SRC_SIGN_CELLS_BLINKANIMATION_H_ */
