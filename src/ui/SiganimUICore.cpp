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

#include <QFileDialog>
#include <QString>
#include "SiganimUICore.h"
#include "../render/GIFSink.h"
#include "SiganimMainWindow.h"

SiganimUICore::SiganimUICore(SiganimCore* core) {
    this->core = core;
    this->mainWindow = new SiganimMainWindow(this);
}

SiganimUICore::~SiganimUICore() {
    delete this->mainWindow;
}

void SiganimUICore::showUI() {
    this->mainWindow->show();
}

void SiganimUICore::saveSignToGIF() {
    // Show "Save as" dialog
    QFileDialog saveDialog(this->mainWindow, "Save GIF to...", ".", "*.gif");
    saveDialog.setAcceptMode(QFileDialog::AcceptMode::AcceptSave);

    if(saveDialog.exec() == QFileDialog::Accepted) {
        QStringList selectedFiles = saveDialog.selectedFiles();
        // There should be only one - take it
        QString fileName = selectedFiles.first();
        GIFSink s(core->getSign(), core->getFontSet(),
                mainWindow->getEditor()->getCurrentRasterizer(), 1);
        std::string fileNameAsString = fileName.toStdString();
        s.render(fileNameAsString.data());
    }
}

SiganimCore* SiganimUICore::getCore() const {
    return this->core;
}
