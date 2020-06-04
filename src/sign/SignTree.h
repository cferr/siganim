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

class SignTree;

template<bool isConst> class AbstractSignTreeDispatcher {
private:
    typedef typename std::conditional<isConst, const Sign, Sign>::type SignT;
    typedef typename std::conditional<isConst, const Display,
            Display>::type DisplayT;
    typedef typename std::conditional<isConst, const Text,
                Text>::type TextT;
    typedef typename std::conditional<isConst, const Split,
                Split>::type SplitT;
    typedef typename std::conditional<isConst, const MarqueeAnimation,
                MarqueeAnimation>::type MarqueeAnimationT;
    typedef typename std::conditional<isConst, const BlinkAnimation,
                BlinkAnimation>::type BlinkAnimationT;
    typedef typename std::conditional<isConst, const Compose,
                Compose>::type ComposeT;
    typedef typename std::conditional<isConst, const Fill,
                Fill>::type FillT;
public:
    virtual ~AbstractSignTreeDispatcher() {}

    virtual void dispatchCallback(SignT& s) = 0;
    virtual void dispatchCallback(DisplayT& s) = 0;
    virtual void dispatchCallback(TextT& s) = 0;
    virtual void dispatchCallback(SplitT& s) = 0;
    virtual void dispatchCallback(MarqueeAnimationT& s) = 0;
    virtual void dispatchCallback(BlinkAnimationT& s) = 0;
    virtual void dispatchCallback(ComposeT& s) = 0;
    virtual void dispatchCallback(FillT& s) = 0;
};

typedef AbstractSignTreeDispatcher<true> ConstSignTreeDispatcher;
typedef AbstractSignTreeDispatcher<false> SignTreeDispatcher;

class SignTreeStructureObserver {

public:
    virtual ~SignTreeStructureObserver() {}
    virtual void observeStructure(const SignTree* sender) = 0;

};

class SignTree {
    std::vector<SignTreeStructureObserver*> structureObservers;

protected:
    void structureChanged() const;

public:
    virtual ~SignTree() {}

    void attachStructureObserver(SignTreeStructureObserver* observer);
    void detachStructureObserver(SignTreeStructureObserver* observer);

    virtual void callbackDispatch(ConstSignTreeDispatcher* s) const = 0;
    virtual void callbackDispatch(SignTreeDispatcher* s) = 0;

};



#endif /* SRC_SIGN_SIGNTREE_H_ */
