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

#ifndef SRC_SIGNCELLNODE_H_
#define SRC_SIGNCELLNODE_H_

#include <vector>
#include <tuple>
#include <iostream>

#include "SignCellTree.h"
#include "SignImage.h"

class SignCellNode : public SignCellTree {
protected:
	bool checkResize() const override;

private:
	struct SignCellCoords {
		uint32_t x;
		uint32_t y;
	};

	typedef struct SignCellCoords SignCellCoords;

	typedef std::vector<SignCellTree*> SignCellPtrVector;
	typedef std::vector<SignCellTree*>::iterator SignCellPtrVectorIt;
	typedef std::vector<SignCellTree*>::const_iterator SignCellPtrVectorConstIt;
	typedef std::vector<SignCellCoords> SignCellCoordVector;
	typedef std::vector<SignCellCoords>::iterator SignCellCoordVectorIt;
	typedef std::vector<SignCellCoords>::const_iterator
			SignCellCoordVectorConstIt;

	SignCellPtrVector children;
	SignCellCoordVector childrenCoords;

public:
	SignCellNode(unsigned int height, unsigned int width);
	SignCellNode(unsigned int height, unsigned int width,
			const std::vector<std::tuple<SignCellTree*,
				unsigned int, unsigned int>> children);
	SignCellNode(unsigned int height, unsigned int width,
			std::initializer_list<std::tuple<SignCellTree*,
				unsigned int, unsigned int>> children);
	virtual ~SignCellNode();

	SignTreeType getType() const;
	void accept(SignTreeVisitor& visitor);

	bool addCell(SignCellTree* child, unsigned int x, unsigned int y);
	bool removeCell(const SignCellTree* child);

	void render(SignImage* target, const unsigned int frame,
			const unsigned int x, const unsigned int y, SignRgbPixel background,
			SignRgbPixel foreground) const;

	std::ostream& serialize(std::ostream &strm) const;
};

std::ostream& operator<<(std::ostream &strm, const SignCellNode &s);

#endif /* SRC_SIGNCELLNODE_H_ */
