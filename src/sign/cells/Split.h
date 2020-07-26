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

#ifndef SRC_SIGN_CELLS_SPLIT_H_
#define SRC_SIGN_CELLS_SPLIT_H_

#include <vector>
#include <tuple>
#include <iostream>

#include "../SignCell.h"
#include "../SignTreeVisitor.h"

class Split: public SignCell {
public:
    class TopOrLeftChildBuilder : public Builder {
    private:
        Split* split;
    public:
        TopOrLeftChildBuilder(Split* split);
        bool build(SignCell* child);
    };

    class BottomOrRightChildBuilder : public Builder {
    private:
        Split* split;
    public:
        BottomOrRightChildBuilder(Split* split);
        bool build(SignCell* child);
    };

    enum SplitDirection {
        SPLIT_HORIZONTAL,
        SPLIT_VERTICAL,
        SPLIT_SW_NE_DIAGONAL,
        SPLIT_NW_SE_DIAGONAL
    };

private:
    enum SplitDirection splitDirection;
    unsigned int splitPos;
    SignCell* topOrLeftChild;
    SignCell* bottomOrRightChild;

public:
    Split();
    Split(enum SplitDirection splitDirection, unsigned int splitPos,
            SignCell* topOrLeftChild, SignCell* bottomOrRightChild);
    Split(const Split* a);
    SignCell* copy();
    virtual ~Split();

    virtual const char* CellTypeStr() const {
        return "Cell Split";
    }

    virtual void accept(SignTreeVisitor &visitor);
    virtual void accept(ConstSignTreeVisitor &visitor) const;
    virtual void callbackDispatch(ConstSignTreeDispatcher* s) const;
    virtual void callbackDispatch(SignTreeDispatcher* s);

    virtual unsigned int getHeight() const;
    virtual unsigned int getWidth() const;
    virtual unsigned int getChildHeight(const SignCell* child) const override;
    virtual unsigned int getChildWidth(const SignCell* child) const override;

    enum SplitDirection getSplitDirection() const;
    bool setSplitDirection(enum SplitDirection direction);

    bool setSplitPos(unsigned int splitPos);
    unsigned int getSplitPos() const;

    bool setTopOrLeftChild(SignCell* child);
    SignCell* getTopOrLeftChild() const;
    bool setBottomOrRightChild(SignCell* child);
    SignCell* getBottomOrRightChild() const;
    TopOrLeftChildBuilder* topOrLeftChildBuilder();
    BottomOrRightChildBuilder* bottomOrRightChildBuilder();

    void deleteChild(SignTree* child);

    virtual std::ostream& serialize(std::ostream &strm) const;
    json_object* toJSON() const;

    void deepDetachStructureObserver(SignTreeStructureObserver* observer);
};

std::ostream& operator<<(std::ostream &strm, const Split &s);

#endif /* SRC_SIGN_CELLS_SPLIT_H_ */
