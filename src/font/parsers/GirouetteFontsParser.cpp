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

#include "../../../tinyxml/tinyxml2.h"
#include <unicode/schriter.h>
#include <unicode/ustream.h>
#include <cstddef>
#include <iostream>
#include "GirouetteFontsParser.h"
#include "ParserExceptions.h"

using namespace tinyxml2;

GirouetteFontsParser::~GirouetteFontsParser() {
}

std::vector<Font*> GirouetteFontsParser::parseGirouetteXML(
        const char* fileName) {
    std::vector<Font*> fonts;

    XMLDocument xmlDoc;
    if(xmlDoc.LoadFile(fileName) == XMLError::XML_SUCCESS) {
        XMLElement* root = xmlDoc.FirstChildElement("fontlist");
        if(root == NULL)
            throw FileLoadException(fileName);

        XMLElement* fontElt = root->FirstChildElement("font");
        while(fontElt != NULL) {
            const char* name = NULL;
            std::string nameStr;
            if(fontElt->QueryStringAttribute("name", &name)
                    == XMLError::XML_SUCCESS) {
                nameStr.assign(name);
            } else {
                nameStr = "Unnamed";
            }
            Font* font = new Font(nameStr);

            // All characters have the same height and width in this format
            // (which is its main drawback, BTW).

            unsigned int width;
            unsigned int height = 0;
            if(fontElt->QueryAttribute("x", &width) != XMLError::XML_SUCCESS)
                width = 0;

            if(fontElt->QueryAttribute("y", &height) != XMLError::XML_SUCCESS)
                height = 0;

            // Get characters
            XMLElement* charElt = fontElt->FirstChildElement("char");
            while(charElt != NULL) {
                const char* charVal = NULL;
                if(charElt->QueryStringAttribute("name", &charVal)
                        == XMLError::XML_SUCCESS) {
                    // TODO get a single UTF-8 char out of multiple
                    // const char (bytes)
                    icu::UnicodeString ucodeName(charVal);

                    UChar32 charCode = ucodeName.char32At(0);
                    if(charCode != 0xFFFF) {
                        // Name is valid. We can build the char.
                        Character* character = new Character(charCode,
                                width, height);

                        std::string dataStr;
                        const char* data = NULL;
                        if(charElt->QueryStringAttribute("data", &data)
                                == XMLError::XML_SUCCESS) {
                            // Fill in the pixel matrix
                            dataStr.assign(data);
                            unsigned int posA = 0, posB = 0;
                            unsigned int len = dataStr.length();
                            enum state { X, Y };
                            enum state s = X;

                            unsigned int x = 0;
                            unsigned int y = 0;

                            while(posA < len && posB < len) {
                                unsigned char c = dataStr[posB];
                                switch(s) {
                                case X:
                                    if(c == ',') {
                                        x = atoi(dataStr
                                                .substr(posA, (posB - posA))
                                                .c_str());
                                        posA = posB + 1;
                                        s = Y;
                                    }
                                    break;
                                case Y:
                                    if(c == ';') {
                                        y = atoi(dataStr
                                                .substr(posA, (posB - posA))
                                                .c_str());

                                        character->setBit(x, y,
                                                Character::Bit::ON);

                                        posA = posB + 1;
                                        s = X;
                                    }
                                    break;
                                }
                                posB++;
                            }

                        }
                        font->addCharacter(character);
                    }
                }

                charElt = charElt->NextSiblingElement("char");
            }

            fonts.push_back(font);

            fontElt = fontElt->NextSiblingElement("font");
        }
    } else throw FileLoadException(fileName);

    return fonts;
}

