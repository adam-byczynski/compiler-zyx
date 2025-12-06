/*
    This is base, abstract class inherited by each Expression class later.

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


#ifndef THESIS_COMPILER_ABSTRACTEXPRAST_H
#define THESIS_COMPILER_ABSTRACTEXPRAST_H

#include <string>

#include "ExpressionType.h"


class AbstractExprAST {
public:
    virtual ~AbstractExprAST() = default;
    virtual ExpressionType getExpressionType() const {return expressionType;}
    virtual std::string getExpressionTypeString() const {return getExpressionTypeInString(this->expressionType);}


private:
    int id;

    ExpressionType expressionType = ExpressionType::AbstractExpr;
};

#endif //THESIS_COMPILER_ABSTRACTEXPRAST_H
