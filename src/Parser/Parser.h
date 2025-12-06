/*  This is a main Parser class. It takes stream of Token objects from Lexer
    and generated from them the Abstract Syntax Tree, later used in semantic analysis
    and code generation.

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


#ifndef THESIS_COMPILER_PARSER_H
#define THESIS_COMPILER_PARSER_H

#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <memory>

#include "../BasicComponents/KeywordType.h"
#include "../BasicComponents/Token.h"

#include "Utils/OperatorPrecedence.h"

#include "Expressions/AbstractExprAST.h"
#include "Expressions/KeywordExpressions/BranchingIfExprAST.h"
#include "Expressions/KeywordExpressions/LoopWhileExprAST.h"
#include "Expressions/OperationExpressions/BinaryOperatorExprAST.h"
#include "Expressions/OperationExpressions/DeclarationExprAST.h"
#include "Expressions/ProgramExprAST.h"
#include "Expressions/UtilsExpressions/BlockExprAST.h"
#include "Expressions/ValueExpressions/BooleanValueExprAST.h"
#include "Expressions/ValueExpressions/FloatValueExprAST.h"
#include "Expressions/ValueExpressions/IntegerValueExprAST.h"
#include "Expressions/ValueExpressions/StringValueExprAST.h"

#include "Utils/ASTPrinter.h"

#include "../Utilities/Logger.h"


class Parser {
public:
    explicit Parser(Logger& logger, std::vector<Token> &tokens);
    void parse();
    void printParsingTree();

    ProgramExprAST& getParsingTreePointer();

private:
    Logger& logger;

    std::vector<Token>& tokens;
    size_t tokenIndex;
    Token* currentToken;
    std::unique_ptr<ProgramExprAST>  parsingTree;
    size_t currentExpressionID;

    void getNextToken();
    std::string getPosition();
    Token* peekTokenAhead(int peekDistance);
    bool isTokenSequenceCorrect(const std::vector<Token *>& tokens);


    // Functions for parsing different types of expressions
    std::unique_ptr<AbstractExprAST> parseArgumentValueExpression();
    std::unique_ptr<AbstractExprAST> parseAssignmentExpression();
    std::unique_ptr<AbstractExprAST> parseBinaryExpressionRHS(std::unique_ptr<AbstractExprAST> lhs, int minPrecedence);
    std::unique_ptr<AbstractExprAST> parseBlockExpression();
    std::unique_ptr<AbstractExprAST> parseBooleanExpression();
    std::unique_ptr<AbstractExprAST> parseIdentifierExpression();
    std::unique_ptr<AbstractExprAST> parseIfExpression();
    std::unique_ptr<AbstractExprAST> parseKeywordIdentifierExpression();
    std::unique_ptr<AbstractExprAST> parseLoopWhileExpression();
    std::unique_ptr<AbstractExprAST> parseNonKeywordIdentifierExpression();
    std::unique_ptr<AbstractExprAST> parseNumberFloatExpression();
    std::unique_ptr<AbstractExprAST> parseNumberIntegerExpression();
    std::unique_ptr<AbstractExprAST> parseOperatorBinaryExpression();
    std::unique_ptr<AbstractExprAST> parsePrimaryExpression();
    std::unique_ptr<AbstractExprAST> parsePrintFunctionExpression();
    std::unique_ptr<AbstractExprAST> parseStringExpression();
    std::unique_ptr<AbstractExprAST> parseValueExpression();
    std::unique_ptr<AbstractExprAST> parseVariableDeclaration();
    std::unique_ptr<AbstractExprAST> parseVariableReferenceExpression();


};

#endif //THESIS_COMPILER_PARSER_H
