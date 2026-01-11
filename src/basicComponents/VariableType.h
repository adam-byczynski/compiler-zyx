/*
    This is a file which contains properties of variables, such as scope, and type, and their utils functions.


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


#ifndef THESIS_COMPILER_VARIABLETYPE_H
#define THESIS_COMPILER_VARIABLETYPE_H

#include <string>
#include <unordered_map>
#include <optional>
#include <stdexcept>

#include "LanguageLiterals.h"


enum class VariableScope {
    GLOBAL = 0,
};


enum class VariableType {
    INT = 0,
    STRING = 1,
    FLOAT = 2,
    BOOL = 3,
};

namespace VariableTypeUtils {
    const std::unordered_map<std::string, VariableType> variableTypeMap = {
            {LT_VARIABLE_TYPE_INT,    VariableType::INT},
            {LT_VARIABLE_TYPE_FLOAT, VariableType::FLOAT},
            {LT_VARIABLE_TYPE_STRING, VariableType::STRING},
            {LT_VARIABLE_TYPE_BOOL, VariableType::BOOL}
    };

    inline std::optional<VariableType> getVariableTypeFromString(const std::string &identifier) {
        auto it = variableTypeMap.find(identifier);
        if (it != variableTypeMap.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    inline std::optional<std::string> getStringFromVariableType(VariableType type) {
        for (const auto &pair: variableTypeMap) {
            if (pair.second == type) {
                return pair.first;
            }
        }
        return std::nullopt;
    }

    inline std::string getVariableScopeString(VariableScope scope) {
        switch (scope) {
            case VariableScope::GLOBAL:
                return "Global";
            default:
                throw std::runtime_error("ERROR: Could not get variable scope string.");
        }
    };

}

#endif //THESIS_COMPILER_VARIABLETYPE_H
