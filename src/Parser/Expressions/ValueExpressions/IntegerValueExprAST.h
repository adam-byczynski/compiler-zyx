/*
    This is an expression class which captures every occurrence of Integer value from source code.

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


#ifndef THESIS_COMPILER_INTEGERVALUEEXPRAST_H
#define THESIS_COMPILER_INTEGERVALUEEXPRAST_H

#include "../AbstractExprAST.h"

#include "../../../BasicComponents/Token.h"


class IntegerValueExprAST : public virtual AbstractExprAST {
public:
    explicit IntegerValueExprAST(int id, int value, Token& valueRefTok);
    long long int getValue()  const { return value; }
    int getID() const { return id; }

    Token& getValueRefTok() const {return valueRefTok;}

private:
    int id;

    long long int value;
    Token& valueRefTok;

    ExpressionType expressionType = ExpressionType::IntegerValueExpr;
};


#endif //THESIS_COMPILER_INTEGERVALUEEXPRAST_H
