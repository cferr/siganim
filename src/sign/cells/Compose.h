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

#ifndef SRC_SIGN_CELLS_COMPOSE_H_
#define SRC_SIGN_CELLS_COMPOSE_H_

#include <iostream>
#include "../SignCell.h"
#include "../SignTreeVisitor.h"

class Compose : public SignCell {
public:
    class ForegroundBuilder : public Builder {
    private:
        Compose* compose;
    public:
        ForegroundBuilder(Compose* compose);
        bool build(SignCell* child);
    };
    class BackgroundBuilder : public Builder {
    private:
        Compose* compose;
    public:
        BackgroundBuilder(Compose* compose);
        bool build(SignCell* child);
    };
private:
    SignCell* background;
    SignCell* foreground;

public:
    Compose();
    Compose(SignCell* background, SignCell* foreground);
    Compose(const Compose* a);
    SignCell* copy();
    virtual ~Compose();

    virtual const char* CellTypeStr() const {
        return "Compose";
    }

    SignCell* getForeground() const;
    SignCell* getBackground() const;
    bool setForeground(SignCell* foreground);
    bool setBackground(SignCell* background);
    BackgroundBuilder* backgroundBuilder();
    ForegroundBuilder* foregroundBuilder();

    virtual unsigned int getHeight() const;
    virtual unsigned int getWidth() const;
    virtual unsigned int getChildHeight(const SignCell* child) const;
    virtual unsigned int getChildWidth(const SignCell* child) const;

    void accept(SignTreeVisitor& visitor);
    void accept(ConstSignTreeVisitor& visitor) const;
    virtual void callbackDispatch(ConstSignTreeDispatcher* s) const;
    virtual void callbackDispatch(SignTreeDispatcher* s);

    void deleteChild(SignTree* child);

    virtual std::ostream& serialize(std::ostream &strm) const;
    json_object* toJSON() const;
    void deepDetachStructureObserver(SignTreeStructureObserver* observer);
};

std::ostream& operator<<(std::ostream &strm, const Compose &s);

#endif /* SRC_SIGN_CELLS_COMPOSE_H_ */
