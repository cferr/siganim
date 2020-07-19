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
        rasterizer(rasterizer), displayType(displayType), blockNumber(0) {
    if(font != nullptr)
        this->singleFontSet.addFont(font);
}

FontQtModel::~FontQtModel() {
    // Fonts in here don't belong to us. Clear them out.
    this->singleFontSet.clear();
}

QModelIndex FontQtModel::index(int row, int column,
        const QModelIndex &parent) const {
    if(this->font == nullptr)
       return QModelIndex();

    std::vector<UChar32> codes = this->font->listCharCodes();
    if(!parent.isValid() && column == 0) {
        struct CharacterOption* option =
                (struct CharacterOption*)malloc(sizeof(struct CharacterOption));
        if(this->blockNumber == 0) {
            option->character = this->font->get(codes.at(row));
            option->code = codes.at(row);
            option->hasCharacter = true;
        } else {
//            UChar32 code = (unsigned int)row +
//                                    UnicodeBlockStart(this->blockNumber);
            UChar32 code = UnicodeNthPrintableChar(this->blockNumber, row);
            option->code = code;
            try {
                option->character = this->font->get(code);
                option->hasCharacter = true;
            } catch(Font::CharNotFoundException& e) {
                option->character = nullptr;
                option->hasCharacter = false;
            }
        }
        return this->createIndex(row, column, option);
    } else return QModelIndex();
}

QModelIndex FontQtModel::parent(const QModelIndex &child) const {
    return QModelIndex();
}

int FontQtModel::rowCount(const QModelIndex &parent) const {
    if(!parent.isValid() && this->font != nullptr) {
        if(this->blockNumber == 0) {
            return (int)(this->font->getNbCharacters());
        } else {
            // TODO add non-printable boolean to allow such chars
            int nbCodePoints = UnicodeBlockPointCount(this->blockNumber);
            UChar32 start = UnicodeBlockStart(this->blockNumber);
            unsigned count = 0;
            for(int i = 0; i < nbCodePoints; ++i) {
                if(u_isgraph(start + (unsigned int)i)) {
                    count++;
                }
            }
            return count;
        }
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
    if(font != nullptr)
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
    struct CharacterOption* c =
            (struct CharacterOption*)index.internalPointer();
    UChar32 charU8Code = c->code;
    if(role == Qt::DisplayRole) {
        return QString::fromStdString(UnicodeCharName(charU8Code));
    } else if(role == Qt::DecorationRole) {
        if(c->hasCharacter) {
            Character* charModel = c->character;
            Sign* tempSign = new Sign({
                    new Display(charModel->getWidth(), charModel->getHeight(),
                            this->displayType,
                            new Text(this->font->getFamily(),
                                this->font->getStyle(),
                                Text::HALIGN_CENTER,
                                Text::VALIGN_CENTER_CENTER,
                                icu::UnicodeString::fromUTF32(&charU8Code, 1))
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
            QIcon icon = QIcon::fromTheme("image-missing");
            QPixmap iconPixmap =
                    icon.pixmap(32, 32, QIcon::Mode::Normal, QIcon::State::On);
            return QIcon(iconPixmap);
        }
    } else {
        return QVariant(QVariant::Invalid);
    };
}

void FontQtModel::setBlockNumber(unsigned int blockNumber) {
    this->beginResetModel();
    this->blockNumber = blockNumber;
    this->endResetModel();
}
