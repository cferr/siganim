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

#ifndef SIGN_SIGANIMJSONSIGNPARSER_H_
#define SIGN_SIGANIMJSONSIGNPARSER_H_

#include <exception>
#include <json-c/json.h>
#include "Sign.h"

class SiganimJSONSignParser {
public:
    class MissingPropertyException : public std::exception {
        std::string propertyName;
    public:
        MissingPropertyException(const std::string& propertyName) :
            propertyName(propertyName) {}
        virtual ~MissingPropertyException() {};
        const char* what() const throw () {
            return propertyName.c_str();
        };
    };

    class BadPropertyValueException : public std::exception {
        std::string propertyName;
    public:
        BadPropertyValueException(const std::string& propertyName) :
            propertyName(propertyName) {}
        virtual ~BadPropertyValueException() {};
        const char* what() const throw () {
            return propertyName.c_str();
        };
    };

    class UnsupportedPropertyTypeException : public std::exception {
        std::string propertyName;
        std::string propertyType;
        std::string expectedType;
        std::string message;
    public:
        UnsupportedPropertyTypeException(const std::string& propertyName,
                const std::string& propertyType,
                const std::string& expectedType) :
            propertyName(propertyName), propertyType(propertyType),
            expectedType(expectedType) {
            this->message = propertyName + ": expected type " + expectedType
                    + ", got " + propertyType;
        }
        virtual ~UnsupportedPropertyTypeException() {};
        const char* what() const throw () {
            return message.c_str();
        };
    };


private:
    static BlinkAnimation* buildBlinkAnimation(json_object* obj);
    static Compose* buildCompose(json_object* obj);
    static Display* buildDisplay(json_object* obj);
    static Fill* buildFill(json_object* obj);
    static MarqueeAnimation* buildMarqueeAnimation(json_object* obj);
    static Split* buildSplit(json_object* obj);
    static Text* buildText(json_object* obj);
    static SignColor buildSignColor(json_object* obj);

public:
    virtual ~SiganimJSONSignParser() {};

    static Sign* parseJSON(json_object* obj);
    static SignCell* buildCell(json_object* obj);

};

#endif /* SIGN_SIGANIMJSONSIGNPARSER_H_ */
