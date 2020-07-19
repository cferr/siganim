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
#include <QMenu>
#include "SiganimMainWindow.h"

SiganimMainWindow::SiganimMainWindow(SiganimUICore* uiCore) : uiCore(uiCore)
{
    this->resize(800, 600);

    this->studio = new FontStudio(uiCore, this);
    this->editor = new SignEditor(uiCore, this);
    this->tabs = new QTabWidget();
    tabs->addTab(this->editor, "Sign Editor");
    tabs->addTab(this->studio, "Font Studio");

    this->setWindowTitle(QString("SigAnim Sign Animator"));

    this->setCentralWidget(this->tabs);

    QMenuBar* menuBar = this->menuBar();

    QMenu* fileMenu = new QMenu(QString("File"), menuBar);
    QAction* fileNewAction = new QAction(QString("New Sign"), fileMenu);
    QAction* fileOpenAction = new QAction(QString("Open Sign..."), fileMenu);
    QAction* fileSaveAction = new QAction(QString("Save Sign"), fileMenu);
    QAction* fileSaveAsAction =
            new QAction(QString("Save Sign As..."), fileMenu);
    QAction* fileExportToGIFAction =
            new QAction(QString("Export Sign to GIF"), fileMenu);
    QAction* fileExitAction = new QAction(QString("Exit"), fileMenu);
    fileMenu->addAction(fileNewAction);
    fileMenu->addAction(fileOpenAction);
    fileMenu->addAction(fileSaveAction);
    fileMenu->addAction(fileSaveAsAction);
    fileMenu->addAction(fileExportToGIFAction);
    fileMenu->addAction(fileExitAction);

    QMenu* fontsMenu = new QMenu("Fonts", menuBar);
    QAction* fontsImportGirouetteAction =
            new QAction(QString("Import Girouette fonts..."), fontsMenu);
    QAction* fontsImportHELENAction =
                new QAction(QString("Import HELEN fonts..."), fontsMenu);
    QAction* fontsImportEdigirAction =
                new QAction(QString("Import Edigir fonts..."), fontsMenu);
    QAction* fontsExportAction =
                new QAction(QString("Export database to file..."), fontsMenu);
    QAction* fontsFontStudioAction =
                new QAction(QString("Open Font Studio"), fontsMenu);
    fontsMenu->addAction(fontsImportGirouetteAction);
    fontsMenu->addAction(fontsImportHELENAction);
    fontsMenu->addAction(fontsImportEdigirAction);
    fontsMenu->addAction(fontsExportAction);
    fontsMenu->addAction(fontsFontStudioAction);

    QMenu* helpMenu = new QMenu("Help", menuBar);
    QAction* helpUserGuideAction = new QAction(QString("User Guide"), fileMenu);
    QAction* helpAboutAction = new QAction(QString("About..."), fileMenu);
    helpMenu->addAction(helpUserGuideAction);
    helpMenu->addAction(helpAboutAction);

    // Generate menus
    menuBar->addMenu(fileMenu);
    menuBar->addMenu(fontsMenu);
    menuBar->addMenu(helpMenu);

    // Connect signals
    connect(fileExportToGIFAction,
            &QAction::triggered,
            uiCore,
            &SiganimUICore::saveSignToGIF);
    connect(fileExitAction,
            &QAction::triggered,
            this,
            &QMainWindow::close);

}

SiganimMainWindow::~SiganimMainWindow() {
    delete this->tabs; // deletes editor and studio
}

SignEditor* SiganimMainWindow::getEditor() const {
    return this->editor;
}
