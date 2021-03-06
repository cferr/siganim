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

#ifndef UI_CHARACTERLISTWIDGET_H_
#define UI_CHARACTERLISTWIDGET_H_

#include <unicode/uchar.h>
#include <QListView>
#include "../../font/Character.h"

class CharacterListWidget : public QListView {
    Q_OBJECT
protected:
    virtual void currentChanged(const QModelIndex& current,
            const QModelIndex& previous) override;

public:
    CharacterListWidget(QWidget* parent);
    virtual ~CharacterListWidget() {
    }

signals:
    void characterSelected(Character* c);
    void unavailableCharacterSelected(UChar32 code);
};

#endif /* UI_CHARACTERLISTWIDGET_H_ */
