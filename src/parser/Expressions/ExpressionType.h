/*
    This is a header file used for utilities related to expressions.


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


#ifndef THESIS_COMPILER_EXPRESSIONTYPE_H
#define THESIS_COMPILER_EXPRESSIONTYPE_H

#include <string>
#include <stdexcept>


enum class ExpressionType {
    AbstractExpr = 0,
    ProgramExpr = 1,

    IntegerValueExpr = 2,
    FloatValueExpr = 3,
    StringValueExpr = 4,
    BooleanValueExpr = 5,

    VariableReferenceExpr = 6,
    VariableAssignmentExpr = 7,
    VariableDeclarationExpr = 8,
    FunctionPrintValueExpr = 9,

    BinaryOperatorExpr = 10,

    OperatorAdditionExpr = 11,
    OperatorSubtractionExpr = 12,
    OperatorMultiplicationExpr = 13,
    OperatorDivisionExpr = 14,

    BranchingIfExpr = 15,
    LoopWhileExpr = 16,

    BlockExpr = 17
};


inline std::string getExpressionTypeInString(ExpressionType type) {
    switch (type) {
        case ExpressionType::AbstractExpr:
            return "Abstract Expression";
        case ExpressionType::ProgramExpr:
            return "Program Expression";
        case ExpressionType::IntegerValueExpr:
            return "Integer Value Expression";
        case ExpressionType::FloatValueExpr:
            return "Float Value Expression";
        case ExpressionType::StringValueExpr:
            return "String Value Expression";
        case ExpressionType::VariableReferenceExpr:
            return "Variable Reference Expression";
        case ExpressionType::VariableAssignmentExpr:
            return "Variable Assignment Expression";
        case ExpressionType::VariableDeclarationExpr:
            return "Variable Declaration Expression";
        case ExpressionType::FunctionPrintValueExpr:
            return "Function Print Value Expression";
        case ExpressionType::BinaryOperatorExpr:
            return "Binary Operator Expression";
        case ExpressionType::OperatorAdditionExpr:
            return "Operator Addition Expression";
        case ExpressionType::OperatorSubtractionExpr:
            return "Operator Subtraction Expression";
        case ExpressionType::OperatorMultiplicationExpr:
            return "Operator Multiplication Expression";
        case ExpressionType::OperatorDivisionExpr:
            return "Operator Division Expression";
        case ExpressionType::BranchingIfExpr:
            return "Branching If Expression";
        case ExpressionType::LoopWhileExpr:
            return "Loop While Expression";
        case ExpressionType::BlockExpr:
            return "Block Expression";
        default:
            throw std::runtime_error("ERROR: Could not get a string of Expression Type");
    }
}


#endif //THESIS_COMPILER_EXPRESSIONTYPE_H
