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

#include <QAction>
#include "SiganimMainWindow.h"

SiganimMainWindow::SiganimMainWindow() :
    SiganimMainWindow(nullptr, nullptr) {

}

SiganimMainWindow::SiganimMainWindow(Sign* sign, Text* textCell) :
    textCell(textCell) {
    this->setWindowTitle(QString("SigAnim Sign Animator"));

    QWidget *ui_area = new QWidget;
    this->setCentralWidget(ui_area);

    QMenuBar* menuBar = this->menuBar();
    // Generate menus
    menuBar->addAction(new QAction(QString("File"), menuBar));

    this->verticalLayout = new QVBoxLayout();

    this->signWidget = new SignWidget(sign);

    if(textCell != nullptr) {
        this->text = new QLineEdit();
        this->verticalLayout->addWidget(this->text);
        this->text->connect(this->text,
                &QLineEdit::textChanged,
                this,
                &SiganimMainWindow::updateSignText);
    }

    this->verticalLayout->addWidget(this->signWidget);

    // Set image widget as central widget
    ui_area->setLayout(this->verticalLayout);
}

SiganimMainWindow::~SiganimMainWindow() {

}

void SiganimMainWindow::updateSignText(const QString& text) {
    this->textCell->setText(icu::UnicodeString(text.toUtf8().data()));
    this->signWidget->signChangedEvent();
}

