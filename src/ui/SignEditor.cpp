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

SignEditor::SignEditor(Sign* sign) {

    this->layout = new QGridLayout(this);

    this->tree = new QTreeWidget(this);
    this->details = new SignTreeDetailsWidget();
    this->signWidget = new SignWidget(sign);

    this->layout->addWidget(this->tree, 0, 0, Qt::AlignLeft);
    this->layout->addWidget(this->details, 0, 1, Qt::AlignLeft);

    this->layout->addWidget(this->signWidget, 1, 0, 1, 2, Qt::AlignCenter);


    // Set image widget as central widget
    this->setLayout(this->layout);

    // Build tree
    this->tree->addTopLevelItem(new SignTreeQtModel(sign, this->details));
    this->tree->expandAll();
    this->tree->setHeaderHidden(true);

    this->tree->connect(this->tree,
            &QTreeWidget::currentItemChanged, this,
            &SignEditor::updateDetails);

}

void SignEditor::updateDetails(QTreeWidgetItem *current,
        QTreeWidgetItem *previous) {
    // Cast as SignTreeQtModel
    SignTreeQtModel* model = (SignTreeQtModel*)current;

    try {
        this->details->update(model->getTreeItem());
    } catch(SignTreeQtModel::NoTreeItemException& e) {
        this->details->updateEmpty();
    }
}
