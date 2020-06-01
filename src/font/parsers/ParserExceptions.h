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

#ifndef SRC_FONT_PARSERS_PARSEREXCEPTIONS_H_
#define SRC_FONT_PARSERS_PARSEREXCEPTIONS_H_

#include <exception>
#include <string>

class FileNotFoundException : public std::exception
{
private:
    const char* file;
    std::string message;
public:
    FileNotFoundException(const char* file) : file(file) {
        this->message = "File " + std::string(file) + " not found";
    }

    const char* what() const throw () {
        return this->message.c_str();
    }
    const char* getFile() const { return file; };
};

class FileLoadException : public std::exception
{
private:
    const char* file;
    std::string message;
public:
    FileLoadException(const char* file) : file(file) {
        this->message = "File " + std::string(file) + " cannot be loaded";
    }

    const char* what() const throw () {
        return this->message.c_str();
    }
    const char* getFile() const { return file; };
};



#endif /* SRC_FONT_PARSERS_PARSEREXCEPTIONS_H_ */
