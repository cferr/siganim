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

#include <cstdlib>
#include <QColor>
#include <QIcon>
#include <QPixmap>
#include <QString>
#include <QVariant>
#include "FontQtModel.h"
#include "../font/UnicodeUtils.h"
#include "../render/SingleFrameSink.h"
#include "../sign/cells/Text.h"
#include "../sign/Sign.h"

FontQtModel::FontQtModel(Font* font, const Rasterizer* rasterizer,
        enum Display::Type displayType) : font(font),
        rasterizer(rasterizer), displayType(displayType) {
    this->singleFontSet.addFont(font);
}

FontQtModel::~FontQtModel() {
    // Fonts in here don't belong to us. Clear them out.
    this->singleFontSet.clear();
}

QModelIndex FontQtModel::index(int row, int column,
        const QModelIndex &parent) const {
    std::vector<UChar32> codes = this->font->listCharCodes();
    if(!parent.isValid() && column == 0) {
        return this->createIndex(row, column, this->font->get(codes.at(row)));
    } else return QModelIndex();
}

QModelIndex FontQtModel::parent(const QModelIndex &child) const {
    return QModelIndex();
}

int FontQtModel::rowCount(const QModelIndex &parent) const {
    if(!parent.isValid()) {
        return (int)(this->font->getNbCharacters());
    } else return 0;
}

int FontQtModel::columnCount(const QModelIndex &parent) const {
    if(!parent.isValid()) {
        return 1;
    } else return 0;
}

void FontQtModel::setFont(Font *font) {
    this->beginResetModel();
    this->singleFontSet.removeFont(this->font);
    this->font = font;
    this->singleFontSet.addFont(this->font);
    this->endResetModel();
}

void FontQtModel::setRasterizer(Rasterizer *rasterizer) {
    this->rasterizer = rasterizer;
}

void FontQtModel::setDisplayType(enum Display::Type displayType) {
    this->displayType = displayType;
}

QVariant FontQtModel::data(const QModelIndex &index,
        int role) const {
    Character* c = (Character*)index.internalPointer();
    UChar32 cval = c->getUTF8Code();
    if(role == Qt::DisplayRole) {
        return QString::fromStdString(UnicodeCharName(c->getUTF8Code()));
    } else if(role == Qt::DecorationRole) {
        Sign* tempSign = new Sign({
                new Display(c->getWidth(), c->getHeight(),
                        this->displayType,
                        new Text(this->font->getFamily(),
                                this->font->getStyle(),
                                Text::HALIGN_CENTER,
                                Text::VALIGN_CENTER_CENTER,
                                icu::UnicodeString::fromUTF32(&cval, 1))
                )
        });
        SingleFrameSink* sf = new SingleFrameSink(&this->singleFontSet,
                this->rasterizer);
        Bitmap* renderedChar = sf->render(tempSign, 0);
        unsigned char* bmap = renderedChar->toRGB32();
        QImage img(bmap, renderedChar->getWidth(),
                renderedChar->getHeight(), QImage::Format_RGB32);
        QPixmap resizedPmap = QPixmap::fromImage(img.scaled(32, 32,
                Qt::KeepAspectRatio));
        delete tempSign;
        delete sf;
        delete renderedChar;
        free(bmap);
        return QIcon(resizedPmap);
    } else {
        return QVariant(QVariant::Invalid);
    };
}

