/*
    This is an expression which captures every variable declaration from source code.

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


#ifndef THESIS_COMPILER_DECLARATIONEXPRAST_H
#define THESIS_COMPILER_DECLARATIONEXPRAST_H

#include <memory>
#include <string>

#include "../AbstractExprAST.h"
#include "../../../BasicComponents/VariableType.h"
#include "../../../BasicComponents/Token.h"


class DeclarationExprAST : public virtual AbstractExprAST {
public:
    DeclarationExprAST(
        int id,
        VariableType type,
        const std::string &varName,
        Token& varNameRefTok,
        VariableScope variableScope,
        std::unique_ptr<AbstractExprAST> initExpression);

    VariableType getVariableType() const;
    const std::string &getName() const { return varName; }
    const std::unique_ptr<AbstractExprAST> &getInitExpression() const { return varValue; }
    int getID() const {return id;}

    std::string getTypeInString();
    VariableScope getScope();
    Token& getVarNameRefTok();

private:
    int id;

    VariableType varType;
    std::string varName;
    Token& varNameRefTok;

    VariableScope varScope;
    std::unique_ptr<AbstractExprAST> varValue;

    ExpressionType expressionType = ExpressionType::VariableDeclarationExpr;
};

#endif //THESIS_COMPILER_DECLARATIONEXPRAST_H
