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

#ifndef SRC_SIGNDISPLAY_H_
#define SRC_SIGNDISPLAY_H_

#include <vector>
#include "SignCell.h"

class SignDisplay : public SignCell {
public:
    enum DisplayType {
        DISPLAY_RGB_LED, DISPLAY_MONOCHROME_LED, DISPLAY_FLIPDISC
    };

private:
    // TODO add some geometry info (margins, display size in cm/in, dpi...)
    unsigned int width;
    unsigned int height;

    enum DisplayType displayType;  // LEDs (monochromatic, RGB) or flip discs.

    SignCell *rootCell;

    const Sign *parentSign;

public:
    SignDisplay(unsigned int width, unsigned int height,
            enum DisplayType type);
    SignDisplay(unsigned int width, unsigned int height,
            enum DisplayType type, SignCell *rootCell);
    virtual ~SignDisplay();

    virtual bool setParent(const SignCell* parent);
    virtual Type getType() const {
        return Type::DISPLAY;
    }

    virtual const char* CellTypeStr() const {
        return "Display";
    }

    virtual void accept(SignTreeVisitor &visitor);
    virtual void accept(ConstSignTreeVisitor &visitor) const;

    bool setParentSign(const Sign* parent);
    const Sign* getParentSign() const;

    bool setRootCell(SignCell *rootCell);
    SignCell* getRootCell() const;

    enum DisplayType getDisplayType() const;
    bool setDisplayType(enum DisplayType displayType);

    unsigned int getHeight() const;
    unsigned int getWidth() const;
    void setHeight(const unsigned int height);
    void setWidth(const unsigned int width);
    void resize(const unsigned int width, const unsigned int height);
    virtual unsigned int getChildHeight(const SignCell* child) const override;
    virtual unsigned int getChildWidth(const SignCell* child) const override;

    std::ostream& serialize(std::ostream &strm) const;
};

std::ostream& operator<<(std::ostream &strm, const SignDisplay &s);

#endif /* SRC_SIGNDISPLAY_H_ */
