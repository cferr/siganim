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


#ifndef SRC_UI_SIGNTREEQTMODEL_H_
#define SRC_UI_SIGNTREEQTMODEL_H_

#include <exception>
#include <map>
#include <vector>
#include <QAbstractItemModel>
#include <QModelIndex>
#include "../sign/SignCell.h"
#include "../sign/SignTree.h"
#include "../sign/Sign.h"
#include "SignTreeDetailsWidget.h"

class SignTreeQtModel: public QAbstractItemModel,
    public SignTreeStructureObserver {

    Q_OBJECT

private:
    class ModelBuilder : public SignTreeVisitor {
    private:
        QModelIndex parentIndex;
        unsigned int currentRow;
        SignTreeQtModel* model;

    public:
        ModelBuilder(SignTreeQtModel* model);
        ModelBuilder(SignTreeQtModel *model, QModelIndex parentIndex,
                unsigned int currentRow);

        void run(SignTree* root);

        virtual void visit(Sign& s);
        virtual void visit(Display& s);
        virtual void visit(Text& s);
        virtual void visit(Split& s);
        virtual void visit(MarqueeAnimation& s);
        virtual void visit(BlinkAnimation& s);
        virtual void visit(Compose& s);
        virtual void visit(Fill& s);
    };

public:


    class IRPointer {
    public:
        class NoSuchItemException : public std::exception {
        public:
            const char* what() {
                return "This model element has no such tree.";
            }
        };
    private:
        union {
            SignTree* signTree;
            SignCell::Builder* signCellBuilder;
            Sign::DisplayBuilder* displayBuilder;
        };
        enum { SIGN_TREE, SIGN_CELL_BUILDER, DISPLAY_BUILDER } type;
    public:
        IRPointer(SignTree* tree) {
            this->signTree = tree;
            this->type = SIGN_TREE;
        }
        IRPointer(SignCell::Builder* builder) {
            this->signCellBuilder = builder;
            this->type = SIGN_CELL_BUILDER;
        }
        IRPointer(Sign::DisplayBuilder* builder) {
            this->displayBuilder = builder;
            this->type = DISPLAY_BUILDER;
        }
        virtual ~IRPointer() { }
        SignTree* getTree();
        SignCell::Builder* getSignCellBuilder();
        Sign::DisplayBuilder* getDisplayBuilder();

    };

private:
    SignTree* tree;
    typedef std::vector<QModelIndex> QModelIndexVector;
    std::map<IRPointer*, QModelIndex> modelBackward;
    std::map<IRPointer*, QModelIndex> parentTable;
    std::map<IRPointer*, QModelIndexVector> childrenTable;
    std::map<IRPointer*, QString> labels;
    std::vector<IRPointer*> pointers;

    void addNode(const char* label, QModelIndex& index, QModelIndex& parent);
    void removeNode(QModelIndex& index);

public:
    SignTreeQtModel(SignTree* tree);
    virtual ~SignTreeQtModel() { }

    virtual QModelIndex index(int row, int column,
                              const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole)
        const;

    virtual void observeStructure(const SignTree* sender);

};

#endif /* SRC_UI_SIGNTREEQTMODEL_H_ */
