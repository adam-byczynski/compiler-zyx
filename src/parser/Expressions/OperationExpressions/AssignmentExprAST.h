/*
    This is an expression class which captures every variable assigment from the source code.

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


#ifndef THESIS_COMPILER_ASSIGNMENTEXPRAST_H
#define THESIS_COMPILER_ASSIGNMENTEXPRAST_H

#include <memory>
#include <string>

#include "../AbstractExprAST.h"
#include "../../../basicComponents/Token.h"
#include "../../../basicComponents/VariableType.h"


class AssignmentExprAST : public virtual AbstractExprAST{
public:
    AssignmentExprAST(
        int id,
        const std::string &varName,
        Token& varNameRefTok,
        std::unique_ptr<AbstractExprAST> newValue,
        VariableScope variableScope
        ) :
    id(id),
    variableName(varName),
    variableNameReferenceToken(varNameRefTok),
    variableReferenceScope(variableScope),
    newValue(std::move(newValue)) {}

    int getID() const { return id; }
    const std::string &getVariableName() const { return variableName; }
    const std::unique_ptr<AbstractExprAST> &getNewValue() const { return newValue; }
    Token& getVariableNameReferenceToken() { return variableNameReferenceToken; }
    VariableScope getVariableScope() { return variableReferenceScope; }

private:
    int id;

    std::string variableName;
    Token& variableNameReferenceToken;
    VariableScope variableReferenceScope;

    std::unique_ptr<AbstractExprAST> newValue;

    ExpressionType expressionType = ExpressionType::VariableAssignmentExpr;
};


#endif //THESIS_COMPILER_ASSIGNMENTEXPRAST_H
