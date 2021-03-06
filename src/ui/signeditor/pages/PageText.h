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


#ifndef UI_PAGES_PAGETEXT_H_
#define UI_PAGES_PAGETEXT_H_

#include <QComboBox>
#include <QColorDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include "../../../font/FontSet.h"
#include "../../../sign/cells/Text.h"

class PageText : public QWidget {
    Q_OBJECT

private:
    Text* treeNode;
    QLineEdit* textLine;
    QPushButton* halignLeft;
    QPushButton* halignCenter;
    QPushButton* halignRight;
    QPushButton* halignJustify;
    QComboBox* valignCombo;

    QPushButton* pickColor;
    QColorDialog* colorPicker;
    QComboBox* fontFamilyCombo;
    QComboBox* fontStyleCombo;


    const FontSet* fontSet;

    // add vertical alignment list / widget
    // add font selection widget

public:
    PageText(Text* treeNode, const FontSet* fontSet);
    virtual ~PageText() { }

    void setFontFamily(const QString& fontFamily);
    void populateFontStyles(const std::string& fontFamily);
    void setFontStyle(const QString& fontStyle);

    void updateText(const QString& text);
    void setHAlignLeft();
    void setHAlignCenter();
    void setHAlignRight();
    void setHAlignJustify();

    void selectColor();
    void setColor(const QColor& color);

public slots:
    void setVAlign(int align);
};

#endif /* UI_PAGES_PAGETEXT_H_ */
