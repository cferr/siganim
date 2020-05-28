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

#ifndef SRC_SIGNCELLSPLIT_H_
#define SRC_SIGNCELLSPLIT_H_

#include <vector>
#include <tuple>
#include <iostream>

#include "SignCell.h"
#include "SignImage.h"
#include "SignTreeVisitor.h"

class SignCellSplit: public SignCell {
public:
    enum SplitDirection {
        SPLIT_HORIZONTAL,
        SPLIT_VERTICAL
    };

private:
    enum SplitDirection splitDirection;
    unsigned int splitPos;
    SignCell* topOrLeftChild;
    SignCell* bottomOrRightChild;

public:

    SignCellSplit(enum SplitDirection splitDirection, unsigned int splitPos,
            SignCell* topOrLeftChild, SignCell* bottomOrRightChild);
    virtual ~SignCellSplit();

    virtual Type getType() const;
    virtual void accept(SignTreeVisitor &visitor);

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

    virtual bool setParent(const SignCell *parent);

    virtual std::ostream& serialize(std::ostream &strm) const;
};

std::ostream& operator<<(std::ostream &strm, const SignCellSplit &s);

#endif /* SRC_SIGNCELLSPLIT_H_ */
