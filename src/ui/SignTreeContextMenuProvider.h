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

#ifndef UI_SIGNTREECONTEXTMENUPROVIDER_H_
#define UI_SIGNTREECONTEXTMENUPROVIDER_H_

#include <QMenu>
#include <QObject>
#include "../sign/Sign.h"
#include "../sign/SignCell.h"

class SignTreeContextMenuProvider : public QObject {
    Q_OBJECT

private:
    SignTree* currentTree;
    SignTree* parentTree;
    SignCell::Builder* cellBuilder;
    Sign::DisplayBuilder* displayBuilder;
    QMenu* contextMenu;

public:
    SignTreeContextMenuProvider(SignTree* current, SignTree* parent);
    SignTreeContextMenuProvider(Sign::DisplayBuilder* builder);
    SignTreeContextMenuProvider(SignCell::Builder* builder);
    virtual ~SignTreeContextMenuProvider() {}

    void buildSign(bool checked);
    void buildDisplay(bool checked);
    void buildText(bool checked);
    void buildSplit(bool checked);
    void buildMarqueeAnimation(bool checked);
    void buildBlinkAnimation(bool checked);
    void buildCompose(bool checked);
    void buildFill(bool checked);
    void deleteNode(bool checked);

    QMenu* getContextMenu();
};

#endif /* UI_SIGNTREECONTEXTMENUPROVIDER_H_ */
