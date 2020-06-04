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

    this->verticalLayout = new QVBoxLayout();

    this->text = new QLineEdit(this);
    this->tree = new QTreeWidget(this);
    this->details = new SignTreeDetailsWidget();
    this->signWidget = new SignWidget(sign);

    this->verticalLayout->addWidget(this->text);
    this->verticalLayout->addWidget(this->tree);
    this->verticalLayout->addWidget(this->details);
    this->verticalLayout->addWidget(this->signWidget);

    // Set image widget as central widget
    this->setLayout(this->verticalLayout);

    // Build tree
    this->tree->addTopLevelItem(new SignTreeQtModel(sign, this->details));
    this->tree->expandAll();

    this->tree->connect(this->tree,
            &QTreeWidget::currentItemChanged, this,
            &SignEditor::updateDetails);

}

void SignEditor::updateDetails(QTreeWidgetItem *current,
        QTreeWidgetItem *previous) {
    // Cast as SignTreeQtModel
    SignTreeQtModel* model = (SignTreeQtModel*)current;

    this->details->update(model->getTreeItem());
}
