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

#ifndef SRC_UI_FONTVISUALEDITOR_H_
#define SRC_UI_FONTVISUALEDITOR_H_

#include <unicode/schriter.h>
#include <string>
#include <QMouseEvent>
#include <QWidget>
#include "SignWidget.h"
#include "../sign/cells/Display.h"
#include "../sign/cells/Text.h"
#include "../render/StaticObservableSink.h"

class FontVisualEditor : public QWidget, public Observer {
    Q_OBJECT
private:
    Sign* sign;
    Display* previewDisplay;
    Text* previewTextCell;

    StaticObservableSink* sink;

    QImage* image;
    unsigned char* pixelData;

protected:
    void paintEvent(QPaintEvent* event) override;

public:
    FontVisualEditor(FontSet* fontSet, const Rasterizer* rasterizer,
            QWidget* parent = nullptr);
    virtual ~FontVisualEditor();

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    void setFont(const std::string fontFamily, const std::string fontStyle);
    void setCharacter(UChar32 index);

    void signChangedEvent();
    void observe(const Observable* sender);

    void setRasterizer(const Rasterizer* rasterizer);
};

#endif /* SRC_UI_FONTVISUALEDITOR_H_ */
