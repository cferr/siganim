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


#ifndef SRC_UI_FONTQTMODEL_H_
#define SRC_UI_FONTQTMODEL_H_

#include <QAbstractItemModel>
#include <QModelIndex>
#include "../font/Font.h"
#include "../render/Rasterizer.h"
#include "../sign/cells/Display.h"

class FontQtModel : public QAbstractItemModel {
    Q_OBJECT

private:
    Font* font;
    const Rasterizer* rasterizer;
    enum Display::Type displayType;

public:
    FontQtModel(Font* font, const Rasterizer* rasterizer,
            enum Display::Type displayType);
    virtual ~FontQtModel() { }

    void setFont(Font* font);
    void setRasterizer(Rasterizer* rasterizer);
    void setDisplayType(enum Display::Type displayType);

    virtual QModelIndex index(int row, int column,
                              const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole)
        const;

};

#endif /* SRC_UI_FONTQTMODEL_H_ */