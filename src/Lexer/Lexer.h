/*
    This is a class for Lexer. It is used to generate vector of tokens from incoming string.


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


#ifndef THESIS_COMPILER_LEXER_H
#define THESIS_COMPILER_LEXER_H


#include <vector>
#include <cctype>
#include <stdexcept>
#include <string>
#include <iostream>
#include <regex>

#include "../BasicComponents/Token.h"
#include "../BasicComponents/TokenType.h"
#include "../BasicComponents/LanguageLiterals.h"
#include "../Utilities/Logger.h"

class Lexer {
public:
    explicit Lexer(Logger& logger, const std::string &sourceCode);
    std::vector<Token> *getTokens();
    void printSourceCode();
    void printAvailableTokens();
    void tokenize();

private:
    Logger& logger;

    std::string sourceCode;
    std::string currentChar;
    size_t currentIndex;
    int column;
    int line;

    std::vector<Token> tokens;

    void advanceIndexing();

    bool isWhitespace(std::string str);
    void handleWhitespaceChar();

    bool isBeginningOfIdentifierChar(std::string str);
    void handleIdentifierToken();

    bool isNumericalChar(std::string str);
    void handleNumberToken();

    bool isBinaryOperatorChar(std::string str);
    void handleBinaryOperatorToken();

    void handleStringToken();
    bool isBeginningOfStringChar(std::string str);

    bool isArgumentOpeningBracketChar(std::string str);
    void handleArgumentOpeningBracketChar();

    void handleArgumentClosingBracketToken();
    bool isArgumentClosingBracketChar(std::string str);

    bool isScopeOpeningBracketChar(std::string str);
    void handleScopeOpeningBracketToken();

    bool isScopeClosingBracketChar(std::string str);
    void handleScopeClosingBracketToken();

    bool isEndOfLineChar(std::string str);
    void handleEndOfLineToken();

    bool isCommentChar(std::string str);
    void handleCommentToken();

    bool isEndOfFileToken();
    void handleEndOfFileToken();

    void handleUnsupportedToken();
    std::string peekNextChar();
    void checkIfTokenizationSucceeded();
};

#endif //THESIS_COMPILER_LEXER_H
