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

#include "SignEditor.h"
#include "SignTreeQtModel.h"

SignEditor::SignEditor(Sign* sign) : sign(sign) {

    this->layout = new QGridLayout(this);

    this->details = new SignTreeDetailsWidget();
    this->tree = new SignTreeWidget(this, this->details);

    this->signWidget = new SignWidget(sign);

    this->layout->addWidget(this->tree, 0, 0, Qt::AlignLeft);
    this->layout->addWidget(this->details, 0, 1, Qt::AlignLeft);

    this->layout->addWidget(this->signWidget, 1, 0, 1, 2, Qt::AlignCenter);


    // Set image widget as central widget
    this->setLayout(this->layout);

    // Build tree
    SignTreeQtModel* model = new SignTreeQtModel(sign);
    this->tree->setModel(model);

    this->tree->expandAll();
    this->tree->setHeaderHidden(true);
    connect(model, &SignTreeQtModel::rowsInserted,
            this->tree, &SignTreeWidget::expandTreeRows);
}

