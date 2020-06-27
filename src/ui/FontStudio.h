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

#ifndef SRC_UI_FONTSTUDIO_H_
#define SRC_UI_FONTSTUDIO_H_

#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>
#include <QWidget>
#include "../font/FontSet.h"
#include "../render/RasterizerSet.h"
#include "FontVisualEditor.h"
#include "FontQtModel.h"
#include "CharacterListWidget.h"

class FontStudio : public QWidget {
    Q_OBJECT

private:
    QGridLayout *mainLayout;
    QLabel* welcome;

    QComboBox* rasterizerCombo;

    // TODO clean up this list of attributes and move some to an char editor
    // class (which will also have height/width spinners)
    Character* currentCharacter;
    const Rasterizer* editorsRasterizer;
    FontVisualEditor* visualEditor;
    FontSet* fontSet;

    const Rasterizer* modelRasterizer;
    const RasterizerSet* rasterizerSet;

    QComboBox* fontFamilyCombo;
    QComboBox* fontStyleCombo;

    QSpinBox* heightSpinner;
    QSpinBox* widthSpinner;

    CharacterListWidget* characterList;
    FontQtModel* model;

    Font* currentFont;

    void populateFontStyles(const std::string& fontFamily);
    void populateCharacters(const std::string& fontFamily,
            const std::string& fontStyle);

public:
    FontStudio(FontSet* fontSet, const RasterizerSet* rasterizerSet,
            QWidget* parent);
    virtual ~FontStudio() { }

    void setFontFamily(const std::string fontFamily);
    void setFontStyle(const std::string fontStyle);

public slots:
    void fontFamilyComboChanged(const QString& fontFamily);
    void fontStyleComboChanged(const QString& fontStyle);
    void setRasterizer(const QString& rasterizer);
    void setEditedCharacter(Character* c);
    void setCharacterHeight(int height);
    void setCharacterWidth(int width);
};

#endif /* SRC_UI_FONTSTUDIO_H_ */
