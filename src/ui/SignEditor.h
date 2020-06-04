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

#ifndef SRC_UI_SIGNEDITOR_H_
#define SRC_UI_SIGNEDITOR_H_

#include <QLineEdit>
#include <QMenuBar>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "../sign/cells/Text.h"
#include "SignWidget.h"

class SignEditor: public QWidget {
    Q_OBJECT

private:
    QVBoxLayout *verticalLayout;
    QLineEdit *text;
    SignWidget *signWidget;
    QTreeWidget *tree;

public:
    SignEditor(Sign* sign);
    virtual ~SignEditor() {
    }

};

#endif /* SRC_UI_SIGNEDITOR_H_ */
