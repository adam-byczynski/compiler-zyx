/*
    This source file contains methods used by SymbolTable class.


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


#include "SymbolTable.h"


void SymbolTable::declareVariable(std::string &variableName, VariableType variableType, VariableScope variableScope,
                                  llvm::Value *variableValue) {
    declaredVariables[variableName] = {variableName, variableType, variableScope, variableValue};
}


bool SymbolTable::isVariableDeclared(const std::string& variableName, VariableScope variableScope) const {
    auto it = declaredVariables.find(variableName);
    return it != declaredVariables.end() && it->second.variableScope == variableScope;
}


bool SymbolTable::setVariableValuePointer(const std::string &variableName, llvm::Value *variableValuePointer) {
    auto it = declaredVariables.find(variableName);

    if (it != declaredVariables.end()) {
        it->second.variableValuePointer = variableValuePointer;
        return true;
    } else {
        return false;
    }
}


std::optional<SymbolInfo*> SymbolTable::getVariableInfo(const std::string &variableName) {
    auto it = declaredVariables.find(variableName);

    if(it != declaredVariables.end()){
        return &it->second;
    } else {
        return std::nullopt;
    }
}


void SymbolTable::printSymbolTable() const {
    std::cout << "\n------------------------------\n"
              << "SYMBOL TABLE: \n"
              << "------------------------------\n";
    for (const auto &entry : declaredVariables) {
        const SymbolInfo &symbolInfo = entry.second;
        std::cout << "Variable Name: " << symbolInfo.variableName << "\n";
        std::cout << "Type: " << VariableTypeUtils::getStringFromVariableType(symbolInfo.variableType).value() << "\n";
        std::cout << "Scope: " << VariableTypeUtils::getVariableScopeString(symbolInfo.variableScope) << "\n";
        std::cout << "Value Pointer: " << symbolInfo.variableValuePointer << "\n";
        std::cout << "------------------------------\n\n";
    }
}

