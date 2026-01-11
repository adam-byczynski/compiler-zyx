/*
    This is an expression which captures every print operation from source code.
    Whenever a print expression is found in source code, a FunctionPrintValueExprAST is created.


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


#ifndef THESIS_COMPILER_FUNCTIONPRINTVALUEEXPRAST_H
#define THESIS_COMPILER_FUNCTIONPRINTVALUEEXPRAST_H

#include <memory>

#include "../AbstractExprAST.h"

#include "../../../basicComponents/Token.h"


class FunctionPrintValueExprAST : public virtual AbstractExprAST {
public:
    explicit FunctionPrintValueExprAST(int id, std::unique_ptr<AbstractExprAST> arguments, Token& argRefTok);
    const std::unique_ptr<AbstractExprAST> &getArgument() const;
    int getID() const {return id;}

    Token& getArgumentRefTok() {
        return argRefTok;
    }

private:
    int id;

    std::unique_ptr<AbstractExprAST> argument;
    Token& argRefTok;

    ExpressionType expressionType = ExpressionType::FunctionPrintValueExpr;
};


#endif //THESIS_COMPILER_FUNCTIONPRINTVALUEEXPRAST_H
