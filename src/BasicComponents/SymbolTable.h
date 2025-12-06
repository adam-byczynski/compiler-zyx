/*
    This header file contains two important parts:
    - SymbolInfo struct, which is used for defining variables in SymbolTable
    - SymbolTable class, which is used in Semantic Analysis and Code Generation to keep track of declared variables.


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


#ifndef THESIS_COMPILER_SYMBOLTABLE_H
#define THESIS_COMPILER_SYMBOLTABLE_H


#include "VariableType.h"

#include <iostream>
#include <string>
#include <unordered_map>

#include "llvm/IR/Value.h"


struct SymbolInfo {
    std::string variableName;
    VariableType variableType;
    VariableScope variableScope;
    llvm::Value* variableValuePointer{};
};


class SymbolTable {
public:
    SymbolTable() = default;

    bool isVariableDeclared(const std::string& variableName, VariableScope variableScope) const;
    std::optional<SymbolInfo*> getVariableInfo(const std::string& variableName);
    void declareVariable(std::string& variableName, VariableType variableType, VariableScope variableScope, llvm::Value* variableValue);
    void printSymbolTable() const;
    bool setVariableValuePointer(const std::string &variableName, llvm::Value *variableValuePointer);

private:
    std::unordered_map<std::string, SymbolInfo> declaredVariables;
};


#endif //THESIS_COMPILER_SYMBOLTABLE_H
