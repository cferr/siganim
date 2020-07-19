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

#ifndef RENDER_RASTERIZERSET_H_
#define RENDER_RASTERIZERSET_H_

#include <map>
#include "Rasterizer.h"

class RasterizerSet {
public:
    class RasterizerNotFoundException : public std::exception
    {
    private:
        const RasterizerSet* rset;
        std::string rasterizer;
        std::string message;
    public:
        RasterizerNotFoundException(const RasterizerSet* rset,
                std::string rasterizer) :
            rset(rset), rasterizer(rasterizer) {
            this->message = "Rasterizer " + rasterizer + " not found";
        }

        const char* what() const throw () {
            return this->message.c_str();
        }

        const RasterizerSet* getRasterizerSet() const { return rset; };
        const std::string getRasterizer() const { return rasterizer; };
    };

private:
    std::map<std::string, Rasterizer*> rasterizers;

public:
    RasterizerSet();
    RasterizerSet(std::vector<Rasterizer*> rasterizers);
    RasterizerSet(std::initializer_list<Rasterizer*> rasterizers);
    virtual ~RasterizerSet();

    void addRasterizer(Rasterizer* rasterizer);
    void addRasterizers(const std::vector<Rasterizer*> rasterizers);
    bool removeRasterizer(Rasterizer* rasterizer);
    bool removeRasterizer(const std::string& name);

    Rasterizer* get(const std::string& name);
    const Rasterizer* getConst(const std::string& name) const;

    std::vector<Rasterizer*> getRasterizers() const;

};

#endif /* RENDER_RASTERIZERSET_H_ */
