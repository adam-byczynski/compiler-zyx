/*
    This is an expression which captures every floating point type value from source code.

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


#ifndef THESIS_COMPILER_FLOATVALUEEXPRAST_H
#define THESIS_COMPILER_FLOATVALUEEXPRAST_H

#include "../AbstractExprAST.h"

#include "../../../basicComponents/Token.h"


class FloatValueExprAST : public virtual AbstractExprAST {
public:
    explicit FloatValueExprAST(int id, float value, Token& valueRefTok) : id(id), value(value), valueRefTok(valueRefTok) {}

    int getID() const {return id;}
    double getValue() const {return value;}
    Token& getValueRefTok() const {return valueRefTok;}

private:
    int id;

    double value;
    Token& valueRefTok;

    ExpressionType expressionType = ExpressionType::FloatValueExpr;
};


#endif //THESIS_COMPILER_FLOATVALUEEXPRAST_H
