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

#ifndef SRC_SIGN_SIGNTREE_H_
#define SRC_SIGN_SIGNTREE_H_

#include <vector>

// All classes part of the tree
class Sign;
class Display;
class Text;
class Split;
class MarqueeAnimation;
class BlinkAnimation;
class Compose;
class Fill;

class SignTreeStructureObserver;

class SignTree {
    std::vector<SignTreeStructureObserver*> structureObservers;

protected:
    void structureChanged() const;

public:
    virtual ~SignTree() {}

    void attachStructureObserver(SignTreeStructureObserver* observer);
    void detachStructureObserver(SignTreeStructureObserver* observer);

    virtual void callbackDispatch(SignTreeStructureObserver* s) const = 0;

};

class SignTreeStructureObserver {

public:
    virtual ~SignTreeStructureObserver() {}
    virtual void observeStructure(const SignTree* sender) = 0;

    virtual void dispatchCallback(const Sign& s) = 0;
    virtual void dispatchCallback(const Display& s) = 0;
    virtual void dispatchCallback(const Text& s) = 0;
    virtual void dispatchCallback(const Split& s) = 0;
    virtual void dispatchCallback(const MarqueeAnimation& s) = 0;
    virtual void dispatchCallback(const BlinkAnimation& s) = 0;
    virtual void dispatchCallback(const Compose& s) = 0;
    virtual void dispatchCallback(const Fill& s) = 0;

};



#endif /* SRC_SIGN_SIGNTREE_H_ */
