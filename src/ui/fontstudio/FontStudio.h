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

#include <unicode/uchar.h>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QWidget>
#include "../../font/FontSet.h"
#include "../../render/RasterizerSet.h"
#include "FontVisualEditor.h"
#include "FontQtModel.h"
#include "CharacterListWidget.h"
#include "NoSuchCharWidget.h"
#include "../SiganimUICore.h"

class FontStudio : public QWidget {
    Q_OBJECT

private:
    SiganimUICore* uiCore;

    QGridLayout *mainLayout;
    QLabel* welcome;

    QComboBox* rasterizerCombo;

    // TODO clean up this list of attributes and move some to an char editor
    // class (which will also have height/width spinners)
    Character* currentCharacter;
    UChar32 currentCharacterCode;
    const Rasterizer* editorsRasterizer;
    FontVisualEditor* visualEditor;
    NoSuchCharWidget* noSuchChar;
    FontSet* fontSet;

    const Rasterizer* modelRasterizer;
    const RasterizerSet* rasterizerSet;

    QComboBox* fontFamilyCombo;
    QComboBox* fontStyleCombo;

    QComboBox* UnicodeBlockCombo;

    QSpinBox* heightSpinner;
    QSpinBox* widthSpinner;

    QPushButton* renameFontButton;
    QPushButton* changeStyleButton;

    CharacterListWidget* characterList;
    FontQtModel* model;

    QPushButton* commitButton;

    Font* currentFont;

    void populateFontFamilies();
    void populateFontStyles(const std::string& fontFamily);
    void populateUnicodeBlocks();
    void populateRasterizers();

public:
    FontStudio(SiganimUICore* core, QWidget* parent);
    virtual ~FontStudio();

    void setFontFamily(const std::string fontFamily);
    void setFontStyle(const std::string fontStyle);

public slots:
    void fontFamilyComboChanged(const QString& fontFamily);
    void fontStyleComboChanged(const QString& fontStyle);
    void unicodeBlockChanged(const int block);
    void setRasterizer(const QString& rasterizer);
    void setEditedCharacter(Character* c);
    void unavailableCharacterPrompt(UChar32 code);
    void setCharacterHeight(int height);
    void setCharacterWidth(int width);
    void promptRenameFont();
    void promptChangeStyle();
    void commitFontChanges();
    void createCharacterInFont();
};

#endif /* SRC_UI_FONTSTUDIO_H_ */
