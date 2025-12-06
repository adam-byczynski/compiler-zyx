/*
    This is a source file used to keeping methods of Lexer class.
    They are mainly used to split incoming source code into Token class objects.


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


#include "Lexer.h"


Lexer::Lexer(Logger &logger, const std::string &sourceCode)
    : logger(logger),
      sourceCode(sourceCode)
{
    this->currentIndex = 0;
    this->column = 1;
    this->line = 1; // It's intuitive to count index and col starting from 0, but lines from 1
    this->currentChar = std::string(1, sourceCode[currentIndex]);
}

// FIXME TODO dodac wszystkie funkcje void, na bool by zwracaly wynik operacji i potem error check

void Lexer::tokenize() {
    while (currentIndex < sourceCode.size()) {
        // Whitespace - skipped
        if (isWhitespace(currentChar)) {
            handleWhitespaceChar();
        }

        // Identifier: [a-zA-Z][a-zA-Z0-9]*
        else if (isBeginningOfIdentifierChar(currentChar)) {
            handleIdentifierToken();
        }

        // Number: [0-9.]+
        else if (isNumericalChar(currentChar)) {
            handleNumberToken();
        }

        // Operator: [+ - * / = > < == !=]
        else if (isBinaryOperatorChar(currentChar)) {
            handleBinaryOperatorToken();
        }

        // Argument opening bracket: (
        else if (isArgumentOpeningBracketChar(currentChar)) {
            handleArgumentOpeningBracketChar();
        }

        // Argument closing bracket: )
        else if (isArgumentClosingBracketChar(currentChar)) {
            handleArgumentClosingBracketToken();
        }

        // Scope opening bracket: {
        else if (isScopeOpeningBracketChar(currentChar)) {
            handleScopeOpeningBracketToken();
        }

        // Scope closing bracket: }
        else if (isScopeClosingBracketChar(currentChar)) {
            handleScopeClosingBracketToken();
        }

        // String value: ".+", checks for first quote char
        else if (isBeginningOfStringChar(currentChar)) {
            handleStringToken();
        }

        // End of line char: semicolon
        else if (isEndOfLineChar(currentChar)) {
            handleEndOfLineToken();
        }
        // Comment char: #
        else if (isCommentChar(currentChar)) {
            handleCommentToken();
        }

        // Raises an error - char not supported
        else {
            handleUnsupportedToken();
        }
    }
    // CurrentIndex is at last char
    handleEndOfFileToken();

    checkIfTokenizationSucceeded();
}

void Lexer::advanceIndexing() {
    if (currentIndex < sourceCode.size()) {
        currentIndex++;
        currentChar = sourceCode[currentIndex];

        if (currentChar == "\n") {
            line++;
            column = 0;
        } else {
            column++;
        }
    } else {
        this->logger.logMessage(LogSeverity::Error,"No more chars left for tokenization. Lexer should have stopped at previous token.");
    }
}


std::vector<Token> *Lexer::getTokens() {
    return &this->tokens;
}


void Lexer::printSourceCode() {
    std::cout << this->sourceCode;
}


void Lexer::printAvailableTokens() {
    std::cout << "\n\n------------------------------\n"
              << "LEXER TOKENS: \n"
              << "------------------------------\n";
    std::cout << "Lexer found " << this->tokens.size() << " tokens." << std::endl;

    for (size_t index = 0; index < tokens.size(); ++index) {
        std::cout << index << ". ";
        tokens[index].print();
        if (tokens[index].type == TokenType::END_OF_LINE ||
            tokens[index].type == TokenType::SCOPE_OPENING_BRACKET ||
            tokens[index].type == TokenType::SCOPE_CLOSING_BRACKET) {
            std::cout << std::endl;
        }
    }
    std::cout << "------------------------------\n\n";
}


bool Lexer::isWhitespace(std::string str) {
    return std::regex_match(str, std::regex(LT_REGEX_PATTERN_IS_WHITESPACE));
}


void Lexer::handleWhitespaceChar() {
    advanceIndexing();
}


bool Lexer::isBeginningOfIdentifierChar(std::string str) {
    return std::regex_match(str, std::regex(LT_REGEX_PATTERN_IS_VALID_BEGINNING_OF_IDENTIFIER));
}

void Lexer::handleIdentifierToken() {
    std::string identifier;

    while (currentIndex < sourceCode.size() &&
            std::regex_match(currentChar, std::regex(LT_REGEX_PATTERN_IS_VALID_IDENTIFIER_CHAR))
            )
    {
        identifier += currentChar;
        advanceIndexing();
    }
    tokens.emplace_back(TokenType::IDENTIFIER,
                        TokenTypeUtils::getTokenName(TokenType::IDENTIFIER),
                        TokenTypeUtils::getTokenLiteral(TokenType::IDENTIFIER),
                        identifier,
                        line,
                        column - identifier.size()
    );
}


bool Lexer::isNumericalChar(std::string str) {
    return std::regex_match(str, std::regex(LT_REGEX_PATTERN_IS_VALID_BEGINNING_OF_NUMBER));
}


void Lexer::handleNumberToken() {
    TokenType tokenType;
    std::string tokenName;
    std::string tokenLiteral;
    std::string value;

    bool isFloat = false;

    while (currentIndex < sourceCode.size() &&
        std::regex_match(currentChar, std::regex(LT_REGEX_PATTERN_IS_VALID_NUMBER_CHAR)))
    {
        value += currentChar;
        if (this->currentChar == LT_CHAR_FLOAT_SEPARATOR) {
            isFloat = true;
        }
        advanceIndexing();
    }

    if (isFloat) {
        tokenType = TokenType::NUMERICAL_FLOAT_LITERAL;
        tokenName = TokenTypeUtils::getTokenName(TokenType::NUMERICAL_FLOAT_LITERAL);
        tokenLiteral = TokenTypeUtils::getTokenLiteral(TokenType::NUMERICAL_FLOAT_LITERAL);
    } else {
        tokenType = TokenType::NUMERICAL_INTEGER_LITERAL;
        tokenName = TokenTypeUtils::getTokenName(TokenType::NUMERICAL_INTEGER_LITERAL);
        tokenLiteral = TokenTypeUtils::getTokenLiteral(TokenType::NUMERICAL_INTEGER_LITERAL);
    }

    tokens.emplace_back(tokenType, tokenName, tokenLiteral, value, line, column - value.size());
}


bool Lexer::isBinaryOperatorChar(const std::string str) {
    return (str == LT_CHAR_OPERATOR_ASSIGNMENT ||
            str == LT_CHAR_OPERATOR_ADDITION ||
            str == LT_CHAR_OPERATOR_SUBTRACTION ||
            str == LT_CHAR_OPERATOR_MULTIPLICATION ||
            str == LT_CHAR_OPERATOR_DIVISION ||
            str == LT_OPERATOR_DIVISION_REMAINDER ||
            str == LT_CHAR_OPERATOR_LESS_THAN ||
            str == LT_CHAR_OPERATOR_MORE_THAN ||
            str == LT_CHAR_OPERATOR_NOT);
}


void Lexer::handleBinaryOperatorToken() {
    TokenType tokenType = TokenType::EMPTY;
    bool isCompoundOperator = false;

    if (currentChar == LT_CHAR_OPERATOR_ASSIGNMENT) {
        if(peekNextChar() == LT_CHAR_OPERATOR_ASSIGNMENT) {
                tokenType = TokenType::OPERATOR_EQUALS;
                isCompoundOperator = true;
            } else {
                tokenType = TokenType::OPERATOR_ASSIGNMENT;
            }
    } else if (currentChar == LT_CHAR_OPERATOR_ADDITION) {
        tokenType = TokenType::OPERATOR_ADDITION;
    } else if (currentChar == LT_CHAR_OPERATOR_SUBTRACTION) {
        tokenType = TokenType::OPERATOR_SUBTRACTION;
    } else if (currentChar == LT_CHAR_OPERATOR_MULTIPLICATION) {
        tokenType = TokenType::OPERATOR_MULTIPLICATION;
    } else if (currentChar == LT_CHAR_OPERATOR_DIVISION) {
        tokenType = TokenType::OPERATOR_DIVISION;
    } else if (currentChar == LT_CHAR_OPERATOR_DIVISION_REMAINDER) {
        tokenType = TokenType::OPERATOR_DIVISION_REMAINDER;
    } else if (currentChar == LT_CHAR_OPERATOR_LESS_THAN) {
        tokenType = TokenType::OPERATOR_LESS_THAN;
    } else if (currentChar == LT_CHAR_OPERATOR_MORE_THAN) {
        tokenType = TokenType::OPERATOR_MORE_THAN;
    } else if (currentChar == LT_CHAR_OPERATOR_NOT) {
        if(peekNextChar() == LT_CHAR_OPERATOR_ASSIGNMENT) {
            tokenType = TokenType::OPERATOR_NOT_EQUALS;
            isCompoundOperator = true;
        }
    } else {
        std::string errorMessage = "Unsupported operator: " +
            currentChar + " at line " +  std::to_string(line) + " column " +  std::to_string(column);
        this->logger.logMessage(LogSeverity::Error, errorMessage);
    }

    std::string operatorValue;
    if(isCompoundOperator) {
        operatorValue = currentChar + peekNextChar(); // Get two char value
    } else {
        operatorValue = currentChar;
    }

    if (tokenType == TokenType::EMPTY) {
        std::string errorMessage = "Unsupported token type: " +
            currentChar + " at line " +  std::to_string(line) + " column " +  std::to_string(column);
        this->logger.logMessage(LogSeverity::Error, errorMessage);
    }

    std::string tokenName = TokenTypeUtils::getTokenName(tokenType);
    std::string tokenLiteral = TokenTypeUtils::getTokenLiteral(tokenType);

    tokens.emplace_back(tokenType, tokenName, tokenLiteral, operatorValue, line, column);
    if(isCompoundOperator) {
        advanceIndexing(); // Skip first of the two characters
    }
    advanceIndexing();
}


bool Lexer::isBeginningOfStringChar(std::string str) {
    return str == LT_CHAR_TEXT_QUOTE;
}


void Lexer::handleStringToken() {
    tokens.emplace_back(TokenType::TEXT_VALUE_QUOTE,
                        TokenTypeUtils::getTokenName(TokenType::TEXT_VALUE_QUOTE),
                        TokenTypeUtils::getTokenLiteral(TokenType::TEXT_VALUE_QUOTE),
                        currentChar,
                        line,
                        column);
    advanceIndexing();

    std::string text;
    while (currentIndex < sourceCode.size() && currentChar != LT_CHAR_TEXT_QUOTE) {
        text += currentChar;
        advanceIndexing();
    }

    tokens.emplace_back(TokenType::STRING_LITERAL,
                        TokenTypeUtils::getTokenName(TokenType::STRING_LITERAL),
                        TokenTypeUtils::getTokenLiteral(TokenType::STRING_LITERAL),
                        text,
                        line,
                        column - text.size());

    tokens.emplace_back(TokenType::TEXT_VALUE_QUOTE,
                        TokenTypeUtils::getTokenName(TokenType::TEXT_VALUE_QUOTE),
                        TokenTypeUtils::getTokenLiteral(TokenType::TEXT_VALUE_QUOTE),
                        currentChar,
                        line,
                        column);
    advanceIndexing();
}


bool Lexer::isEndOfLineChar(std::string str) {
    return str == LT_CHAR_END_OF_LINE;
}


void Lexer::handleEndOfLineToken() {
    tokens.emplace_back(TokenType::END_OF_LINE,
                        TokenTypeUtils::getTokenName(TokenType::END_OF_LINE),
                        TokenTypeUtils::getTokenLiteral(TokenType::END_OF_LINE),
                        currentChar,
                        line,
                        column);
    advanceIndexing();
}


bool Lexer::isCommentChar(std::string str) {
    return str == LT_CHAR_COMMENT;
}


void Lexer::handleCommentToken() {
    while (currentIndex < sourceCode.size() && currentChar != "\n") {
        advanceIndexing();
    }
}


bool Lexer::isEndOfFileToken() {
    return currentIndex == (sourceCode.size() - 1);
}

void Lexer::handleEndOfFileToken() {
    tokens.emplace_back(TokenType::END_OF_FILE,
                        TokenTypeUtils::getTokenName(TokenType::END_OF_FILE),
                        TokenTypeUtils::getTokenLiteral(TokenType::END_OF_FILE),
                        LT_CHAR_END_OF_FILE,
                        line,
                        column);
}


bool Lexer::isArgumentOpeningBracketChar(std::string str) {
    return str == LT_CHAR_ROUND_OPENING_BRACKET;
}


void Lexer::handleArgumentOpeningBracketChar() {
    tokens.emplace_back(TokenType::ROUND_OPENING_BRACKET,
                        TokenTypeUtils::getTokenName(TokenType::ROUND_OPENING_BRACKET),
                        TokenTypeUtils::getTokenLiteral(TokenType::ROUND_OPENING_BRACKET),
                        currentChar,
                        line,
                        column);
    advanceIndexing();
}


bool Lexer::isArgumentClosingBracketChar(std::string str) {
    return str == LT_CHAR_ROUND_CLOSING_BRACKET;
}


void Lexer::handleArgumentClosingBracketToken() {
    tokens.emplace_back(TokenType::ROUND_CLOSING_BRACKET,
                        TokenTypeUtils::getTokenName(TokenType::ROUND_CLOSING_BRACKET),
                        TokenTypeUtils::getTokenLiteral(TokenType::ROUND_CLOSING_BRACKET),
                        currentChar,
                        line,
                        column);
    advanceIndexing();
}


bool Lexer::isScopeOpeningBracketChar(std::string str) {
    return str == LT_CHAR_CURLY_OPENING_BRACKET;
}


void Lexer::handleScopeOpeningBracketToken() {
    tokens.emplace_back(TokenType::SCOPE_OPENING_BRACKET,
                        TokenTypeUtils::getTokenName(TokenType::SCOPE_OPENING_BRACKET),
                        TokenTypeUtils::getTokenLiteral(TokenType::SCOPE_OPENING_BRACKET),
                        currentChar,
                        line,
                        column);
    advanceIndexing();
}

bool Lexer::isScopeClosingBracketChar(std::string str) {
    return str == LT_CHAR_CURLY_CLOSING_BRACKET;
}

void Lexer::handleScopeClosingBracketToken() {
    tokens.emplace_back(TokenType::SCOPE_CLOSING_BRACKET,
                        TokenTypeUtils::getTokenName(TokenType::SCOPE_CLOSING_BRACKET),
                        TokenTypeUtils::getTokenLiteral(TokenType::SCOPE_CLOSING_BRACKET),
                        currentChar,
                        line,
                        column);
    advanceIndexing();
}

void Lexer::handleUnsupportedToken() {
    std::string errorString = "Unsupported token: \"" + currentChar + "\" at line " +
            std::to_string(line) + ", column " + std::to_string(column);
    this->logger.logMessage(LogSeverity::Error, errorString);
}

std::string Lexer::peekNextChar() {
    if (currentIndex + 1 < sourceCode.length()) {
        return std::string(1, sourceCode[currentIndex + 1]);
    }
    return LT_CHAR_END_OF_FILE;
}

void Lexer::checkIfTokenizationSucceeded() {
    if (this->tokens.empty()) {
        this->logger.logMessage(LogSeverity::Error, "The file has not been yet tokenized, or the set of tokens is empty.");
    }
}
