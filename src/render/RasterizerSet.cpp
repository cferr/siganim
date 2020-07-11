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


#include "RasterizerSet.h"

RasterizerSet::RasterizerSet() {

}

RasterizerSet::RasterizerSet(std::vector<Rasterizer*> rasterizers) {
    for(auto i = rasterizers.begin(); i < rasterizers.end(); ++i)
        this->addRasterizer(*i);
}

RasterizerSet::RasterizerSet(std::initializer_list<Rasterizer*> rasterizers) {
    for(auto i = rasterizers.begin(); i < rasterizers.end(); ++i)
        this->addRasterizer(*i);
}

void RasterizerSet::addRasterizer(Rasterizer *rasterizer) {
    this->rasterizers.insert(
            std::pair<std::string, Rasterizer*>(
                    rasterizer->getName(), rasterizer));
}

bool RasterizerSet::removeRasterizer(Rasterizer *rasterizer) {
    auto iterator = this->rasterizers.begin();
    bool ret = false;
    while(iterator != this->rasterizers.end()) {
        if((*iterator).second == rasterizer) {
            ret = true;
            iterator = this->rasterizers.erase(iterator);
        } else iterator++;
    }
    return ret;
}

bool RasterizerSet::removeRasterizer(const std::string& name) {
    try {
        this->removeRasterizer(this->get(name));
        return true;
    } catch(RasterizerNotFoundException& e) {
        return false;
    }
}

Rasterizer* RasterizerSet::get(const std::string& name) {
    auto iterator = this->rasterizers.find(name);
    if(iterator != this->rasterizers.end()) {
        return (*iterator).second;
    } else throw RasterizerNotFoundException(this, name);
}

const Rasterizer* RasterizerSet::getConst(const std::string& name) const {
    auto iterator = this->rasterizers.find(name);
    if(iterator != this->rasterizers.end()) {
        return (*iterator).second;
    } else throw RasterizerNotFoundException(this, name);
}


std::vector<Rasterizer*> RasterizerSet::getRasterizers() const {
    std::vector<Rasterizer*> ret;
    for(auto i = this->rasterizers.begin(); i != this->rasterizers.end();
            ++i)
        ret.push_back((*i).second);
    return ret;
}

RasterizerSet::~RasterizerSet() {
    for(auto i = this->rasterizers.begin(); i != this->rasterizers.end();
                ++i)
        delete (*i).second;
}
