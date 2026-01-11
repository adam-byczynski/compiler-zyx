/*
    This source file contains methods for Token class objects


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


#include "Token.h"

#include <iostream>


void Token::print() {
    std::cout << "[L:" << line << " C:" << column << "] " << "Token -> Type: " << name
              << ", Value: \"" << value << "\"\n";
}

Token::Token(AbstractToken t, const std::string &v, int ln, int col)
        : AbstractToken(std::move(t)), value(v), line(ln), column(col) {}

Token::Token(TokenType t, const std::string &n, const std::string &l, const std::string &v, int ln, int col)
        : AbstractToken(t, n, l), value(v), line(ln), column(col) {}
