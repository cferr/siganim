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

#include "SignCellNode.h"
#include <iostream>

SignCellNode::SignCellNode(unsigned int width, unsigned int height) :
        SignCellTree(width, height) {

}

SignCellNode::SignCellNode(unsigned int width, unsigned int height,
    const std::vector<std::tuple<SignCellTree*, unsigned int, unsigned int>>
            children) : SignCellNode(width, height) {
    for (auto child : children)
        this->addCell(std::get<0>(child), std::get<1>(child),
                std::get<2>(child));
}

SignCellNode::SignCellNode(unsigned int width, unsigned int height,
    std::initializer_list<std::tuple<SignCellTree*, unsigned int, unsigned int>>
        children) : SignCellNode(width, height) {

    for (auto child : children)
        this->addCell(std::get<0>(child), std::get<1>(child),
                std::get<2>(child));
}

SignCellNode::~SignCellNode() {

}

SignTreeType SignCellNode::getType() const {
    return SignTreeType::SIGN_CELL_NODE;
}

void SignCellNode::accept(SignTreeVisitor &visitor) {
    visitor.visit(*this);
}

bool SignCellNode::checkResize() const {
    bool ret = true;
    CellPtrVectorConstIt childIt = this->children.begin();
    CellCoordVectorConstIt childCoordIt = this->childrenCoords.begin();
    for (; (childIt < this->children.end()) &&
        (childCoordIt < this->childrenCoords.end()); childIt++, childCoordIt++)
    {
        if (((*childIt)->getHeight() + childCoordIt->y > this->getHeight())
                || ((*childIt)->getWidth() + childCoordIt->x
                        > this->getWidth()))
            ret &= false;
    }

    return ret;
}

bool SignCellNode::addCell(SignCellTree *child, unsigned int x,
        unsigned int y) {
    if (y + child->getHeight() <= this->getHeight()) {
        if (x + child->getWidth() <= this->getWidth()) {

            SignCellCoords coords;
            coords.x = x;
            coords.y = y;

            child->setParent(this);

            this->children.push_back(child);
            this->childrenCoords.push_back(coords);
            return true;
        }
    }
    return false;
}

bool SignCellNode::removeCell(const SignCellTree *child) {
    bool ret = false;

    CellPtrVectorConstIt childIt = this->children.begin();
    CellCoordVectorConstIt childCoordIt = this->childrenCoords.begin();
    for (;
            (childIt < this->children.end())
                    && (childCoordIt < this->childrenCoords.end());
            childIt++, childCoordIt++) {
        if (*childIt == child) {
            this->children.erase(childIt);
            this->childrenCoords.erase(childCoordIt);
            ret = true;
        }
    }

    return ret;
}

SignCellNode::CellPtrVector SignCellNode::getChildren() {
    return this->children;
}

SignCellNode::CellCoordVector SignCellNode::getChildrenCoords() {
    return this->childrenCoords;
}

std::ostream& SignCellNode::serialize(std::ostream &strm) const {
    strm << "{ " << this->width << "x" << this->height << " : { ";

    SignCellNode::CellPtrVectorConstIt childIt = this->children.begin();
    SignCellNode::CellCoordVectorConstIt childCoordIt =
            this->childrenCoords.begin();

    while ((childIt < this->children.end())
            && (childCoordIt < this->childrenCoords.end())) {
        strm << "+" << (*childCoordIt).x << "+" << (*childCoordIt).y << " : "
                << (*(*childIt));
        childIt++;
        childCoordIt++;
        if ((childIt < this->children.end())
                && (childCoordIt < this->childrenCoords.end())) {
            strm << ", ";
        }
    }
    strm << " } }";

    return strm;
}

std::ostream& operator<<(std::ostream &strm, const SignCellNode &s) {
    return s.serialize(strm);
}
