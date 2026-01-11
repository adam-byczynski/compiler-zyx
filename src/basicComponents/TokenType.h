/*
    This is a file which contains two core parts:
    - TokenType enum class - which differentiates each token type. Used extensively in parsing.
    - TokenTypeUtils - various utils used to get token strings, checks etc.


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


#ifndef TOKENTYPE_H
#define TOKENTYPE_H

#include <string>
#include <stdexcept>

#include "LanguageLiterals.h"

enum class TokenType {
    EMPTY = 0,

    IDENTIFIER = 1,

    NUMERICAL_INTEGER_LITERAL = 2,
    NUMERICAL_FLOAT_LITERAL = 3,
    STRING_LITERAL = 4,

    TEXT_VALUE_QUOTE = 5,         // '"'

    OPERATOR_ADDITION = 6,        // '+'
    OPERATOR_SUBTRACTION = 7 ,     // '-'
    OPERATOR_MULTIPLICATION = 8,  // '*'
    OPERATOR_DIVISION = 9,        // '/'
    OPERATOR_DIVISION_REMAINDER = 23, // '%'
    OPERATOR_ASSIGNMENT = 10,      // '='
    OPERATOR_LESS_THAN = 11,       // '<'
    OPERATOR_MORE_THAN = 12,       // '>'
    OPERATOR_EQUALS = 13,          // "=="
    OPERATOR_NOT_EQUALS = 14,      // "!="

    COMMENT = 15,                  // '#'

    ROUND_OPENING_BRACKET = 17, // '('
    ROUND_CLOSING_BRACKET = 18, // ')'
    SCOPE_OPENING_BRACKET = 19,    // '{'
    SCOPE_CLOSING_BRACKET = 20,    // '}'
    END_OF_LINE = 21,              // ';'
    END_OF_FILE = 22,
};

namespace TokenTypeUtils
{
    inline std::string getTokenLiteral(TokenType type) {
        switch (type) {
        case TokenType::IDENTIFIER:
            return LT_UNDEFINED_IDENTIFIER_STRING;
        case TokenType::NUMERICAL_INTEGER_LITERAL:
            return LT_UNDEFINED_INTEGER_STRING;
        case TokenType::NUMERICAL_FLOAT_LITERAL:
            return LT_UNDEFINED_FLOAT_STRING;
        case TokenType::STRING_LITERAL:
            return LT_UNDEFINED_STRING_STRING;
        case TokenType::OPERATOR_ADDITION:
            return LT_OPERATOR_ADDITION;
        case TokenType::OPERATOR_SUBTRACTION:
            return LT_OPERATOR_SUBTRACTION;
        case TokenType::OPERATOR_MULTIPLICATION:
            return LT_OPERATOR_MULTIPLICATION;
        case TokenType::OPERATOR_DIVISION:
            return LT_OPERATOR_DIVISION;
        case TokenType::OPERATOR_DIVISION_REMAINDER:
            return LT_OPERATOR_DIVISION_REMAINDER;
        case TokenType::OPERATOR_ASSIGNMENT:
            return LT_CHAR_OPERATOR_ASSIGNMENT;
        case TokenType::OPERATOR_LESS_THAN:
            return LT_OPERATOR_LESS_THAN;
        case TokenType::OPERATOR_MORE_THAN:
            return LT_OPERATOR_GREATER_THAN;
        case TokenType::OPERATOR_EQUALS:
            return LT_OPERATOR_EQUAL;
        case TokenType::OPERATOR_NOT_EQUALS:
            return LT_OPERATOR_NOT_EQUAL;
        case TokenType::COMMENT:
            return LT_CHAR_COMMENT;
        case TokenType::TEXT_VALUE_QUOTE:
            return LT_CHAR_TEXT_QUOTE;
        case TokenType::ROUND_OPENING_BRACKET:
            return LT_CHAR_ROUND_OPENING_BRACKET;
        case TokenType::ROUND_CLOSING_BRACKET:
            return LT_CHAR_ROUND_CLOSING_BRACKET;
        case TokenType::SCOPE_OPENING_BRACKET:
            return LT_CHAR_CURLY_OPENING_BRACKET;
        case TokenType::SCOPE_CLOSING_BRACKET:
            return LT_CHAR_CURLY_CLOSING_BRACKET;
        case TokenType::END_OF_LINE:
            return LT_CHAR_END_OF_LINE;
        case TokenType::END_OF_FILE:
            return LT_UNDEFINED_STRING;
        default:
            throw std::runtime_error("ERROR: Getting literal of unsupported token type.");
        }
    }

    inline std::string getTokenName(TokenType type) {
        switch (type) {
        case TokenType::IDENTIFIER:
            return LT_TOKEN_NAME_IDENTIFIER;
        case TokenType::NUMERICAL_INTEGER_LITERAL:
            return LT_TOKEN_NAME_NUMERICAL_INTEGER_LITERAL;
        case TokenType::NUMERICAL_FLOAT_LITERAL:
            return LT_TOKEN_NAME_NUMERICAL_FLOAT_LITERAL;
        case TokenType::STRING_LITERAL:
            return LT_TOKEN_NAME_STRING_LITERAL;
        case TokenType::OPERATOR_ADDITION:
            return LT_TOKEN_NAME_OPERATOR_ADDITION;
        case TokenType::OPERATOR_SUBTRACTION:
            return LT_TOKEN_NAME_OPERATOR_SUBTRACTION;
        case TokenType::OPERATOR_MULTIPLICATION:
            return LT_TOKEN_NAME_OPERATOR_MULTIPLICATION;
        case TokenType::OPERATOR_DIVISION:
            return LT_TOKEN_NAME_OPERATOR_DIVISION;
        case TokenType::OPERATOR_DIVISION_REMAINDER:
            return LT_TOKEN_NAME_OPERATOR_DIVISION_REMAINDER;
        case TokenType::OPERATOR_ASSIGNMENT:
            return LT_TOKEN_NAME_OPERATOR_ASSIGNMENT;
        case TokenType::OPERATOR_LESS_THAN:
            return LT_TOKEN_NAME_OPERATOR_LESS_THAN;
        case TokenType::OPERATOR_MORE_THAN:
            return LT_TOKEN_NAME_OPERATOR_MORE_THAN;
        case TokenType::OPERATOR_EQUALS:
            return LT_TOKEN_NAME_OPERATOR_EQUALS;
        case TokenType::OPERATOR_NOT_EQUALS:
            return LT_TOKEN_NAME_OPERATOR_NOT_EQUALS;
        case TokenType::COMMENT:
            return LT_TOKEN_NAME_COMMENT;
        case TokenType::TEXT_VALUE_QUOTE:
            return LT_TOKEN_NAME_TEXT_VALUE_QUOTE;
        case TokenType::ROUND_OPENING_BRACKET:
            return LT_TOKEN_NAME_ROUND_OPENING_BRACKET;
        case TokenType::ROUND_CLOSING_BRACKET:
            return LT_TOKEN_NAME_ROUND_CLOSING_BRACKET;
        case TokenType::SCOPE_OPENING_BRACKET:
            return LT_TOKEN_NAME_SCOPE_OPENING_BRACKET;
        case TokenType::SCOPE_CLOSING_BRACKET:
            return LT_TOKEN_NAME_SCOPE_CLOSING_BRACKET;
        case TokenType::END_OF_LINE:
            return LT_TOKEN_NAME_END_OF_LINE;
        case TokenType::END_OF_FILE:
            return LT_TOKEN_NAME_END_OF_FILE;
        default:
            throw std::runtime_error("ERROR: Getting name of unsupported token type.");
        }
    }

    inline bool isOperatorTokenType(TokenType type) {
        switch (type) {
            case TokenType::OPERATOR_ADDITION:
            case TokenType::OPERATOR_SUBTRACTION:
            case TokenType::OPERATOR_MULTIPLICATION:
            case TokenType::OPERATOR_DIVISION:
            case TokenType::OPERATOR_DIVISION_REMAINDER:
            case TokenType::OPERATOR_LESS_THAN:
            case TokenType::OPERATOR_MORE_THAN:
            case TokenType::OPERATOR_EQUALS:
            case TokenType::OPERATOR_NOT_EQUALS:
                return true;
            default:
                return false;
        }
    }

    inline bool isAllowedTokenInValueExpression(TokenType type) {
        switch (type) {
            case TokenType::IDENTIFIER:
            case TokenType::NUMERICAL_INTEGER_LITERAL:
            case TokenType::NUMERICAL_FLOAT_LITERAL:
            case TokenType::STRING_LITERAL:
            case TokenType::ROUND_OPENING_BRACKET:
            case TokenType::ROUND_CLOSING_BRACKET:
            case TokenType::TEXT_VALUE_QUOTE:
                return true;
            default:
                return false;
        }
    }
}
#endif // TOKENTYPE_H