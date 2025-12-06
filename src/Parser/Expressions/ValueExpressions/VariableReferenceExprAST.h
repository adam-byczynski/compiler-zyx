/*
    This is an expression class used for keeping track of variable references.

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


#ifndef THESIS_COMPILER_VARIABLEREFERENCEEXPRAST_H
#define THESIS_COMPILER_VARIABLEREFERENCEEXPRAST_H

#include <string>

#include "../AbstractExprAST.h"
#include "../../../BasicComponents/Token.h"
#include "../../../BasicComponents/VariableType.h"


class VariableReferenceExprAST : public virtual AbstractExprAST {
public:
    explicit VariableReferenceExprAST(
        int id,
        const std::string &refVarName,
        Token& refVarNameRefTok,
        VariableScope scope) : id(id), refVarName(refVarName), refVarNameRefTok(refVarNameRefTok), referencedScope(scope) {}

    void setReferencedScope(VariableScope scope) { referencedScope = scope; }

    int getID() const { return id; }
    const std::string &getReferencedVariableName() const { return refVarName; }
    Token& getValueRefTok() const { return refVarNameRefTok; }
    VariableScope getReferencedScope() const { return referencedScope; }

private:
    int id;

    std::string refVarName;
    Token& refVarNameRefTok;

    VariableScope referencedScope;
    ExpressionType expressionType = ExpressionType::VariableReferenceExpr;
};


#endif //THESIS_COMPILER_VARIABLEREFERENCEEXPRAST_H
