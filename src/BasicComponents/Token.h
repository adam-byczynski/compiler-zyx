/*
    This is a Token class which inherits from AbstractToken and is used
    to keep hold all tokens found in compiler source code.


Copyright (C) 2025 Adam Byczyński.

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


#ifndef TOKEN_H
#define TOKEN_H

#include <string>

#include "TokenType.h"
#include "AbstractToken.h"

class Token : public virtual AbstractToken {
public:
    std::string value;
    int line;
    int column;

    Token(AbstractToken t, const std::string &v, int ln, int col);

    Token(TokenType t, const std::string &n, const std::string &l,
          const std::string &v, int ln, int col);

    void print();
};

#endif // TOKEN_H
