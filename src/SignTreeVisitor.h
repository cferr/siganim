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

class Sign;
class SignDisplay;
class SignCellTree;
class SignCellLeaf;
class SignCellNode;

class SignTreeVisitor {
public:
    virtual ~SignTreeVisitor();

    virtual void visit(Sign &s) = 0;
    virtual void visit(SignDisplay &s) = 0;
    virtual void visit(SignCellLeaf &s) = 0;
    virtual void visit(SignCellNode &s) = 0;
};

#endif /* SRC_SIGNTREEVISITOR_H_ */
