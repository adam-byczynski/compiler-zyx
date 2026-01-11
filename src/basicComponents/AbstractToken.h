/*
    Abstract class inherited as a basis for a Token object


Copyright (C) 2026 Adam Byczyński.

This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.
    If not, see <https://www.gnu.org/licenses/>.  */


#ifndef ABSTRACTTOKENTYPE_H
#define ABSTRACTTOKENTYPE_H


#include <string>

#include "TokenType.h"


class AbstractToken {
public:
    TokenType type;
    std::string name;
    std::string lexeme;

    AbstractToken(TokenType t, const std::string& n, const std::string& l);
};

#endif //ABSTRACTTOKENTYPE_H
