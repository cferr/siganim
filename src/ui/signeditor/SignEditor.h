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

#include <QComboBox>
#include <QGridLayout>
#include <QMenuBar>
#include <QPushButton>
#include <QTreeWidget>
#include <QWidget>

#include "../../font/FontSet.h"
#include "../../render/RasterizerSet.h"
#include "../../sign/cells/Text.h"
#include "SignTreeQtModel.h"
#include "SignTreeDetailsWidget.h"
#include "SignTreeWidget.h"
#include "SignWidget.h"
#include "../SiganimUICore.h"

class SignEditor: public QWidget {
    Q_OBJECT

private:
    QGridLayout* layout;
    SignWidget *signWidget;
    QComboBox* rasterizerCombo;
    QPushButton* saveToGIFButton;
    SignTreeWidget *tree;
    SignTreeDetailsWidget* details;
    SignTreeQtModel* model;
    Sign* sign;
    const FontSet* fontSet;
    const RasterizerSet* rasterizerSet;
    const Rasterizer* currentRasterizer;

    void populateRasterizers();

public:
    SignEditor(SiganimUICore* uiCore, QWidget* parent);
    virtual ~SignEditor();

    void setRasterizer(const QString& rasterizer);

    void getRasterizer();

    const Rasterizer* getCurrentRasterizer() const;

};

#endif /* SRC_UI_SIGNEDITOR_H_ */
