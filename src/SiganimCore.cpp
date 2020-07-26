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

#include "SiganimCore.h"

SiganimCore::SiganimCore(const std::string& databaseLocation) {
    this->defaults = new SiganimDefaults();
    this->fonts = new FontSet();
    this->rasterizers = new RasterizerSet();
    // There should always be a rasterizer.
    this->rasterizers->addRasterizer(
            new Rasterizer(this->defaults->defaultRasterizer));
    this->currentSignFile = new SiganimFileHandler();
    this->sign = new Sign();

    try {
        SiganimFileHandler* database = new SiganimFileHandler(databaseLocation);
        this->currentDatabase = database;
        this->fonts->addFonts(database->getFontSet()->getFonts());
        this->rasterizers->addRasterizers(database->getRasterizerSet()
                ->getRasterizers());
    } catch(std::exception& e) {
        // throw back?
        this->currentDatabase = nullptr;
    }

}

void SiganimCore::setCurrentSignFile(const std::string &location) {
    delete this->currentSignFile;
    if(location.empty()) {
        // "Untitled" or something like that
        this->currentSignFile = new SiganimFileHandler();
    } else {
        this->currentSignFile = new SiganimFileHandler(location);
    }
    if(this->currentSignFile->getSign() != nullptr) {
        delete this->sign;
        this->sign = this->currentSignFile->getSign();
    }
}

void SiganimCore::saveSignsToFile() {
    this->currentSignFile->setSign(this->sign);
    this->currentSignFile->save();
}

void SiganimCore::saveDatabase() {
}

void SiganimCore::exportDatabaseToFile(const std::string &location) {
}

SiganimDefaults* SiganimCore::getDefaults() const {
    return this->defaults;
}

Sign* SiganimCore::getSign() const {
    return this->sign;
}

FontSet* SiganimCore::getFontSet() const {
    return this->fonts;
}

RasterizerSet* SiganimCore::getRasterizerSet() const {
    return this->rasterizers;
}

void SiganimCore::setSign(Sign *s) {
    if(this->sign != nullptr)
        delete this->sign;
    this->sign = s;
}

SiganimCore::~SiganimCore() {
    delete this->defaults;
    this->fonts->clear();
    delete this->fonts;
    delete this->rasterizers;
    delete this->currentSignFile;
    if(this->currentDatabase != nullptr)
        delete this->currentDatabase;
    delete this->sign;
}
