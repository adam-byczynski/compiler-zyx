/*  This is a class file for Semantic Analyzer, which comes right after parsing.
    Its role is to analyze every expression in Abstract Syntax Tree and assure it is correct.
    If any issue is found, it will raise error and exit process.

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


#ifndef THESIS_COMPILER_SEMANTICANALYZER_H
#define THESIS_COMPILER_SEMANTICANALYZER_H

#include <iostream>
#include <regex>
#include <cmath>

#include "../basicComponents/TokenType.h"
#include "../basicComponents/SymbolTable.h"

#include "../parser/expressions/keywordExpressions/BranchingIfExprAST.h"
#include "../parser/expressions/keywordExpressions/LoopWhileExprAST.h"
#include "../parser/expressions/operationExpressions/AssignmentExprAST.h"
#include "../parser/expressions/operationExpressions/DeclarationExprAST.h"
#include "../parser/expressions/operationExpressions/FunctionPrintValueExprAST.h"
#include "../parser/expressions/ProgramExprAST.h"
#include "../parser/expressions/operationExpressions/BinaryOperatorExprAST.h"
#include "../parser/expressions/utilsExpressions/BlockExprAST.h"
#include "../parser/expressions/valueExpressions/BooleanValueExprAST.h"
#include "../parser/expressions/valueExpressions/FloatValueExprAST.h"
#include "../parser/expressions/valueExpressions/IntegerValueExprAST.h"
#include "../parser/expressions/valueExpressions/StringValueExprAST.h"
#include "../parser/expressions/valueExpressions/VariableReferenceExprAST.h"

#include "../utilities/Logger.h"


class SemanticAnalyzer {
public:
    explicit SemanticAnalyzer(Logger& logger, ProgramExprAST &parsingTree, SymbolTable& symbolTable);

    void performAnalysis();


private:
    Logger& logger;

    ProgramExprAST &parsingTree;
    SymbolTable& symbolTable;

    static constexpr size_t MAX_STRING_LENGTH = 255;

    // Various utils functions
    std::string getPosition(const Token & token);
    VariableType getBinaryOperationReturnType(BinaryOperatorExprAST* expr);
    VariableType determineExpressionReturnType(AbstractExprAST* expr);
    bool areOperandTypesCompatible(BinaryOperatorType opType, VariableType lhsType, VariableType rhsType);
    bool checkIfValueMatchesType(VariableType variableType, const std::unique_ptr<AbstractExprAST> &expr);
    bool isValidVariableIdentifier(const std::string & identifier);

    // Main functions analyzing every expression
    void analyzeAssignmentExpression(AssignmentExprAST *expr);
    void analyzeBinaryOperatorExpression(BinaryOperatorExprAST *expr);
    void analyzeBlockExpression(BlockExprAST *expr);
    void analyzeBooleanExpr(BooleanValueExprAST * boolean_expr);
    void analyzeBranchingIfExpression(BranchingIfExprAST *expr);
    void analyzeDeclarationExpression(DeclarationExprAST *expr);
    void analyzeStatement(AbstractExprAST *stmt);
    void analyzeFloatExpr(FloatValueExprAST *expr);
    void analyzeFunctionPrintValueExpression(FunctionPrintValueExprAST *expr);
    void analyzeIntegerExpression(IntegerValueExprAST *expr);
    void analyzeLoopWhileExpression(LoopWhileExprAST *expr);
    void analyzeStringExpr(StringValueExprAST *expr);
    void analyzeVariableReferenceValueExpression(VariableReferenceExprAST *expr);
};


#endif //THESIS_COMPILER_SEMANTICANALYZER_H
