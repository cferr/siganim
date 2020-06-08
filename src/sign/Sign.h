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
#include "../render/Bitmap.h"
#include "cells/Display.h"
#include "Observer.h"
#include "SignColor.h"
#include "SignTree.h"

class Sign : public Observable, public SignTree {
public:
    class DisplayBuilder {
    private:
        Sign* sign;
    public:
        DisplayBuilder(Sign* sign);
        bool build(Display* display);
    };

private:
    std::vector<Display*> displays;

public:
    typedef std::vector<Display*>::iterator SignDisplayVectIt;
    typedef std::vector<Display*>::const_iterator SignDisplayVectConstIt;

    Sign();
    Sign(std::vector<Display*> displays);
    Sign(std::initializer_list<Display*> displays);
    Sign(const Sign* a);

    virtual ~Sign();

    void accept(SignTreeVisitor &visitor);
    void accept(ConstSignTreeVisitor &visitor) const;
    virtual void callbackDispatch(ConstSignTreeDispatcher* s) const;
    virtual void callbackDispatch(SignTreeDispatcher* s);

    bool addDisplay(Display *display);
    bool removeDisplay(Display *display);
    std::vector<Display*> getDisplays() const;
    DisplayBuilder* displayBuilder();

    std::ostream& serialize(std::ostream &strm) const;

    void modified() const;

    void deleteChild(SignTree* child);

    void deepDetachStructureObserver(SignTreeStructureObserver* observer);

};

std::ostream& operator<<(std::ostream &strm, const Sign &s);

#endif /* SRC_SIGN_H_ */
