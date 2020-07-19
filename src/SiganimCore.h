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

#ifndef SIGANIMCORE_H_
#define SIGANIMCORE_H_

#include <vector>
#include "SiganimDefaults.h"
#include "SiganimFileHandler.h"
#include "sign/Sign.h"
#include "font/FontSet.h"
#include "render/RasterizerSet.h"

class SiganimCore {
    SiganimDefaults* defaults;
    Sign* sign;
    FontSet* fonts;
    RasterizerSet* rasterizers;
    SiganimFileHandler* currentSignFile;
    SiganimFileHandler* currentDatabase;

public:
    SiganimCore(const std::string& databaseLocation);

    void setCurrentSignFile(const std::string& location = "");
    void saveSignsToFile();
    void saveDatabase();
    void exportDatabaseToFile(const std::string& location);

    SiganimDefaults* getDefaults() const;

    Sign* getSign() const;
    void setSign(Sign* s);
    FontSet* getFontSet() const;
    RasterizerSet* getRasterizerSet() const;

    virtual ~SiganimCore();
};

#endif /* SIGANIMCORE_H_ */
