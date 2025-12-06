/* This is a source file for Parser class.
   It contains methods for parsing every type of expression of supported grammar.
   The final output of Parser here is its class member - parsingTree.

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


#include "Parser.h"


Parser::Parser(Logger& logger, std::vector<Token> &tokens) :
    logger(logger),
    tokens(tokens),
    tokenIndex(0),
    currentToken(&tokens[tokenIndex]),
    parsingTree(nullptr),
    currentExpressionID(1) // Starting at 1 for clarity
    {}


void Parser::parse() {
    auto program = std::make_unique<ProgramExprAST>();

    while (tokenIndex < tokens.size()) {
        switch (currentToken->type) {
            case TokenType::END_OF_FILE:
                this->parsingTree = std::move(program);
                return;
            default: {
                auto expressionNode = this->parsePrimaryExpression();
                if (expressionNode) {
                    program->addStatement(std::move(expressionNode));
                }
            }
        }
    }
}


void Parser::getNextToken() {
    if (this->tokenIndex < this->tokens.size()) {
        this->tokenIndex++;
        this->currentToken = &tokens[tokenIndex];

    } else {
        this->logger.logMessage(LogSeverity::Error, getPosition() + "No more tokens for parsing left. Parser should have stopped at previous EOF token.");
    }
}


std::unique_ptr<AbstractExprAST> Parser::parsePrimaryExpression() {
    switch (currentToken->type) {
        case TokenType::IDENTIFIER:
            return parseIdentifierExpression();
        default:
            std::string errorMessage = getPosition() + "Unexpected token in primary expression. Expected identifier."
                                        + currentToken->value;
            this->logger.logMessage(LogSeverity::Error, errorMessage);
    }
}


std::unique_ptr<AbstractExprAST> Parser::parseIdentifierExpression() {
    std::string identifierValue = currentToken->value;
    std::optional<KeywordType> keywordOrNull = KeywordTypeUtils::getKeywordType(identifierValue);

    if (keywordOrNull.has_value()) {
        return parseKeywordIdentifierExpression();
    } else {
        return parseNonKeywordIdentifierExpression();
    }
}


std::unique_ptr<AbstractExprAST> Parser::parseVariableDeclaration() {
    Token* variableNameRefTok = currentToken;
    VariableScope variableScope = VariableScope::GLOBAL;

    // First token will be type of variable
    std::optional<VariableType> typeOrNull = VariableTypeUtils::getVariableTypeFromString(currentToken->value);
    if(!typeOrNull.has_value()) {
        this->logger.logMessage(LogSeverity::Error,  getPosition() + "Unsupported variable type: " + currentToken->value);
    }
    VariableType variableType = typeOrNull.value();

    getNextToken();
    if (currentToken->type != TokenType::IDENTIFIER) {
        this->logger.logMessage(LogSeverity::Error,  getPosition() + "Expected identifier after type in variable declaration.");
    }

    std::string variableName = currentToken->value;

    getNextToken();
    if (currentToken->type != TokenType::OPERATOR_ASSIGNMENT) {
        this->logger.logMessage(LogSeverity::Error,  getPosition() + "Expected assignment sign '=' after variable name in variable declaration.");
    }

    getNextToken();
    auto variableValue = parseArgumentValueExpression();

    if (currentToken->type != TokenType::END_OF_LINE) {
        this->logger.logMessage(LogSeverity::Error,  getPosition() + "Expected ';' at the end of declaration.");
    }

    getNextToken(); // Consume current semicolon toke

    return std::make_unique<DeclarationExprAST>(
        currentExpressionID++,
        variableType,
        variableName,
        *variableNameRefTok,
        variableScope,
        std::move(variableValue));
}


void Parser::printParsingTree() {
    if (parsingTree) {
        ASTPrinter printer;
        printer.printAST(parsingTree);
    } else {
        this->logger.logMessage(LogSeverity::Error,  getPosition() + "No parse tree available. Make sure to call parse() first.");
    }
}

std::unique_ptr<AbstractExprAST> Parser::parseNumberIntegerExpression() {
    int intValue;

    try {
        intValue = std::stoi(tokens[tokenIndex].value);
    } catch (std::out_of_range &e) {
        this->logger.logMessage(LogSeverity::Error,  getPosition() + "Int value is out of range.");
    }
    Token* intValueRefTok = currentToken;

    getNextToken();

    return std::make_unique<IntegerValueExprAST>(currentExpressionID++, intValue, *intValueRefTok);
}


std::unique_ptr<AbstractExprAST> Parser::parseStringExpression() {
    getNextToken(); // Skip first quote sign
    if (currentToken->type != TokenType::STRING_LITERAL) {
        this->logger.logMessage(LogSeverity::Error, getPosition() + "Expected string literal after quote lexeme.");
    }

    std::string stringValue = currentToken->value;
    Token* stringValueRefTok = currentToken;

    getNextToken(); // Skipping string value
    getNextToken(); // Skipping second quote sign
    return std::make_unique<StringValueExprAST>(currentExpressionID++, stringValue, *stringValueRefTok);
}


std::unique_ptr<AbstractExprAST> Parser::parseNumberFloatExpression() {
    double floatVal;

    try {
        floatVal = std::stod(tokens[tokenIndex].value);
    } catch (std::out_of_range &e) {
        this->logger.logMessage(LogSeverity::Error,  getPosition() + "Float value is out of range.");
    }

    Token* floatValRefTok = currentToken;
    getNextToken();

    return std::make_unique<FloatValueExprAST>(currentExpressionID++, floatVal, *floatValRefTok);
}


std::unique_ptr<AbstractExprAST> Parser::parseBooleanExpression() {
    Token* boolValTokRef = currentToken;
    std::string booleanValueString = currentToken->value;
    bool booleanValue;

    if(booleanValueString == LT_KEYWORD_LITERAL_TRUE) {
        booleanValue = true;
    } else if(booleanValueString == LT_KEYWORD_LITERAL_FALSE) {
        booleanValue = false;
    } else {
        this->logger.logMessage(LogSeverity::Error, getPosition() + "Expected boolean value literal. Got: " + booleanValueString);
    }
    getNextToken();
    return std::make_unique<BooleanValueExprAST>(currentExpressionID++, booleanValue,  *boolValTokRef);
}


ProgramExprAST &Parser::getParsingTreePointer() {
    return *parsingTree;
}


std::unique_ptr<AbstractExprAST> Parser::parseKeywordIdentifierExpression() {
    std::string identifierValue = currentToken->value;

    switch (KeywordType identifierKeyword = KeywordTypeUtils::getKeywordType(identifierValue).value()) {
        case KeywordType::INT:
        case KeywordType::FLOAT:
        case KeywordType::STRING:
        case KeywordType::BOOL:
            return parseVariableDeclaration();

        case KeywordType::PRINT:
            return parsePrintFunctionExpression();

        case KeywordType::TRUE:
        case KeywordType::FALSE:
            return parseBooleanExpression();

        case KeywordType::WHILE:
            return parseLoopWhileExpression();

        case KeywordType::IF:
            return parseIfExpression();

        default:
            this->logger.logMessage(LogSeverity::Error, getPosition() + "Expected keyword. Found not supported identifier: " + identifierValue);
    }
}


std::string Parser::getPosition() {
    return "[L:" + std::to_string(currentToken->line) +
            ", C:" + std::to_string(currentToken->column) + "]: ";
}


std::unique_ptr<AbstractExprAST> Parser::parseVariableReferenceExpression() {
    std::string referredVariableName = currentToken->value;
    Token* refVarNameRefTok = currentToken;
    VariableScope scope = VariableScope::GLOBAL;

    getNextToken();

    return std::make_unique<VariableReferenceExprAST>(currentExpressionID++, referredVariableName, *refVarNameRefTok, scope);
}


std::unique_ptr<AbstractExprAST> Parser::parseValueExpression() {
    switch(currentToken->type) {
        case TokenType::IDENTIFIER:
            return parseIdentifierExpression();
        case TokenType::NUMERICAL_INTEGER_LITERAL:
            return parseNumberIntegerExpression();
        case TokenType::NUMERICAL_FLOAT_LITERAL:
            return parseNumberFloatExpression();
        case TokenType::TEXT_VALUE_QUOTE:
            return parseStringExpression();
        default:
            this->logger.logMessage(LogSeverity::Error, getPosition() + "The token is not a supported value expression.");
    }
}


std::unique_ptr<AbstractExprAST> Parser::parseAssignmentExpression() {
    std::string identifierValue = currentToken->value;
    Token* identifierValueRefTok = currentToken;
    VariableScope scope = VariableScope::GLOBAL;

    getNextToken();
    // Expect '='
    if (currentToken->type != TokenType::OPERATOR_ASSIGNMENT) {
        this->logger.logMessage(LogSeverity::Error, getPosition() + "Expected assignment sign '=' after variable name variable assignment. Variables must be initialized on declaration.");
    }

    getNextToken(); // Expect value expression until end of line
    auto variableValue = parseArgumentValueExpression();

    // Expect ';'
    if (currentToken->type != TokenType::END_OF_LINE) {
        this->logger.logMessage(LogSeverity::Error, getPosition() + "Expected ';' at the end of assignment. Each statement must end with end of line symbol.");
    }

    getNextToken(); // Consume current semicolon token

    return std::make_unique<AssignmentExprAST>(currentExpressionID++, identifierValue, *identifierValueRefTok, std::move(variableValue), scope);
}


std::unique_ptr<AbstractExprAST> Parser::parsePrintFunctionExpression() {
    this->getNextToken(); // We are skipping "print" identifier token
    this->getNextToken(); // We are skipping "(" token

    Token* argsRefTok = currentToken;
    auto printArgument = parseArgumentValueExpression();

    this->getNextToken(); // We are skipping ")" token
    if (currentToken->type != TokenType::END_OF_LINE) {
        this->logger.logMessage(LogSeverity::Error, getPosition() + "Expected ';' at the end of print statement");
    }
    getNextToken(); // Consume current semicolon toke

    return std::make_unique<FunctionPrintValueExprAST>(currentExpressionID++, std::move(printArgument), *argsRefTok);
}


std::unique_ptr<AbstractExprAST> Parser::parseNonKeywordIdentifierExpression() {
    switch (peekTokenAhead(1)->type) {
        case TokenType::OPERATOR_ASSIGNMENT:
            return parseAssignmentExpression();
        default:
            return parseVariableReferenceExpression();
    }
}

Token* Parser::peekTokenAhead(int peekDistance) {
    if(tokenIndex + peekDistance < tokens.size()) {
        return &tokens[tokenIndex + peekDistance];
    } else {
        this->logger.logMessage(LogSeverity::Error, getPosition() + "Expected more tokens after non-identifier keyword.");
    }
}


std::unique_ptr<AbstractExprAST> Parser::parseArgumentValueExpression() {
    // Check out all tokens until ';' or until ')' and next token being '{' as this would be whole value expression.
    // If there is single token, decide whether it is variable reference or immediate value and parse appropriately.
    // If there are more tokens, we are expecting binary operation so parseOperatorBinaryExpression().

    // Utils functions which help with clarity during initial checks of arguments

    // This one checks for moment when there are two tokens after each other: ')' '{'
    // This one checks for moment when there are two tokens after each other: ')' '{'
    // which indicates end of arguments in if and while
    auto isNotClosingBracketAtTheEndOfValueExpr = [this](int index) {
        return peekTokenAhead(index)->type != TokenType::ROUND_CLOSING_BRACKET &&
            (peekTokenAhead(index + 1)->type != TokenType::SCOPE_OPENING_BRACKET ||
            peekTokenAhead(index + 1)->type != TokenType::END_OF_LINE);
    };

    // This functions check for end of line token ';', which indicates end of expression in assignment and declarations
    auto isNotEndOfLineToken = [this](int index) {
        return peekTokenAhead(index)->type != TokenType::END_OF_LINE;
    };


    std::vector<Token*> tokensInStatement;
    int peekDistance = 0;
    int bracketsDifference = 0;

    while(isNotEndOfLineToken(peekDistance) && isNotClosingBracketAtTheEndOfValueExpr(peekDistance)) {
        tokensInStatement.push_back(peekTokenAhead(peekDistance));

        if(peekTokenAhead(peekDistance)->type == TokenType::ROUND_OPENING_BRACKET) {
            bracketsDifference++;
        }
        if(peekTokenAhead(peekDistance)->type == TokenType::ROUND_CLOSING_BRACKET) {
            bracketsDifference--;
        }
        peekDistance++;
    }


    // Additional checks: for mismatch in number of opening and closing brackets
    {
        if(bracketsDifference != 0) {
            this->logger.logMessage(LogSeverity::Error, getPosition() + "In the arguments there is different number of opening and closing brackets.");
        }

        // If first and last tokens in arguments are values, and not operators
        if(!TokenTypeUtils::isAllowedTokenInValueExpression(tokensInStatement.front()->type) ||
            !TokenTypeUtils::isAllowedTokenInValueExpression(tokensInStatement.back()->type))
        {
            this->logger.logMessage(LogSeverity::Error, getPosition() + "Incorrect syntax. First or last token in binary expression is an operator instead of value.");
        }

        // If in sequence of tokens, values and operators are interweaving with each other
        // to check for sth like (2+1-*5 3*) which would be otherwise accepted
        if(isTokenSequenceCorrect(tokensInStatement)) {
            this->logger.logMessage(LogSeverity::Error, getPosition() + "The sequence of tokens is incorrect. Probably there are two consecutive numbers or operators next to each other.");
        }
    }


    // Having that, finally we wrap up this function
    // If content is empty -> no arguments provided so error
    if (tokensInStatement.empty()) {
        this->logger.logMessage(LogSeverity::Error, getPosition() + "No arguments have been provided.");
    }

    // If there is single argument(string argument which unfortunately consists of 3 tokens) -> parse single value expression
    else if (tokensInStatement.size() == 1) {
        return parseValueExpression();
    }

    // Else we know we have more than one value -> parse binary operator expression
    else {
        return parseOperatorBinaryExpression();
    }
}


std::unique_ptr<AbstractExprAST> Parser::parseOperatorBinaryExpression() {
    std::unique_ptr<AbstractExprAST> lhs = parseValueExpression();

    if (!lhs) {
        this->logger.logMessage(LogSeverity::Error, getPosition() + "Failed to parse left-hand side expression.");
    }

    return parseBinaryExpressionRHS(std::move(lhs), OperatorPrecedence::minPriority);
}


std::unique_ptr<AbstractExprAST> Parser::parseBinaryExpressionRHS(std::unique_ptr<AbstractExprAST> lhs, int minPrecedence) {
    Token* lhsRefTok = currentToken;

    while (true) {

        // Basing on preparations, current token is expected to be an operator
        // If current token is not an operator, parsing is complete
        if (!TokenTypeUtils::isOperatorTokenType(currentToken->type)) {
            break;
        }

        // Get precedence of current token which is an operator
        std::optional<int> currentOpPrecOrNull = OperatorPrecedence::getOperatorPrecedence(currentToken->value);
        if(!currentOpPrecOrNull.has_value()) {
            this->logger.logMessage(LogSeverity::Error, getPosition() + "The operator " + currentToken->value + " does not have available precedence level.");
        }
        int currentOpPrec = currentOpPrecOrNull.value();

        // If precedence of the current operator is lower than the minPrecedence, break out
        if (currentOpPrec < minPrecedence) {
            break;
        }

        // Get the Binary Operator Type of current token - which is still an operator
        std::optional<BinaryOperatorType> operatorTypeOrNull = BinaryOperatorTypeUtils::getBinaryOperatorType(currentToken->value);
        if (!operatorTypeOrNull.has_value()) {
            this->logger.logMessage(LogSeverity::Error, getPosition() + "Could not get BinaryOperatorType of operator: " + currentToken->value);
        }
        BinaryOperatorType currentOperatorType = operatorTypeOrNull.value();

        // Consume the operator token, next is second operand
        getNextToken();

        // We are at second value in binop, so parse it
        Token* rhsRefTok = currentToken;
        auto rhs = parseValueExpression();

        // If current token is ';' or ')' we know we are at the end of value expression no more recursion
        if (currentToken->type != TokenType::END_OF_LINE && currentToken->type != TokenType::ROUND_CLOSING_BRACKET) {

            // We are at next operator, that needs to be compared with previous one
            // Get precedence of next operator
            std::optional<int> nextOpPrecOrNull = OperatorPrecedence::getOperatorPrecedence(currentToken->value);
            if(!nextOpPrecOrNull.has_value()) {
                this->logger.logMessage(LogSeverity::Error, getPosition() + "The operator " + currentToken->value + " does not have available precedence level.");
            }
            int nextOpPrec = nextOpPrecOrNull.value();

            // If the next token is an operator with higher or equal precedence, parse it as part of the current binary expression
            if (rhs &&
                TokenTypeUtils::isOperatorTokenType(currentToken->type) &&
                nextOpPrec >= currentOpPrec)
            {
                rhs = parseBinaryExpressionRHS(std::move(rhs), nextOpPrec);
            }
        }


        // If not more recursion combine lhs and rhs using the current operator
        lhs = std::make_unique<BinaryOperatorExprAST>(currentExpressionID++, std::move(lhs), std::move(rhs), *lhsRefTok, *rhsRefTok, currentOperatorType);
    }

    return lhs;
}


bool Parser::isTokenSequenceCorrect(const std::vector<Token*>& tokens) {
    // In every correct arguments expression, first token can't be an operator,
    // so setting previous token initially as any operator(here addition) will work as expected assuring that
    // first token is any value(int,float, string, identifier)
    TokenType previousType = TokenType::OPERATOR_ADDITION;

    for (const auto& token : tokens) {
        bool isPreviousOperator = TokenTypeUtils::isOperatorTokenType(previousType);
        bool isCurrentOperator = TokenTypeUtils::isOperatorTokenType(token->type);

        bool isPreviousValue = TokenTypeUtils::isAllowedTokenInValueExpression(previousType);
        bool isCurrentValue = TokenTypeUtils::isAllowedTokenInValueExpression(token->type);

        // Consecutive operators or values are not correct in the grammar
        if ((isCurrentOperator xor isPreviousOperator) && (isCurrentValue xor isPreviousValue)) {
            return false;
        }
        previousType = token->type;
    }
    return true;
}


std::unique_ptr<AbstractExprAST> Parser::parseIfExpression() {
    getNextToken(); // Consume 'if' token

    // Expect '('
    if (currentToken->type != TokenType::ROUND_OPENING_BRACKET) {
        this->logger.logMessage(LogSeverity::Error, getPosition() + "Expected '(' after If identifier.");
    }
    getNextToken(); // Consume '('

    // Saving token reference for condition to be passed later in expr. It will be similar with Then and Else blocks
    Token* condTokRef = currentToken;

    // Parse condition expression
    auto condition = parseArgumentValueExpression();
    if (!condition) {
        this->logger.logMessage(LogSeverity::Error, getPosition() + "Condition in If expression is null or empty.");
    }

    // Expect ')'
    if (currentToken->type != TokenType::ROUND_CLOSING_BRACKET) {
        this->logger.logMessage(LogSeverity::Error, getPosition() + "Expected ')' after arguments in If expression.");
    }

    getNextToken(); // Consume ')'

    // Expect '{'
    if (currentToken->type != TokenType::SCOPE_OPENING_BRACKET) {
        this->logger.logMessage(LogSeverity::Error, getPosition() + "Expected '{' after condition part in If expression.");
    }

    // Save tok ref for Then branch block
    Token* thenTokRef = currentToken;

    // Parse block body of If statement
    auto ifBranchBody = parseBlockExpression();

    if (!ifBranchBody) {
        this->logger.logMessage(LogSeverity::Error, getPosition() + "Body part of If expression is null or empty.");
    }

    // Using unique_ptr for elseTokRef is a workaround, with normal pointer there was very weird error. Feel free to check.
    std::unique_ptr<Token> elseTokRef;
    std::unique_ptr<AbstractExprAST> elseBranchBody = nullptr;

    // Check next token is an identifier and equal to Keyword 'else'
    auto keywordIdentifierOrNull = KeywordTypeUtils::getKeywordType(currentToken->value);
    bool isNextTokenAnElseIdentifier = false;

    if (keywordIdentifierOrNull.has_value()) {
        isNextTokenAnElseIdentifier = currentToken->type == TokenType::IDENTIFIER &&
            keywordIdentifierOrNull.value() == KeywordType::ELSE;
    }

    if (isNextTokenAnElseIdentifier) {
        // Save tok ref for Else branch block
        elseTokRef = std::make_unique<Token> (*currentToken);

        getNextToken(); // Consume 'else' token
        // Expect '{'
        if (currentToken->type != TokenType::SCOPE_OPENING_BRACKET) {
            this->logger.logMessage(LogSeverity::Error, getPosition() + "Expected '{' after 'else' in If expression.");
        }

        elseBranchBody = parseBlockExpression();

        if (!elseBranchBody) {
            this->logger.logMessage(LogSeverity::Error, getPosition() + "Body part of Else section in If expression is null or empty.");
        }
    }

    return std::make_unique<BranchingIfExprAST>(
        currentExpressionID++,
        std::move(condition),
        std::move(ifBranchBody),
        std::move(elseBranchBody),
        *condTokRef,
        *thenTokRef,
        *elseTokRef);
}


std::unique_ptr<AbstractExprAST> Parser::parseLoopWhileExpression() {
    getNextToken(); // Consume 'while' token
    // Expect '('
    if (currentToken->type != TokenType::ROUND_OPENING_BRACKET) {
        this->logger.logMessage(LogSeverity::Error, getPosition() + "Expected '(' after While identifier.");
    }
    getNextToken(); // Consume '('


    // Parse condition
    Token* conditionRefTok = currentToken;
    auto condition = parseArgumentValueExpression();

    if (!condition) {
        this->logger.logMessage(LogSeverity::Error, getPosition() + "Condition in While loop is null or empty.");
    }


    // Expect ')'
    if (currentToken->type != TokenType::ROUND_CLOSING_BRACKET) {
        this->logger.logMessage(LogSeverity::Error, getPosition() + "Expected ')' after arguments in While loop.");
    }
    getNextToken(); // Consume ')'


    // Expect '{'
    if (currentToken->type != TokenType::SCOPE_OPENING_BRACKET) {
        this->logger.logMessage(LogSeverity::Error, getPosition() + "Expected '{' after condition part in While expression.");
    }


    // Parse body (which should be a block or single expression)
    Token* bodyRefTok = currentToken;
    auto body = parseBlockExpression();
    if (!condition) {
        this->logger.logMessage(LogSeverity::Error, getPosition() + "Body of While loop is null or empty.");
    }

    return std::make_unique<LoopWhileExprAST>(currentExpressionID++, std::move(condition), std::move(body), *conditionRefTok, *bodyRefTok);
}


std::unique_ptr<AbstractExprAST> Parser::parseBlockExpression() {
    getNextToken(); // Consume scope opening token '{'

    Token* blockRefTok = currentToken;
    std::vector<std::unique_ptr<AbstractExprAST>> expressions;

    // Parse expressions until we hit closing brace
    while (currentToken->type != TokenType::SCOPE_CLOSING_BRACKET) {
        if (currentToken->type == TokenType::END_OF_FILE) {
            logger.logMessage(LogSeverity::Error, getPosition() + "Unexpected end of file inside nested scope block.");
        }

        // Parsing next expression in the block
        auto expr = parsePrimaryExpression();
        if (!expr) {
            logger.logMessage(LogSeverity::Error, getPosition() + "Nested expression in scope block is null or empty.");
        }

        expressions.push_back(std::move(expr));

        // Handle semicolons if your language requires them
        if (currentToken->type == TokenType::END_OF_LINE) {
            getNextToken(); // Consume ';'
        }
    }

    // Expect '}'
    if (currentToken->type != TokenType::SCOPE_CLOSING_BRACKET) {
        this->logger.logMessage(LogSeverity::Error, getPosition() + "Expected '}' at the end of scope block.");
    }
    getNextToken(); // Consume '}'


    return std::make_unique<BlockExprAST>(currentExpressionID++, std::move(expressions), *blockRefTok);
}
