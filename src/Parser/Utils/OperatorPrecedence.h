/*
    This is the header file used to keeps track of operator precedence.
    This is only used in parsing of binary operator expressions, to decide
    which operations should be parsed first.


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


#ifndef OPERATORPRECEDENCE_H
#define OPERATORPRECEDENCE_H

#include <string>
#include <map>
#include <optional>

#include "../../BasicComponents/LanguageLiterals.h"


namespace OperatorPrecedence {
    const int minPriority = 0;

    const std::map<std::string, int> binaryOperatorPrecedence = {
        {LT_OPERATOR_MULTIPLICATION, 40},
        {LT_OPERATOR_DIVISION, 40},
        {LT_OPERATOR_DIVISION_REMAINDER, 40},
        {LT_OPERATOR_ADDITION, 30},
        {LT_OPERATOR_SUBTRACTION, 30},
        {LT_OPERATOR_LESS_THAN, 20},
        {LT_OPERATOR_GREATER_THAN, 20},
        {LT_OPERATOR_EQUAL, 10},
        {LT_OPERATOR_NOT_EQUAL, 10},
    };

    inline std::optional<int> getOperatorPrecedence(const std::string &op) {
        auto it = binaryOperatorPrecedence.find(op);
        if (it != binaryOperatorPrecedence.end()) {
            return it->second;
        } else {
            return std::nullopt;
        }
    }
}


#endif //OPERATORPRECEDENCE_H
