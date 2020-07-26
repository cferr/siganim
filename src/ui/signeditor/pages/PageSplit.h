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


#ifndef UI_PAGES_PAGESPLIT_H_
#define UI_PAGES_PAGESPLIT_H_

#include <QSpinBox>
#include <QRadioButton>
#include <QWidget>
#include "../../../sign/cells/Split.h"

class PageSplit : public QWidget {
    Q_OBJECT

private:
    Split* treeNode;
    QSpinBox* posSpinner;
    QRadioButton* radioDirHorizontal;
    QRadioButton* radioDirVertical;
    QRadioButton* radioDirDiagonalSWNE;
    QRadioButton* radioDirDiagonalNWSE;

public:
    PageSplit(Split* treeNode);
    virtual ~PageSplit() { }

public slots:
    void setDirHorizontal(bool set);
    void setDirVertical(bool set);
    void setDirDiagonalSWNE(bool set);
    void setDirDiagonalNWSE(bool set);
    void setSplitPos(int pos);
};

#endif /* UI_PAGES_PAGESPLIT_H_ */
