/*
    This is a core class used for generation of LLVM IR code
    from Abstract Syntax Tree created by Parser.


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


#ifndef THESIS_COMPILER_LLVM_IR_GENERATOR_H
#define THESIS_COMPILER_LLVM_IR_GENERATOR_H

#include <iostream>
#include <map>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"

#include "../parser/expressions/keywordExpressions/BranchingIfExprAST.h"
#include "../parser/expressions/keywordExpressions/LoopWhileExprAST.h"
#include "../parser/expressions/operationExpressions/AssignmentExprAST.h"
#include "../parser/expressions/operationExpressions/BinaryOperatorExprAST.h"
#include "../parser/expressions/operationExpressions/DeclarationExprAST.h"
#include "../parser/expressions/operationExpressions/FunctionPrintValueExprAST.h"
#include "../parser/expressions/ProgramExprAST.h"
#include "../parser/expressions/utilsExpressions/BlockExprAST.h"
#include "../parser/expressions/valueExpressions/BooleanValueExprAST.h"
#include "../parser/expressions/valueExpressions/FloatValueExprAST.h"
#include "../parser/expressions/valueExpressions/IntegerValueExprAST.h"
#include "../parser/expressions/valueExpressions/StringValueExprAST.h"
#include "../parser/expressions/valueExpressions/VariableReferenceExprAST.h"

#include "../basicComponents/SymbolTable.h"
#include "../basicComponents/LanguageLiterals.h"

#include "../utilities/Logger.h"



class LLVM_IR_Generator {

public:
    explicit LLVM_IR_Generator(Logger& logger, ProgramExprAST &parsingTree, SymbolTable& symbolTable);

    void generateLLVM_IR();
    void printCodeGenerationLogs();
    llvm::Module& getModuleReference();

private:
    Logger& logger;

    std::unique_ptr<llvm::LLVMContext> Context;
    std::unique_ptr<llvm::IRBuilder<>> Builder;
    std::unique_ptr<llvm::Module> Module;

    ProgramExprAST &parsingTree;
    SymbolTable& symbolTable;

    static constexpr uint64_t defaultIntegerBitWidth = 64;
    static constexpr uint64_t defaultFloatBitWidth = 64;
    static constexpr uint64_t defaultBoolBitWidth = 1;

    llvm::Value* intFormatStr;
    llvm::Value* floatFormatStr;
    llvm::Value* stringFormatStr;
    llvm::Value* boolFormatStr;


    // Core LLVM block functions
    void declareBuiltinFunctions();
    void declarePrintFunction();

    void declareMainFunction();
    void terminateMainFunction();

    // utils functions
    std::string getPosition(const Token & token);

    llvm::Type* getLLVMTypeFromVariableType(VariableType type);
    llvm::Type* getLLVMTypeFromExpressionType(AbstractExprAST *expr);

    llvm::Value* getStringFormatFromLLVMValueType(llvm::Type* type);

    llvm::Value* generateBinopCalculationResult(BinaryOperatorType opType, llvm::Value *lhsValuePtr,
        llvm::Value *rhsValuePtr, llvm::Type *lhsLLVMType, llvm::Type* rhsLLVMType);

    // Main functions generating code for expressions
    llvm::Value* generateCodeForAssignmentExpression(AssignmentExprAST *expr);
    llvm::Value* generateCodeForBooleanExpression(BooleanValueExprAST* expr);
    llvm::Value* generateCodeForDeclarationExpression(DeclarationExprAST *expr);
    llvm::Value* generateCodeForStatement(AbstractExprAST *expr);
    llvm::Value* generateCodeForFloatExpression(FloatValueExprAST *expr);


    llvm::Value* generateCodeForFunctionPrintValueExpression(FunctionPrintValueExprAST *expr);
    llvm::Value* generateCodeForValueExpression(AbstractExprAST *expr);
    llvm::Value* generateCodeForIntegerExpression(IntegerValueExprAST *expr);
    llvm::Value* generateCodeForStringExpression(StringValueExprAST *expr);
    llvm::Value* generateCodeForVariableReferenceExpression(VariableReferenceExprAST *expr);
    llvm::Value* generateCodeForBinaryOperatorExpression(BinaryOperatorExprAST* expr);
    llvm::Value* generateCodeForBlockExpr(BlockExprAST* expr);
    llvm::Value* generateCodeForBranchingIfExpr(BranchingIfExprAST* expr);
    llvm::Value* generateCodeForLoopWhileExpression(LoopWhileExprAST* expr);
};

#endif //THESIS_COMPILER_LLVM_IR_GENERATOR_H
