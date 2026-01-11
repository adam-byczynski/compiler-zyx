/*
    Header file for LLVM_ExecutableGenerator class.


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


#ifndef THESIS_COMPILER_LLVMEXECUTABLEGENERATOR_H
#define THESIS_COMPILER_LLVMEXECUTABLEGENERATOR_H

#include <string>
#include <vector>
#include <system_error>
#include <iostream>
#include <filesystem>
#include <cstdlib>

#include "llvm/IR/Value.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/ADT/APInt.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/TargetParser//Host.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/FileSystem.h"

#include "../utilities/Logger.h"


class LLVM_ExecutableGenerator {
public:
    explicit LLVM_ExecutableGenerator(Logger& logger, llvm::Module& module);

    void generateObjectFile(const std::string &objectFileName);
    void generateExecutableFile(const std::string &objectFileName);

private:
    Logger& logger;
    
    llvm::Module *module;

    void emitObjectFile(const std::string &objectFileName);
    void emitLinkedExecutableFile(const std::string &exectuableFileName);
};


#endif //THESIS_COMPILER_LLVMEXECUTABLEGENERATOR_H
