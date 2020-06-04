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


#ifndef SRC_UI_SIGNTREEQTMODEL_H_
#define SRC_UI_SIGNTREEQTMODEL_H_

#include <QTreeWidgetItem>
#include "../sign/SignTree.h"

class SignTreeQtModel: public QTreeWidgetItem, public SignTreeStructureObserver {
private:
    SignTree* underlyingTreeItem;

public:
    SignTreeQtModel(SignTree* underlyingTreeItem);
    virtual ~SignTreeQtModel() {
        QList<QTreeWidgetItem*> children = this->takeChildren();
        for(auto i = children.begin(); i < children.end(); ++i)
            delete *i;
    }

    virtual void dispatchCallback(const Sign& s);
    virtual void dispatchCallback(const Display& s);
    virtual void dispatchCallback(const Text& s);
    virtual void dispatchCallback(const Split& s);
    virtual void dispatchCallback(const MarqueeAnimation& s);
    virtual void dispatchCallback(const BlinkAnimation& s);
    virtual void dispatchCallback(const Compose& s);
    virtual void dispatchCallback(const Fill& s);

    virtual void observeStructure(const SignTree* sender);

    void rebuild();

};

#endif /* SRC_UI_SIGNTREEQTMODEL_H_ */
