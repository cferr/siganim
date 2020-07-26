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
    this->clearOptions();
}

QModelIndex FontQtModel::index(int row, int column,
        const QModelIndex &parent) const {
    if(this->font == nullptr)
       return QModelIndex();

    if(!parent.isValid() && column == 0) {
        std::map<int, struct CharacterOption*>::const_iterator
            option_iterator = this->charOptions.find(row);
        if(option_iterator == this->charOptions.end()) {
            throw std::runtime_error("charOptions");
        }
        struct CharacterOption* option = option_iterator->second;
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
    this->clearOptions();
    this->rebuildOptions();
    this->endResetModel();
}

void FontQtModel::setRasterizer(Rasterizer *rasterizer) {
    this->rasterizer = rasterizer;
}

void FontQtModel::setDisplayType(enum Display::Type displayType) {
    this->displayType = displayType;
}

void FontQtModel::rebuildOptions() {
    if(this->font != nullptr) {
        if(this->blockNumber == 0) {
            std::vector<UChar32> codes = this->font->listCharCodes();
            for(unsigned int i = 0; i < this->font->getNbCharacters(); ++i) {
                Character* c = this->font->get(codes.at(i));
                struct CharacterOption* option = (struct CharacterOption*)
                                        malloc(sizeof(struct CharacterOption));
                option->code = codes.at(i);
                option->hasCharacter = true;
                option->character = c;
                this->charOptions.insert(
                        std::pair<int, struct CharacterOption*>(i, option));
            }
        } else {
            UChar32 currentChar = UnicodeBlockStart(blockNumber);
            UChar32 lastChar = currentChar +
                    UnicodeBlockPointCount(blockNumber);
            int row = 0;
            while(currentChar < lastChar) {
                if(u_isgraph(currentChar)) {
                    struct CharacterOption* option = (struct CharacterOption*)
                            malloc(sizeof(struct CharacterOption));
                    option->code = currentChar;
                    try {
                        Character* c = this->font->get(currentChar);
                        option->character = c;
                        option->hasCharacter = true;
                    } catch(Font::CharNotFoundException& e) {
                        option->hasCharacter = false;
                    }
                    this->charOptions.insert(
                            std::pair<int, struct CharacterOption*>(row, option));
                    ++row;
                }
                currentChar++;
            }
        }
    }
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
    this->clearOptions();
    this->rebuildOptions();
    this->endResetModel();
}

void FontQtModel::clearOptions() {
    for(auto i = this->charOptions.begin(); i != this->charOptions.end(); ++i) {
        free(i->second);
    }
    this->charOptions.clear();
}
