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

#ifndef SIGANIMFILEHANDLER_H_
#define SIGANIMFILEHANDLER_H_

#include <vector>
#include "font/FontSet.h"
#include "sign/Sign.h"

class SiganimFileHandler {
private:
    std::string location;
    FontSet* fonts;
    std::vector<Sign*> signs;

    void parseInputFile();

public:
    SiganimFileHandler();
    SiganimFileHandler(const char* location);
    SiganimFileHandler(const std::string& location);
    virtual ~SiganimFileHandler();

    void setLocation(const std::string& location);
    std::string getLocation() const;

    // Perhaps there should be a SignSet...
    Sign* getSign(unsigned int number) const;
    void addSign(Sign* s);
    void removeSign(unsigned int number);
    void removeSign(Sign* s);

    FontSet* getFontSet() const;

    bool save();

};

#endif /* SIGANIMFILEHANDLER_H_ */
