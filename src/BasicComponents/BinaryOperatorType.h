/*
    Main file defining various operators.
    - BinaryOperatorType is a core enum which is used to differentiate each operator.
    - BinaryOperatorTypeUtils handle various utility functions such as mapping operator literal to operator enum.


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


#ifndef BINARYOPERATORTYPE_H
#define BINARYOPERATORTYPE_H


#include <unordered_map>
#include <string>

#include "LanguageLiterals.h"


enum class BinaryOperatorType {
    ADD = 0,
    SUBTRACT = 1,
    MULTIPLY = 2,
    DIVIDE = 3,
    DIVIDE_REMAINDER = 8,
    LESS_THAN = 4,
    GREATER_THAN = 5,
    EQUAL = 6,
    NOT_EQUAL = 7
};

namespace BinaryOperatorTypeUtils {
    inline const std::unordered_map<std::string, BinaryOperatorType> operatorMap = {
        {LT_OPERATOR_ADDITION, BinaryOperatorType::ADD},
        {LT_OPERATOR_SUBTRACTION, BinaryOperatorType::SUBTRACT},
        {LT_OPERATOR_MULTIPLICATION, BinaryOperatorType::MULTIPLY},
        {LT_OPERATOR_DIVISION, BinaryOperatorType::DIVIDE},
        {LT_OPERATOR_DIVISION_REMAINDER, BinaryOperatorType::DIVIDE_REMAINDER},
        {LT_OPERATOR_LESS_THAN, BinaryOperatorType::LESS_THAN},
        {LT_OPERATOR_GREATER_THAN, BinaryOperatorType::GREATER_THAN},
        {LT_OPERATOR_EQUAL, BinaryOperatorType::EQUAL},
        {LT_OPERATOR_NOT_EQUAL, BinaryOperatorType::NOT_EQUAL}
    };

    inline std::optional<BinaryOperatorType> getBinaryOperatorType(const std::string &op) {
        auto it = operatorMap.find(op);
        if (it != operatorMap.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    inline std::optional<std::string> getBinaryOperatorTypeString(BinaryOperatorType op) {
        for (const auto &pair: operatorMap) {
            if (pair.second == op) {
                return pair.first;
            }
        }
        return std::nullopt;
    }

    inline std::string getOperatorString(BinaryOperatorType opType) {
        switch (opType) {
            case BinaryOperatorType::ADD: return LT_OPERATOR_ADDITION;
            case BinaryOperatorType::SUBTRACT: return LT_OPERATOR_SUBTRACTION;
            case BinaryOperatorType::MULTIPLY: return LT_OPERATOR_MULTIPLICATION;
            case BinaryOperatorType::DIVIDE: return LT_OPERATOR_DIVISION;
            case BinaryOperatorType::DIVIDE_REMAINDER: return LT_OPERATOR_DIVISION_REMAINDER;
            case BinaryOperatorType::EQUAL: return LT_OPERATOR_EQUAL;
            case BinaryOperatorType::NOT_EQUAL: return LT_OPERATOR_NOT_EQUAL;
            case BinaryOperatorType::LESS_THAN: return LT_OPERATOR_LESS_THAN;
            case BinaryOperatorType::GREATER_THAN: return LT_OPERATOR_GREATER_THAN;
            default: return "unknown";
        }
    }
}


#endif //BINARYOPERATORTYPE_H
