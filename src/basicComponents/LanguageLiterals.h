/*
   This header file keeps definitions of all literals used in
   compiler, so that they are not hard coded in various parts of the code.


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


#ifndef THESIS_COMPILER_TOKENLITERALS_H
#define THESIS_COMPILER_TOKENLITERALS_H


//---LEXER-REGEX-PATTERNS--------------------------
#define LT_REGEX_PATTERN_IS_WHITESPACE R"(\s)"
#define LT_REGEX_PATTERN_IS_VALID_BEGINNING_OF_IDENTIFIER R"([a-zA-Z_])"
#define LT_REGEX_PATTERN_IS_VALID_IDENTIFIER_CHAR R"([a-zA-Z_0-9])"
#define LT_REGEX_PATTERN_IS_VALID_BEGINNING_OF_NUMBER R"([0-9\.])"
#define LT_REGEX_PATTERN_IS_VALID_NUMBER_CHAR R"([0-9\.])"

//---TOKEN-CHARS----------------------------------
#define LT_CHAR_FLOAT_SEPARATOR "."

#define LT_CHAR_OPERATOR_ADDITION "+"
#define LT_CHAR_OPERATOR_SUBTRACTION "-"
#define LT_CHAR_OPERATOR_MULTIPLICATION "*"
#define LT_CHAR_OPERATOR_DIVISION "/"
#define LT_CHAR_OPERATOR_DIVISION_REMAINDER "%"
#define LT_CHAR_OPERATOR_LESS_THAN "<"
#define LT_CHAR_OPERATOR_MORE_THAN ">"
#define LT_CHAR_OPERATOR_NOT "!"
#define LT_CHAR_OPERATOR_ASSIGNMENT "="

#define LT_CHAR_ROUND_OPENING_BRACKET "("
#define LT_CHAR_ROUND_CLOSING_BRACKET ")"
#define LT_CHAR_CURLY_OPENING_BRACKET "{"
#define LT_CHAR_CURLY_CLOSING_BRACKET "}"

#define LT_CHAR_TEXT_QUOTE "\""
#define LT_CHAR_END_OF_LINE ";"
#define LT_CHAR_COMMENT "#"
#define LT_CHAR_UNDERSCORE "_"

#define LT_CHAR_END_OF_FILE "\0"

//---TOKEN_NAMES---------------------------
#define LT_TOKEN_NAME_IDENTIFIER "Identifier"
#define LT_TOKEN_NAME_NUMERICAL_INTEGER_LITERAL "Integer Literal"
#define LT_TOKEN_NAME_NUMERICAL_FLOAT_LITERAL "Float Literal"
#define LT_TOKEN_NAME_STRING_LITERAL "String Literal"
#define LT_TOKEN_NAME_OPERATOR_ADDITION "Operator Addition"
#define LT_TOKEN_NAME_OPERATOR_SUBTRACTION "Operator Subtraction"
#define LT_TOKEN_NAME_OPERATOR_MULTIPLICATION "Operator Multiplication"
#define LT_TOKEN_NAME_OPERATOR_DIVISION "Operator Division"
#define LT_TOKEN_NAME_OPERATOR_DIVISION_REMAINDER "Operator Division Remainder"
#define LT_TOKEN_NAME_OPERATOR_ASSIGNMENT "Operator Assignment"
#define LT_TOKEN_NAME_OPERATOR_LESS_THAN "Operator Less Than"
#define LT_TOKEN_NAME_OPERATOR_MORE_THAN "Operator More Than"
#define LT_TOKEN_NAME_OPERATOR_EQUALS "Operator Equals"
#define LT_TOKEN_NAME_OPERATOR_NOT_EQUALS "Operator Not Equals"
#define LT_TOKEN_NAME_COMMENT "Comment"
#define LT_TOKEN_NAME_TEXT_VALUE_QUOTE "Text Value Quote"
#define LT_TOKEN_NAME_ROUND_OPENING_BRACKET "Opening Bracket"
#define LT_TOKEN_NAME_ROUND_CLOSING_BRACKET "Closing Bracket"
#define LT_TOKEN_NAME_SCOPE_OPENING_BRACKET "Scope Opening Bracket"
#define LT_TOKEN_NAME_SCOPE_CLOSING_BRACKET "Scope Closing Bracket"
#define LT_TOKEN_NAME_END_OF_LINE "End of Line"
#define LT_TOKEN_NAME_END_OF_FILE "End of File"

//---UNDEFINED_TOKENS__---------------------------
#define LT_UNDEFINED_STRING "_undefined"

#define LT_UNDEFINED_IDENTIFIER_STRING "_undefined - Token Identifier Literal"
#define LT_UNDEFINED_INTEGER_STRING "_undefined - Token Integer Literal"
#define LT_UNDEFINED_FLOAT_STRING "_undefined - Token Float Literal"
#define LT_UNDEFINED_STRING_STRING "_undefined - Token String Literal"


//---BINARY-OPERATORS----------------------------------
#define LT_OPERATOR_ADDITION "+"
#define LT_OPERATOR_SUBTRACTION "-"
#define LT_OPERATOR_MULTIPLICATION "*"
#define LT_OPERATOR_DIVISION "/"
#define LT_OPERATOR_DIVISION_REMAINDER "%"
#define LT_OPERATOR_LESS_THAN "<"
#define LT_OPERATOR_GREATER_THAN ">"
#define LT_OPERATOR_EQUAL LT_CHAR_OPERATOR_ASSIGNMENT LT_CHAR_OPERATOR_ASSIGNMENT
#define LT_OPERATOR_NOT_EQUAL LT_CHAR_OPERATOR_NOT LT_CHAR_OPERATOR_ASSIGNMENT

//---VARIABLE-TYPES----------------------------------
#define LT_VARIABLE_TYPE_INT "int"
#define LT_VARIABLE_TYPE_FLOAT "float"
#define LT_VARIABLE_TYPE_STRING "string"
#define LT_VARIABLE_TYPE_BOOL "bool"

#define LT_VARIABLE_BOOL_VALUE_TRUE "True"
#define LT_VARIABLE_BOOL_VALUE_FALSE "False"

//---KEYWORDS----------------------------------
#define LT_KEYWORD_LITERAL_INT LT_VARIABLE_TYPE_INT
#define LT_KEYWORD_LITERAL_FLOAT LT_VARIABLE_TYPE_FLOAT
#define LT_KEYWORD_LITERAL_STRING LT_VARIABLE_TYPE_STRING
#define LT_KEYWORD_LITERAL_BOOL LT_VARIABLE_TYPE_BOOL

#define LT_KEYWORD_LITERAL_TRUE LT_VARIABLE_BOOL_VALUE_TRUE
#define LT_KEYWORD_LITERAL_FALSE LT_VARIABLE_BOOL_VALUE_FALSE

#define LT_KEYWORD_LITERAL_PRINT "print"
#define LT_KEYWORD_LITERAL_WHILE "while"
#define LT_KEYWORD_LITERAL_IF "if"
#define LT_KEYWORD_LITERAL_ELSE "else"

//---LOGGER_MESSAGE_SEVERITY----------------------------------
#define LT_LOGGER_SEVERITY_LEVEL_ERROR "[ERROR]"
#define LT_LOGGER_SEVERITY_LEVEL_WARNING "[WARNING]"
#define LT_LOGGER_SEVERITY_LEVEL_INFO "[INFO]"

//---CODE-GENERATION----------------------------------
#define LT_LLVM_IR_COMPILER_NAME "Zyx Compiler"

#define LT_LLVM_IR_IF_CONDITION_VALUE_PTR_NAME "ifcond"

#define LT_LLVM_IR_PROGRAM_ENTRY_BASIC_BLOCK_NAME "entry"
#define LT_LLVM_IR_MAIN_FUNCTION_NAME "main"

#define LT_LLVM_IR_IF_THEN_BRANCH_BASIC_BLOCK_NAME "then"
#define LT_LLVM_IR_IF_ELSE_BRANCH_BASIC_BLOCK_NAME "else"
#define LT_LLVM_IR_IF_MERGE_BRANCH_BASIC_BLOCK_NAME "merge"

#define LT_LLVM_IR_LOOP_WHILE_CONDITION_BASIC_BLOCK_NAME "loopCondition"
#define LT_LLVM_IR_LOOP_WHILE_BODY_BASIC_BLOCK_NAME "loopBody"
#define LT_LLVM_IR_LOOP_WHILE_END_BASIC_BLOCK_NAME "loopEnd"

#define LT_LLVM_IR_BINOP_CAST_TO_FLOAT "lhsCastToFloat"
#define LT_LLVM_IR_BINOP_CAST_TO_FLOAT_RHS "rhsCastToFloat"
#define LT_LLVM_IR_BINOP_ADD_TMP "addtmp"
#define LT_LLVM_IR_BINOP_SUB_TMP "subtmp"
#define LT_LLVM_IR_BINOP_MUL_TMP "multmp"
#define LT_LLVM_IR_BINOP_DIV_TMP "divtmp"
#define LT_LLVM_IR_BINOP_DIV_REM_TMP "divremtmp"
#define LT_LLVM_IR_BINOP_CMP_LT "cmplt"
#define LT_LLVM_IR_BINOP_BOOL_CAST "boolCast"
#define LT_LLVM_IR_BINOP_CMP_GT "cmpgt"
#define LT_LLVM_IR_BINOP_CMP_EQ "cmpeq"
#define LT_LLVM_IR_BINOP_CMP_NE "cmpne"

#define LT_LLVM_FUNCTION_PRINTF_NAME "printf"

#define LT_LLVM_IR_FUNCTION_PRINTF_FORMAT_STRING_NAME_INT ".intFormatString"
#define LT_LLVM_IR_FUNCTION_PRINTF_FORMAT_STRING_NAME_FLOAT ".floatFormatString"
#define LT_LLVM_IR_FUNCTION_PRINTF_FORMAT_STRING_NAME_STRING ".strFormatString"
#define LT_LLVM_IR_FUNCTION_PRINTF_FORMAT_STRING_NAME_BOOL ".boolFormatString"

#define LT_LLVM_IR_FUNCTION_PRINTF_FORMAT_STRING_INT "%d\n"
#define LT_LLVM_IR_FUNCTION_PRINTF_FORMAT_STRING_FLOAT "%g\n"
#define LT_LLVM_IR_FUNCTION_PRINTF_FORMAT_STRING_STRING "%s\n"
#define LT_LLVM_IR_FUNCTION_PRINTF_FORMAT_STRING_BOOL "%d\n"


//---OTHER----------------------------------
#define LT_REQUIRED_SOURCE_CODE_FILE_EXTENSION ".zyx"
#define LT_LOGS_OUTPUT_FILE_EXTENSION ".dump_log"
#define LT_LOGGER_NAME_SUFFIX ""


#endif //THESIS_COMPILER_TOKENLITERALS_H



