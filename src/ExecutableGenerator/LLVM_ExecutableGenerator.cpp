/*
    LLVM_ExecutableGenerator is a class that does two main things:
     - generation of binary .obj from LLVM IR generated in LLVM_IR_Generator
     - emission of final executable by linking .obj file using clang from path


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


#include "LLVM_ExecutableGenerator.h"


LLVM_ExecutableGenerator::LLVM_ExecutableGenerator(Logger &logger, llvm::Module &module) : logger(logger) {
    this->module = &module;
}

void LLVM_ExecutableGenerator::generateObjectFile(const std::string &outputFileName) {
    try {
        this->emitObjectFile(outputFileName);
    } catch (const std::exception &e) {
        std::string llvmErrorMessage = "Error: " + std::string(e.what());
        this->logger.logMessage(LogSeverity::Error, "An error occurred during emitting of object file: " + llvmErrorMessage);
    }
}

void LLVM_ExecutableGenerator::emitObjectFile(const std::string &objectFileName) {
    auto targetTriple = llvm::sys::getDefaultTargetTriple();
    this->module->setTargetTriple(targetTriple);

    std::string error;
    auto target = llvm::TargetRegistry::lookupTarget(targetTriple, error);
    if (!target) {
        this->logger.logMessage(
            LogSeverity::Error,
             "Failed to create target for emitting of object file: " + error);
    }

    auto CPU = "generic";
    auto features = "";

    llvm::TargetOptions opt;
    auto RM = std::optional<llvm::Reloc::Model>();
    auto targetMachine = target->createTargetMachine(targetTriple, CPU, features, opt, RM);

    this->module->setDataLayout(targetMachine->createDataLayout());

    std::error_code EC;
    std::string fullFileName = objectFileName + "_obj";
    llvm::raw_fd_ostream dest(fullFileName, EC, llvm::sys::fs::OF_None);
    if (EC) {
        this->logger.logMessage(
            LogSeverity::Error,
             "Could not open file: " + EC.message());
    }

    // Setup pass manager for optimizations and code generation
    llvm::legacy::PassManager passManager;
    llvm::CodeGenFileType fileType = llvm::CodeGenFileType::ObjectFile;
    if (targetMachine->addPassesToEmitFile(passManager, dest, nullptr, fileType)) {
        this->logger.logMessage(
            LogSeverity::Error,
             "Target machine cannot emit a file of this type");
    }

    passManager.run(*this->module);
    dest.flush();
}


void LLVM_ExecutableGenerator::generateExecutableFile(const std::string &objectFileName) {
    try {
        this->emitLinkedExecutableFile(objectFileName);
    } catch (const std::exception &e) {
        std::string llvmErrorMessage = "Error: " + std::string(e.what());
        this->logger.logMessage(LogSeverity::Error, "An error occurred during emitting of executable file: " + llvmErrorMessage);
    }
}

void LLVM_ExecutableGenerator::emitLinkedExecutableFile(const std::string &exectuableFileName) {
    if (std::system("which clang > /dev/null 2>&1") != 0) {
        this->logger.logMessage(
            LogSeverity::Error,
            "Clang is not installed or not found in PATH.");
    }

    std::filesystem::path objFile = exectuableFileName + "_obj";
    if (!std::filesystem::exists(objFile)) {
        this->logger.logMessage(
            LogSeverity::Error,
            "Could not link to executable. Expected " + exectuableFileName + "_obj file in the same directory.");
    }

    std::string command = "clang " + exectuableFileName + "_obj -o " + exectuableFileName + "_exec";
    int linkObjFileStatus = std::system(command.c_str());

    if (linkObjFileStatus != 0) {
        this->logger.logMessage(
            LogSeverity::Error,
            "Could not link object file " + exectuableFileName + "_obj.");
    }

    int removeObjStatus = std::remove(objFile.c_str());
    if (removeObjStatus != 0) {
        this->logger.logMessage(
            LogSeverity::Error, "Could not clean up Object file after linking.");
    }
}
