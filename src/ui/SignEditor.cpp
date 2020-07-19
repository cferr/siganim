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

#include <QtGlobal>
#include <QFileDialog>
#include <QGroupBox>
#include "SignEditor.h"
#include "../render/GIFSink.h"
#include "../SiganimCore.h"

SignEditor::SignEditor(SiganimUICore* uiCore, QWidget* parent) :
    QWidget(parent), sign(uiCore->getCore()->getSign()),
    fontSet(uiCore->getCore()->getFontSet()),
    rasterizerSet(uiCore->getCore()->getRasterizerSet()) {

    assert(!this->rasterizerSet->getRasterizers().empty());

    this->layout = new QGridLayout(this);

    this->rasterizerCombo = new QComboBox(this);

    this->details = new SignTreeDetailsWidget(this->fontSet);
    this->tree = new SignTreeWidget(this, this->details);

    this->currentRasterizer = *(this->rasterizerSet->getRasterizers().begin());
    this->signWidget = new SignWidget(sign, fontSet, this->currentRasterizer);
    this->saveToGIFButton = new QPushButton("Save to GIF", this);

    QGroupBox* signPreviewBox = new QGroupBox("Sign preview", this);
    QVBoxLayout* previewBoxLayout = new QVBoxLayout();
    previewBoxLayout->addWidget(this->signWidget);
    previewBoxLayout->addWidget(this->saveToGIFButton);
    signPreviewBox->setLayout(previewBoxLayout);

    this->layout->addWidget(this->rasterizerCombo, 0, 1, Qt::AlignLeft);

    this->layout->addWidget(this->tree, 1, 0, Qt::AlignLeft);
    this->layout->addWidget(this->details, 1, 1, Qt::AlignLeft);

    this->layout->addWidget(signPreviewBox, 2, 0, 1, 2, Qt::AlignCenter);
//    this->layout->addWidget(this->signWidget, 2, 0, 1, 2, Qt::AlignCenter);
//    this->layout->addWidget(this->saveToGIFButton, 3, 0, 1, 2, Qt::AlignCenter);

    // Set image widget as central widget
    this->setLayout(this->layout);

    // Build tree
    this->model = new SignTreeQtModel(sign);
    this->tree->setModel(this->model);

    this->tree->expandAll();
    this->tree->setHeaderHidden(true);
    connect(model, &SignTreeQtModel::rowsInserted,
            this->tree, &SignTreeWidget::expandTreeRows);
    connect(rasterizerCombo,
            &QComboBox::currentTextChanged,
            this, &SignEditor::setRasterizer);
    connect(saveToGIFButton,
            &QPushButton::pressed,
            uiCore,
            &SiganimUICore::saveSignToGIF);


    this->populateRasterizers();

}

SignEditor::~SignEditor() {
    delete this->layout;
    delete this->signWidget;
    delete this->rasterizerCombo;
    delete this->tree;
    delete this->details;
    delete this->model;
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

void SignEditor::populateRasterizers() {
    this->rasterizerCombo->clear();
    // Populate rasterizers
    std::vector<Rasterizer*> rasterizers =
            this->rasterizerSet->getRasterizers();
    for(auto i = rasterizers.begin(); i < rasterizers.end(); ++i) {
        this->rasterizerCombo->addItem(QString::fromStdString((*i)->getName()),
                QString::fromStdString((*i)->getName()));
    }
}

//void SignEditor::saveToGIF() {
//    // Show "Save as" dialog
//    QFileDialog saveDialog(this, "Save GIF to...", ".", "*.gif");
//    saveDialog.setAcceptMode(QFileDialog::AcceptMode::AcceptSave);
//
//    if(saveDialog.exec() == QFileDialog::Accepted) {
//        QStringList selectedFiles = saveDialog.selectedFiles();
//        // There should be only one - take it
//        QString fileName = selectedFiles.first();
//        GIFSink s(sign, fontSet, currentRasterizer, 1);
//        std::string fileNameAsString = fileName.toStdString();
//        s.render(fileNameAsString.data());
//    }
//}

void SignEditor::getRasterizer() {
}

const Rasterizer* SignEditor::getCurrentRasterizer() const {
    return this->currentRasterizer;
}
