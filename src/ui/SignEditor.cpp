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

SignEditor::SignEditor(Sign* sign, const FontSet* fontSet,
        const RasterizerSet* rasterizerSet, QWidget* parent) :
    QWidget(parent), sign(sign), fontSet(fontSet),
    rasterizerSet(rasterizerSet) {

    this->layout = new QGridLayout(this);

    this->rasterizerCombo = new QComboBox(this);

    // Populate rasterizers
    std::vector<Rasterizer*> rasterizers =
            this->rasterizerSet->getRasterizers();
    for(auto i = rasterizers.begin(); i < rasterizers.end(); ++i) {
        this->rasterizerCombo->addItem(QString::fromStdString((*i)->getName()),
                QString::fromStdString((*i)->getName()));
    }

    // TODO add default rasterizer in SiganimDefaults...
    if(rasterizers.empty()) {
        this->currentRasterizer = new Rasterizer("Default", 5);
    } else {
        this->currentRasterizer = *(rasterizers.begin());
    }

    this->details = new SignTreeDetailsWidget(this->fontSet);
    this->tree = new SignTreeWidget(this, this->details);

    this->signWidget = new SignWidget(sign, fontSet, this->currentRasterizer);

    this->layout->addWidget(this->rasterizerCombo, 0, 1, Qt::AlignLeft);

    this->layout->addWidget(this->tree, 1, 0, Qt::AlignLeft);
    this->layout->addWidget(this->details, 1, 1, Qt::AlignLeft);

    this->layout->addWidget(this->signWidget, 2, 0, 1, 2, Qt::AlignCenter);


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

void SignEditor::setRasterizer(const QString &rasterizer) {
    try {
        const Rasterizer* r = this->rasterizerSet->getConst(
                rasterizer.toStdString());
        this->signWidget->setRasterizer(r);
        this->currentRasterizer = r;
    } catch(RasterizerSet::RasterizerNotFoundException& e) {

    }
}

