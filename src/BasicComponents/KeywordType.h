/*
    This is a main class for defining and differentiating each keyword identifier string.
    During parsing Parser checks here if particular identifier is a keyword or not.


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


#ifndef THESIS_COMPILER_KEYWORDTYPE_H
#define THESIS_COMPILER_KEYWORDTYPE_H

#include <string>
#include <unordered_map>
#include <optional>

#include "KeywordType.h"
#include "LanguageLiterals.h"

enum class KeywordType
{
    // Variable Types
    INT = 0,
    FLOAT = 1,
    STRING = 2,
    BOOL = 3,

    // Bool values
    TRUE = 4,
    FALSE = 5,

    // Built-in functions
    PRINT = 6,

    // Structural navigation
    WHILE = 7,
    IF = 8,
    ELSE = 9,
};

namespace KeywordTypeUtils {
    inline const std::unordered_map<std::string, KeywordType> keywordMap = {
            {LT_KEYWORD_LITERAL_INT, KeywordType::INT},
            {LT_KEYWORD_LITERAL_FLOAT,  KeywordType::FLOAT},
            {LT_KEYWORD_LITERAL_STRING, KeywordType::STRING},
            {LT_KEYWORD_LITERAL_BOOL, KeywordType::BOOL},

            {LT_KEYWORD_LITERAL_TRUE, KeywordType::TRUE},
            {LT_KEYWORD_LITERAL_FALSE, KeywordType::FALSE},

            {LT_KEYWORD_LITERAL_PRINT, KeywordType::PRINT},

            {LT_KEYWORD_LITERAL_WHILE, KeywordType::WHILE},
            {LT_KEYWORD_LITERAL_IF, KeywordType::IF},
            {LT_KEYWORD_LITERAL_ELSE, KeywordType::ELSE},
    };


    inline std::optional<KeywordType> getKeywordType(const std::string &identifier) {
        auto it = keywordMap.find(identifier);
        if (it != keywordMap.end()) {
            return it->second;
        }
        return std::nullopt;
    }
}

#endif //THESIS_COMPILER_KEYWORDTYPE_H
