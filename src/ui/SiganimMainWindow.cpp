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

SiganimMainWindow::SiganimMainWindow(Sign* sign, FontSet* fontSet)
{
    this->studio = new FontStudio();
    this->editor = new SignEditor(sign, fontSet);
    this->tabs = new QTabWidget();
    tabs->addTab(this->editor, "Sign Editor");
    tabs->addTab(this->studio, "Font Studio");

    this->setWindowTitle(QString("SigAnim Sign Animator"));

//    QWidget *ui_area = new QWidget;
    this->setCentralWidget(this->tabs);

    QMenuBar* menuBar = this->menuBar();
    // Generate menus
    menuBar->addAction(new QAction(QString("File"), menuBar));

}

SiganimMainWindow::~SiganimMainWindow() {

}



