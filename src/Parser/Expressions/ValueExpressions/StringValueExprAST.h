/*
    This is an expression class which captures every immediate string in the source code.


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


#ifndef THESIS_COMPILER_STRINGVALUEEXPRAST_H
#define THESIS_COMPILER_STRINGVALUEEXPRAST_H

#include <string>

#include "../AbstractExprAST.h"
#include "../../../BasicComponents/Token.h"


class StringValueExprAST : public virtual AbstractExprAST {
public:
    explicit StringValueExprAST(int id, const std::string& value, Token& valueRefTok) : id(id), value(value), valueRefTok(valueRefTok) {}

    int getID() const {return id;}
    const std::string &getStringValue() const;
    Token& getValueRefTok() const {return valueRefTok;}


private:
    int id;

    std::string value;
    Token& valueRefTok;

    ExpressionType expressionType = ExpressionType::StringValueExpr;
};

#endif //THESIS_COMPILER_STRINGVALUEEXPRAST_H
