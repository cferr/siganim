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

#ifndef SRC_SIGNTREEVISITOR_H_
#define SRC_SIGNTREEVISITOR_H_

#include <type_traits>

class Sign;
class Display;
class Text;
class Split;
class MarqueeAnimation;
class BlinkAnimation;
class Compose;
class Fill;

template<bool isConst>
    class AbstractSignTreeVisitor {
private:
    typedef typename std::conditional<isConst, const Sign, Sign>::type SignT;
    typedef typename std::conditional<isConst, const Display,
            Display>::type SignDisplayT;
    typedef typename std::conditional<isConst, const Text,
                Text>::type SignCellTextT;
    typedef typename std::conditional<isConst, const Split,
                Split>::type SignCellSplitT;
    typedef typename std::conditional<isConst, const MarqueeAnimation,
                MarqueeAnimation>::type MarqueeAnimationT;
    typedef typename std::conditional<isConst, const BlinkAnimation,
                BlinkAnimation>::type BlinkAnimationT;
    typedef typename std::conditional<isConst, const Compose,
                Compose>::type ComposeT;
    typedef typename std::conditional<isConst, const Fill,
                Fill>::type FillT;

public:
    virtual ~AbstractSignTreeVisitor() {

    };

    virtual void visit(SignT &s) = 0;
    virtual void visit(SignDisplayT &s) = 0;
    virtual void visit(SignCellTextT &s) = 0;
    virtual void visit(SignCellSplitT &s) = 0;
    virtual void visit(MarqueeAnimationT &s) = 0;
    virtual void visit(BlinkAnimationT &s) = 0;
    virtual void visit(ComposeT &s) = 0;
    virtual void visit(FillT &s) = 0;
};

typedef AbstractSignTreeVisitor<false> SignTreeVisitor;
typedef AbstractSignTreeVisitor<true> ConstSignTreeVisitor;

#endif /* SRC_SIGNTREEVISITOR_H_ */
