/*
    This is source file for LLVM_IR_Generator class.
    It contains core methods related to generation of LLVM IR from Abstract Syntax Tree.


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


#include "LLVM_IR_Generator.h"


LLVM_IR_Generator::LLVM_IR_Generator(Logger& logger, ProgramExprAST &parsingTree, SymbolTable& symbolTable)
        : logger(logger), parsingTree(parsingTree), symbolTable(symbolTable) {

    this->Context = std::make_unique<llvm::LLVMContext>();
    this->Builder = std::make_unique<llvm::IRBuilder<>>(*Context);
    this->Module = std::make_unique<llvm::Module>(LT_LLVM_IR_COMPILER_NAME, *Context);

    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();
}

void LLVM_IR_Generator::printCodeGenerationLogs() {
    std::cout << "\n------------------------------\n"
              << "LLVM IR: \n"
              << "------------------------------\n";
    this->Module->print(llvm::errs(), nullptr);
    std::cout << "------------------------------\n\n";

}

llvm::Module& LLVM_IR_Generator::getModuleReference() {
    return *this->Module;
}

void LLVM_IR_Generator::declareMainFunction() {
    llvm::FunctionType *funcType = llvm::FunctionType::get(llvm::Type::getInt32Ty(*Context), false);
    llvm::Function *mainFunction = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, LT_LLVM_IR_MAIN_FUNCTION_NAME, *Module);

    llvm::BasicBlock *entryBlock = llvm::BasicBlock::Create(*Context, LT_LLVM_IR_PROGRAM_ENTRY_BASIC_BLOCK_NAME, mainFunction);
    Builder->SetInsertPoint(entryBlock);
}

void LLVM_IR_Generator::terminateMainFunction() {
    Builder->CreateRet(llvm::ConstantInt::get(llvm::Type::getInt32Ty(*Context), 0));
}

void LLVM_IR_Generator::declareBuiltinFunctions() {
    this->declareMainFunction();
    this->declarePrintFunction();
}

void LLVM_IR_Generator::declarePrintFunction() {
    this->Module->getOrInsertFunction(
        LT_LLVM_FUNCTION_PRINTF_NAME,
        llvm::FunctionType::get(
            // return type is integer because in c printf returns number of chars printed
            Builder->getInt32Ty(),
            // format arg
            Builder->getInt8Ty()->getPointerTo(),
            // variable number of arguments
            true));

    intFormatStr = Builder->CreateGlobalStringPtr(
        LT_LLVM_IR_FUNCTION_PRINTF_FORMAT_STRING_INT,
        LT_LLVM_IR_FUNCTION_PRINTF_FORMAT_STRING_NAME_INT);
    floatFormatStr = Builder->CreateGlobalStringPtr(
        LT_LLVM_IR_FUNCTION_PRINTF_FORMAT_STRING_FLOAT,
        LT_LLVM_IR_FUNCTION_PRINTF_FORMAT_STRING_NAME_FLOAT);
    stringFormatStr = Builder->CreateGlobalStringPtr(
        LT_LLVM_IR_FUNCTION_PRINTF_FORMAT_STRING_STRING,
        LT_LLVM_IR_FUNCTION_PRINTF_FORMAT_STRING_NAME_STRING);
    boolFormatStr = Builder->CreateGlobalStringPtr(
        LT_LLVM_IR_FUNCTION_PRINTF_FORMAT_STRING_BOOL,
        LT_LLVM_IR_FUNCTION_PRINTF_FORMAT_STRING_NAME_BOOL);
}

std::string LLVM_IR_Generator::getPosition(const Token &token) {
    return "[L:" + std::to_string(token.line) + ", C:" + std::to_string(token.column) + "]: ";
}

void LLVM_IR_Generator::generateLLVM_IR() {
    this->declareBuiltinFunctions();

    int counter = 1;
    for (const auto &statementTree : this->parsingTree.statements) {
        this->logger.logMessage(LogSeverity::Info, "Generating code for statement nr. " + std::to_string(counter));

        // generateCodeForStatement does not return anything here, so just execution
        generateCodeForStatement(statementTree.get());
        counter++;
    }

    this->terminateMainFunction();
}

llvm::Value* LLVM_IR_Generator::generateCodeForStatement(AbstractExprAST *expr) {
    if (!expr) {
        this->logger.logMessage(LogSeverity::Error,
            "Could not generated because the expression is null.");
    }

    if (auto declarationExpr = dynamic_cast<DeclarationExprAST *>(expr)) {
        generateCodeForDeclarationExpression(declarationExpr);
    } else if (auto assignmentExpr = dynamic_cast<AssignmentExprAST *>(expr)) {
        generateCodeForAssignmentExpression(assignmentExpr);
    } else if (auto functionPrintValueExpr = dynamic_cast<FunctionPrintValueExprAST *>(expr)) {
        generateCodeForFunctionPrintValueExpression(functionPrintValueExpr);
    } else if (auto whileExpr = dynamic_cast<LoopWhileExprAST *>(expr)) {
        generateCodeForLoopWhileExpression(whileExpr);
    } else if (auto branchingIfExpr = dynamic_cast<BranchingIfExprAST *>(expr)) {
        generateCodeForBranchingIfExpr(branchingIfExpr);
    } else if (auto blockExpr = dynamic_cast<BlockExprAST *>(expr)) {
        generateCodeForBlockExpr(blockExpr);
    } else {
        this->logger.logMessage(LogSeverity::Error,
            "Could not generate code for unknown expression type.");
    }
    return nullptr;
}

llvm::Value* LLVM_IR_Generator::generateCodeForFunctionPrintValueExpression(FunctionPrintValueExprAST *expr) {
    auto printArgumentExpression = expr->getArgument().get();

    llvm::Value* valueToBePrinted = generateCodeForValueExpression(printArgumentExpression);
    if (!valueToBePrinted) {
        this->logger.logMessage(LogSeverity::Error,
            getPosition(expr->getArgumentRefTok()) +
            "Could not generate code for value expression. Unsupported value expression.");
    }

    llvm::Value* formatStrInt = getStringFormatFromLLVMValueType(valueToBePrinted->getType());
    if (!formatStrInt) {
        this->logger.logMessage(LogSeverity::Error,
            getPosition(expr->getArgumentRefTok()) +
            "Could not generate string basing on expression type.");
    }

    // Default container for printf arguments -> formating and value
    std::vector<llvm::Value*> args {formatStrInt, valueToBePrinted};

    auto printFunc = this->Module->getFunction(LT_LLVM_FUNCTION_PRINTF_NAME);
    if (!printFunc) {
        this->logger.logMessage(LogSeverity::Error,
            getPosition(expr->getArgumentRefTok()) +
            "Could not generate code for printing because printing function has not been declared.");
    }

    return Builder->CreateCall(printFunc, args);
}

llvm::Value* LLVM_IR_Generator::generateCodeForValueExpression(AbstractExprAST *expr) {
    if (auto binopExpr = dynamic_cast<BinaryOperatorExprAST *>(expr)) {
        return generateCodeForBinaryOperatorExpression(binopExpr);
    } else if (auto integerExpr = dynamic_cast<IntegerValueExprAST *>(expr)) {
        return generateCodeForIntegerExpression(integerExpr);
    } else if (auto stringExpr = dynamic_cast<StringValueExprAST *>(expr)) {
        return generateCodeForStringExpression(stringExpr);
    } else if (auto floatExpr = dynamic_cast<FloatValueExprAST *>(expr)) {
        return generateCodeForFloatExpression(floatExpr);
    } else if (auto booleanExpr = dynamic_cast<BooleanValueExprAST *>(expr)) {
        return generateCodeForBooleanExpression(booleanExpr);
    } else if (auto variableReferenceValueExpr = dynamic_cast<VariableReferenceExprAST *>(expr)) {
        return generateCodeForVariableReferenceExpression(variableReferenceValueExpr);
    } else {
        return nullptr;
    }
}

llvm::Value* LLVM_IR_Generator::generateCodeForDeclarationExpression(DeclarationExprAST *expr) {
    llvm::Type *llvmVariableType = getLLVMTypeFromVariableType(expr->getVariableType());
    if(!llvmVariableType) {
        this->logger.logMessage(
            LogSeverity::Error,
            getPosition(expr->getVarNameRefTok()) +
            "Could not get LLVM Type from variable type for variable \"" + expr->getName() + "\".");
    }

    const std::string &variableName = expr->getName();

    llvm::Value *initValue = generateCodeForValueExpression(expr->getInitExpression().get());

    llvm::AllocaInst *alloca = Builder->CreateAlloca(llvmVariableType, nullptr, variableName);
    if (initValue) {
        Builder->CreateStore(initValue, alloca);
    }

    this->symbolTable.setVariableValuePointer(variableName, alloca);
    return alloca;
}

llvm::Value* LLVM_IR_Generator::generateCodeForIntegerExpression(IntegerValueExprAST *expr) {
    return llvm::ConstantInt::get(*Context, llvm::APInt(this->defaultIntegerBitWidth, expr->getValue(), true));
}

llvm::Value* LLVM_IR_Generator::generateCodeForFloatExpression(FloatValueExprAST *expr) {
    return llvm::ConstantFP::get(*Context, llvm::APFloat(expr->getValue()));
}

llvm::Value* LLVM_IR_Generator::generateCodeForStringExpression(StringValueExprAST *expr) {
    return this->Builder->CreateGlobalStringPtr(expr->getStringValue());
}

llvm::Value* LLVM_IR_Generator::generateCodeForBooleanExpression(BooleanValueExprAST *expr) {
    return llvm::ConstantInt::get(*Context, llvm::APInt(1, expr->getValue()));
}

llvm::Value* LLVM_IR_Generator::generateCodeForAssignmentExpression(AssignmentExprAST *expr) {
    const std::string& variableName = expr->getVariableName();

    // 1. Getting information about variable with name 'variableName' from Symbol Table
    auto symbolInfoOrNull = this->symbolTable.getVariableInfo(variableName);
    if(!symbolInfoOrNull.has_value()) {
        this->logger.logMessage(
            LogSeverity::Error,
            getPosition(expr->getVariableNameReferenceToken()) +
            "Could not get information from Symbol Table about variable \"" + variableName + "\". Probably this variable has not been declared yet.");
    }

    llvm::Value* oldValuePtr = symbolInfoOrNull.value()->variableValuePointer;
    if (!oldValuePtr) {
        this->logger.logMessage(
            LogSeverity::Error,
            getPosition(expr->getVariableNameReferenceToken()) +
            "Could not get previous value from Symbol Table for Variable \"" + variableName + "\". Probably this variable has not been declared yet.");
    }

    llvm::Value *newValuePtr = generateCodeForValueExpression(expr->getNewValue().get());
    if (!newValuePtr) {
        this->logger.logMessage(
            LogSeverity::Error,
            getPosition(expr->getVariableNameReferenceToken()) +
            "Could not generate code for new value assignment for variable \"" + variableName + "\".");
    }

    this->Builder->CreateStore(newValuePtr, oldValuePtr);

    return newValuePtr;
}

llvm::Value* LLVM_IR_Generator::generateCodeForVariableReferenceExpression(VariableReferenceExprAST *expr) {
    const std::string& variableName = expr->getReferencedVariableName();

    auto symbolInfoOrNull = this->symbolTable.getVariableInfo(variableName);
    if(!symbolInfoOrNull.has_value()) {
        this->logger.logMessage(
            LogSeverity::Error,
            getPosition(expr->getValueRefTok()) +
            "Could not get information from Symbol Table about variable \"" + variableName + "\". Probably this variable has not been declared yet.");
    }

    llvm::Value* variableValuePtr = symbolInfoOrNull.value()->variableValuePointer;
    if(!variableValuePtr){
        this->logger.logMessage(
            LogSeverity::Error,
            getPosition(expr->getValueRefTok()) +
            "Could not generate reference to variable \"" + variableName + "\", because it does not exist.");
    }

    llvm::Type* inferredVariableType = getLLVMTypeFromVariableType(symbolInfoOrNull.value()->variableType);
    if(!inferredVariableType) {
        this->logger.logMessage(
            LogSeverity::Error,
            getPosition(expr->getValueRefTok()) +
            "Could not get LLVM Type from variable type for variable reference \"" + expr->getReferencedVariableName() + "\".");
    }

    llvm::Value* loadedVariableValue = Builder->CreateLoad(
        inferredVariableType,
        variableValuePtr,
        variableName + "_loaded"
    );
    if(!loadedVariableValue){
        this->logger.logMessage(
            LogSeverity::Error,
            "Could not load value of referenced variable \"" + variableName + "\".");
    }

    return loadedVariableValue;
}


llvm::Value* LLVM_IR_Generator::generateCodeForBinaryOperatorExpression(BinaryOperatorExprAST *expr) {
    llvm::Value* lhsValuePtr = nullptr;
    if (auto* lhsBinOp = dynamic_cast<BinaryOperatorExprAST*>(expr->getLHS().get())) {
        lhsValuePtr = generateCodeForBinaryOperatorExpression(lhsBinOp);
    } else {
        lhsValuePtr = generateCodeForValueExpression(expr->getLHS().get());
    }

    if (!lhsValuePtr) {
        this->logger.logMessage(
            LogSeverity::Error,
            getPosition(expr->getLHSRefTok()) +
            "Could not generate code for the left-hand side of the binary operator.");
        return nullptr;
    }

    // Recursive generation for RHS
    llvm::Value* rhsValuePtr = nullptr;
    if (auto* rhsBinOp = dynamic_cast<BinaryOperatorExprAST*>(expr->getRHS().get())) {
        rhsValuePtr = generateCodeForBinaryOperatorExpression(rhsBinOp);
    } else {
        rhsValuePtr = generateCodeForValueExpression(expr->getRHS().get());
    }

    if (!rhsValuePtr) {
        this->logger.logMessage(
            LogSeverity::Error,
            getPosition(expr->getRHSRefTok()) +
            "Could not generate code for the right-hand side of the binary operator.");
    }

    llvm::Type* lhsLLVMType = lhsValuePtr->getType();
    llvm::Type* rhsLLVMType = rhsValuePtr->getType();

    if (!lhsLLVMType) {
        this->logger.logMessage(
            LogSeverity::Error,
            getPosition(expr->getLHSRefTok()) +
            "Could not determine the LLVM type for the left-hand side of the binary operator."
        );
    }

    if (!rhsLLVMType) {
        this->logger.logMessage(
            LogSeverity::Error,
            getPosition(expr->getRHSRefTok()) +
            "Could not determine the LLVM type for the right-hand side of the binary operator."
        );
    }

    llvm::Value* resultValuePtr = generateBinopCalculationResult(
        expr->getOperator(),
        lhsValuePtr,
        rhsValuePtr,
        lhsLLVMType,
        rhsLLVMType);

    if (!resultValuePtr) {
        this->logger.logMessage(
            LogSeverity::Error,
            getPosition(expr->getLHSRefTok()) +
            "Could not generate code for the binary operator expression.");
    }

    return resultValuePtr;
}


llvm::Value * LLVM_IR_Generator::generateBinopCalculationResult(BinaryOperatorType opType,
    llvm::Value *lhsValuePtr, llvm::Value *rhsValuePtr, llvm::Type *lhsLLVMType, llvm::Type *rhsLLVMType)
{
    // Determine the resulting type of the operation
    bool lhsIsFloat = lhsLLVMType->isDoubleTy();
    bool rhsIsFloat = rhsLLVMType->isDoubleTy();
    bool lhsIsInt = lhsLLVMType->isIntegerTy(defaultIntegerBitWidth);
    bool rhsIsInt = rhsLLVMType->isIntegerTy(defaultIntegerBitWidth);
    bool lhsIsBool = lhsLLVMType->isIntegerTy(defaultBoolBitWidth);
    bool rhsIsBool = rhsLLVMType->isIntegerTy(defaultBoolBitWidth);

    llvm::Value* lhsPtr = lhsValuePtr;
    llvm::Value* rhsPtr = rhsValuePtr;

    // Handle type casting
if (lhsIsFloat || rhsIsFloat) {
        if (!lhsIsFloat) {
            lhsPtr = this->Builder->CreateSIToFP(lhsPtr, llvm::Type::getDoubleTy(*Context), LT_LLVM_IR_BINOP_CAST_TO_FLOAT);
        }
        if (!rhsIsFloat) {
            rhsPtr = this->Builder->CreateSIToFP(rhsPtr, llvm::Type::getDoubleTy(*Context), LT_LLVM_IR_BINOP_CAST_TO_FLOAT_RHS);
        }
    }

    llvm::Value* result = nullptr;

    switch (opType) {
        case BinaryOperatorType::ADD:
            if (lhsIsFloat || rhsIsFloat) {
                result = this->Builder->CreateFAdd(lhsPtr, rhsPtr, LT_LLVM_IR_BINOP_ADD_TMP);
            } else {
                result = this->Builder->CreateAdd(lhsPtr, rhsPtr, LT_LLVM_IR_BINOP_ADD_TMP);
            }
            break;
        case BinaryOperatorType::SUBTRACT:
            if (lhsIsFloat || rhsIsFloat) {
                result = this->Builder->CreateFSub(lhsPtr, rhsPtr, LT_LLVM_IR_BINOP_SUB_TMP);
            } else {
                result = this->Builder->CreateSub(lhsPtr, rhsPtr, LT_LLVM_IR_BINOP_SUB_TMP);
            }
            break;
        case BinaryOperatorType::MULTIPLY:
            if (lhsIsFloat || rhsIsFloat) {
                result = this->Builder->CreateFMul(lhsPtr, rhsPtr, LT_LLVM_IR_BINOP_MUL_TMP);
            } else {
                result = this->Builder->CreateMul(lhsPtr, rhsPtr, LT_LLVM_IR_BINOP_MUL_TMP);
            }
            break;
        case BinaryOperatorType::DIVIDE:
            {
                // Compile time check for division by zero
                llvm::Constant* constRhs = llvm::dyn_cast<llvm::Constant>(rhsPtr);

                if (constRhs != nullptr) {
                    if (llvm::ConstantFP *constFP = llvm::dyn_cast<llvm::ConstantFP>(constRhs)) {
                        if (constFP->isZero()) {
                            this->logger.logMessage(
                                LogSeverity::Error,
                                "Cannot divide floats by zero."
                            );
                        }
                    } else if (llvm::ConstantInt *constInt = llvm::dyn_cast<llvm::ConstantInt>(constRhs)) {
                        if (constInt->isZero()) {
                            this->logger.logMessage(
                                LogSeverity::Error,
                                "Cannot divide ints by zero."
                            );
                        }
                    }
                }

                if (lhsIsFloat || rhsIsFloat) {
                    result = this->Builder->CreateFDiv(lhsPtr, rhsPtr, LT_LLVM_IR_BINOP_DIV_TMP);
                } else {
                    result = this->Builder->CreateSDiv(lhsPtr, rhsPtr, LT_LLVM_IR_BINOP_DIV_TMP);
                }
            }
            break;
        case BinaryOperatorType::DIVIDE_REMAINDER:
            {
                // Compile time check for division by zero
                llvm::Constant* constRhs = llvm::dyn_cast<llvm::Constant>(rhsPtr);

                if (constRhs != nullptr) {
                    if (llvm::ConstantFP *constFP = llvm::dyn_cast<llvm::ConstantFP>(constRhs)) {
                        if (constFP->isZero()) {
                            this->logger.logMessage(
                                LogSeverity::Error,
                                "Cannot divide with remainder floats by zero."
                            );
                        }
                    } else if (llvm::ConstantInt *constInt = llvm::dyn_cast<llvm::ConstantInt>(constRhs)) {
                        if (constInt->isZero()) {
                            this->logger.logMessage(
                                LogSeverity::Error,
                                "Cannot divide with remainder integers by zero."
                            );
                        }
                    }
                }

                if (lhsIsFloat || rhsIsFloat) {
                    result = this->Builder->CreateFRem(lhsPtr, rhsPtr, LT_LLVM_IR_BINOP_DIV_REM_TMP);
                } else {
                    result = this->Builder->CreateSRem(lhsPtr, rhsPtr, LT_LLVM_IR_BINOP_DIV_REM_TMP);
                }
            }
            break;
        case BinaryOperatorType::LESS_THAN:
            if (lhsIsFloat || rhsIsFloat) {
                result = this->Builder->CreateFCmpULT(lhsPtr, rhsPtr, LT_LLVM_IR_BINOP_CMP_LT);
            } else {
                result = this->Builder->CreateICmpSLT(lhsPtr, rhsPtr, LT_LLVM_IR_BINOP_CMP_LT);
            }
            result = this->Builder->CreateZExt(result, llvm::Type::getInt1Ty(*Context), LT_LLVM_IR_BINOP_BOOL_CAST);
            break;
        case BinaryOperatorType::GREATER_THAN:
            if (lhsIsFloat || rhsIsFloat) {
                result = this->Builder->CreateFCmpUGT(lhsPtr, rhsPtr, LT_LLVM_IR_BINOP_CMP_GT);
            } else {
                result = this->Builder->CreateICmpSGT(lhsPtr, rhsPtr, LT_LLVM_IR_BINOP_CMP_GT);
            }
            result = this->Builder->CreateZExt(result, llvm::Type::getInt1Ty(*Context), LT_LLVM_IR_BINOP_BOOL_CAST);
            break;
        case BinaryOperatorType::EQUAL:
            if (lhsIsFloat || rhsIsFloat) {
                result = this->Builder->CreateFCmpUEQ(lhsPtr, rhsPtr, LT_LLVM_IR_BINOP_CMP_EQ);
            } else {
                result = this->Builder->CreateICmpEQ(lhsPtr, rhsPtr, LT_LLVM_IR_BINOP_CMP_EQ);
            }
            result = this->Builder->CreateZExt(result, llvm::Type::getInt1Ty(*Context), LT_LLVM_IR_BINOP_BOOL_CAST);
            break;
        case BinaryOperatorType::NOT_EQUAL:
            if (lhsIsFloat || rhsIsFloat) {
                result = this->Builder->CreateFCmpUNE(lhsPtr, rhsPtr, LT_LLVM_IR_BINOP_CMP_NE);
            } else {
                result = this->Builder->CreateICmpNE(lhsPtr, rhsPtr, LT_LLVM_IR_BINOP_CMP_NE);
            }
            result = this->Builder->CreateZExt(result, llvm::Type::getInt1Ty(*Context), LT_LLVM_IR_BINOP_BOOL_CAST);
            break;

        default:
            return nullptr;
    }

    return result;


}


llvm::Value* LLVM_IR_Generator::generateCodeForBlockExpr(BlockExprAST *expr) {
    if (!expr) {
        this->logger.logMessage(
            LogSeverity::Error,
            getPosition(expr->getBlockExprsRefTok()) +
            "Could not generate code for Block Expr because it's null.");
        return nullptr;
    }

    llvm::BasicBlock* currentBlock = Builder->GetInsertBlock();

    // Iterate over all expressions in the block and generate their IR
    for (const auto& expr : expr->getExpressions()) {
        generateCodeForStatement(expr.get());
    }

    // No value is returned from the block itself, just the control flow and scope management
    return nullptr;
}


llvm::Value* LLVM_IR_Generator::generateCodeForBranchingIfExpr(BranchingIfExprAST *expr) {
    if (!expr) {
        this->logger.logMessage(
            LogSeverity::Error,
            "Could not generate code because the expression is null.");
        return nullptr;
    }

    // Generate the condition value.
    llvm::Value *conditionValuePtr = this->generateCodeForValueExpression(expr->getCondition().get());
    if (!conditionValuePtr) {
        this->logger.logMessage(
            LogSeverity::Error,
            "Could not generate code for condition expression.");
        return nullptr;
    }

    // Checking if condition value is boolean -> not equal to 1-bit int 0
    conditionValuePtr = Builder->CreateICmpNE(
        conditionValuePtr, llvm::ConstantInt::get(llvm::Type::getInt1Ty(*Context), 0), LT_LLVM_IR_IF_CONDITION_VALUE_PTR_NAME);

    llvm::Function *TheFunction = Builder->GetInsertBlock()->getParent();

    // Create blocks for the 'then', 'else', and merge.
    llvm::BasicBlock *thenBranchBB = llvm::BasicBlock::Create(*Context, LT_LLVM_IR_IF_THEN_BRANCH_BASIC_BLOCK_NAME, TheFunction);
    llvm::BasicBlock *elseBranchBB = llvm::BasicBlock::Create(*Context, LT_LLVM_IR_IF_ELSE_BRANCH_BASIC_BLOCK_NAME);
    llvm::BasicBlock *mergeResultBB = llvm::BasicBlock::Create(*Context, LT_LLVM_IR_IF_MERGE_BRANCH_BASIC_BLOCK_NAME);

    // Conditional branch to 'then' or 'else'.
    Builder->CreateCondBr(conditionValuePtr, thenBranchBB, elseBranchBB);

    // Generate code for the 'then' block (which is always a BlockExprAST).
    Builder->SetInsertPoint(thenBranchBB);
    generateCodeForStatement(expr->getThenBranchExpr().get());

    Builder->CreateBr(mergeResultBB);  // Jump to the merge block after 'then' block.

    // Generate code for the 'else' block (which is also always a BlockExprAST).
    TheFunction->insert(TheFunction->end(), elseBranchBB);
    Builder->SetInsertPoint(elseBranchBB);
    if(expr->getElseBranchExpr() != nullptr) {
        generateCodeForStatement(expr->getElseBranchExpr().get());
    }
    Builder->CreateBr(mergeResultBB);  // Jump to the merge block after 'else' block.

    // Emit the merge block.
    TheFunction->insert(TheFunction->end(), mergeResultBB);
    Builder->SetInsertPoint(mergeResultBB);

    // If statement does not produce value on its own so return nullptr
    return nullptr;
}


llvm::Value* LLVM_IR_Generator::generateCodeForLoopWhileExpression(LoopWhileExprAST *expr) {
    if (!expr) {
        this->logger.logMessage(
            LogSeverity::Error,
            getPosition(expr->getConditionRefTok()) +
            "Could not generate code for null While expression.");
    }

    llvm::Function *TheFunction = Builder->GetInsertBlock()->getParent();
    if (!TheFunction) {
        this->logger.logMessage(
            LogSeverity::Error,
            getPosition(expr->getConditionRefTok()) +
            "Could not find parent block of While loop.");
    }

    // Create basic blocks for loop structure.
    llvm::BasicBlock *conditionBB = llvm::BasicBlock::Create(*Context, LT_LLVM_IR_LOOP_WHILE_CONDITION_BASIC_BLOCK_NAME, TheFunction);
    llvm::BasicBlock *bodyBB = llvm::BasicBlock::Create(*Context, LT_LLVM_IR_LOOP_WHILE_BODY_BASIC_BLOCK_NAME);
    llvm::BasicBlock *endBB = llvm::BasicBlock::Create(*Context, LT_LLVM_IR_LOOP_WHILE_END_BASIC_BLOCK_NAME);

    // Jump to the condition block to start the loop.
    Builder->CreateBr(conditionBB);
    Builder->SetInsertPoint(conditionBB);

    // Generating value for condition expression
    llvm::Value *conditionValuePtr = generateCodeForValueExpression(expr->getCondition().get());
    if (!conditionValuePtr) {
        this->logger.logMessage(
            LogSeverity::Error,
            getPosition(expr->getConditionRefTok()) +
            "Could not generate code for condition of While loop.");
    }

    // Checking if condition value is boolean -> not equal to 1-bit int 0
    conditionValuePtr = Builder->CreateICmpNE(
        conditionValuePtr, llvm::ConstantInt::get(llvm::Type::getInt1Ty(*Context), 0), LT_LLVM_IR_LOOP_WHILE_CONDITION_BASIC_BLOCK_NAME);


    // ---CORE-LOOP----------------------------
    // If condition is true, jump to body block, else jump to the end
    Builder->CreateCondBr(conditionValuePtr, bodyBB, endBB);

    // It is true, so generate body of the loop.
    TheFunction->insert(TheFunction->end(), bodyBB);
    Builder->SetInsertPoint(bodyBB);
    generateCodeForStatement(expr->getBody().get());

    // After generating body, jump back to the condition block for re-evaluation.
    Builder->CreateBr(conditionBB);

    // At end, so emitting the final code.
    TheFunction->insert(TheFunction->end(), endBB);
    Builder->SetInsertPoint(endBB);

    // While statement does not produce value on its own so return nullptr
    return nullptr;
}


llvm::Value * LLVM_IR_Generator::getStringFormatFromLLVMValueType(llvm::Type *type) {
    if (type->isIntegerTy()) {
        return intFormatStr;
    }
    if (type->isDoubleTy()) {
        return floatFormatStr;
    }
    if (type->isPointerTy()) {
        return stringFormatStr;
    }
    if (type->isIntegerTy(1)) {
        return boolFormatStr;
    }
    return nullptr;
}


llvm::Type* LLVM_IR_Generator::getLLVMTypeFromExpressionType(AbstractExprAST *expr) {
    if (dynamic_cast<IntegerValueExprAST *>(expr)) {
        return llvm::Type::getInt64Ty(*Context);
    }
    if (dynamic_cast<StringValueExprAST *>(expr)) {
        return llvm::Type::getDoubleTy(*Context);
    }
    if (dynamic_cast<FloatValueExprAST *>(expr)) {
        return llvm::Type::getInt8Ty(*Context)->getPointerTo();
    }
    if (dynamic_cast<BooleanValueExprAST *>(expr)) {
        return llvm::Type::getInt1Ty(*Context);
    }
    if (dynamic_cast<BinaryOperatorExprAST *>(expr)) {
        return nullptr;
    }
    return nullptr;
}

llvm::Type* LLVM_IR_Generator::getLLVMTypeFromVariableType(VariableType type) {
    switch (type) {
        case VariableType::INT:
            return llvm::Type::getInt64Ty(*Context);
        case VariableType::FLOAT:
            return llvm::Type::getDoubleTy(*Context);
        case VariableType::STRING:
            return llvm::Type::getInt8Ty(*Context)->getPointerTo();
        case VariableType::BOOL:
            return llvm::Type::getInt1Ty(*Context);
        default:
            return nullptr;
    }
}
