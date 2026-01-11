/* This is a source file which keeps definitions of methods of Compiler class.
   The main method is runCompilation(), where the whole compilation process is conducted.
   Apart from that it handles parsing of command line input and other utils stuff.

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


#include "Compiler.h"

#include "executableGenerator/LLVM_ExecutableGenerator.h"
#include "IOHandling/SourceFile.h"
#include "IRCodeGenerator/LLVM_IR_Generator.h"
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "semanticAnalyzer/SemanticAnalyzer.h"

#include <string>

Compiler::Compiler(int argc, char *argv[]) {
    this->argCounter = argc;
    for (int i = 0; i < argc; ++i) {
        this->argValues.emplace_back(argv[i]);
    }

    this->parseCommandLineArguments();
    this->printCompilationOptions();

    logger.setLoggerName(this->outputFileName + LT_LOGGER_NAME_SUFFIX);
    logger.setInfoModeFlag(this->flagInfo);
};


void Compiler::parseCommandLineArguments() {
    // Start from 1 to skip the compiler program name
    for (int i = 1; i < this->argCounter; ++i) {
        std::string arg = this->argValues[i];

        // Parse long flags
        if (arg.starts_with("--")) {
            if (arg == "--help") {
                this->flagHelpInfo = true;
            } else if (arg == "--verbose") {
                this->flagVerbose = true;
            } else if (arg == "--info") {
                this->flagInfo = true;
            } else if (arg == "--no-link") {
                this->flagLink = false;
            } else if (arg == "--dump-logs") {
                this->flagDumpLogs = true;
            } else if (arg == "--output") {
                if (i + 1 < this->argCounter) {
                    this->outputFileName = this->argValues[++i];
                } else {
                    this->logger.logMessage(LogSeverity::Error, "flag --output requires an argument.");
                }
            } else {
                this->logger.logMessage(LogSeverity::Error, "Unknown option: " + arg);
            }

        // Parse short flags
        } else if (arg.starts_with("-")) {
            for (size_t j = 1; j < arg.length(); ++j) {
                char shortFlag = arg[j];

                switch (shortFlag) {
                    case 'h':
                        this->flagHelpInfo = true;
                        break;

                    case 'v':
                        this->flagVerbose = true;
                        break;

                    case 'i':
                        this->flagInfo = true;
                        break;

                    case 'n':
                        this->flagLink = false;
                        break;

                    case 'd':
                        this->flagDumpLogs = true;
                        break;

                    case 'o':
                        // -o requires a separate argument
                        if (i + 1 < this->argCounter) {
                            this->outputFileName = this->argValues[++i];
                        } else {
                            this->logger.logMessage(LogSeverity::Error, "-o requires an argument.");
                        }
                        break;

                    default:
                        this->logger.logMessage(LogSeverity::Error, "Unknown option: " + std::to_string(shortFlag)
                        );
                }
            }

        } else {
            this->sourceFileName = this->argValues[i];
        }
    }

    if (this->argCounter < 2 || this->flagHelpInfo) {
        logger.logMessage(LogSeverity::Error,
           "Basic usage: zyx [optional-arguments] <source-file>"
           "\n"
           "\nOptional arguments:"
           "\n  --dump-logs, -d          Dump compilation logs to file"
           "\n  --help, -h               Display this help message"
           "\n  --info, -i               Enables printing standard logs to terminal."
           "\n  --output, -o <file>      Specify output executable name"
           "\n  --no-link, -n            By default compiler emits Mach-o executable file and uses Clang linked from $PATH. "
           "\n                           Use this flag to disable automatic linking and get object files."
           "\n  --verbose, -v            Enable verbose logging, both to terminal and log file."
        );
    }
}


void Compiler::runCompilation() {

    // ---READING-INPUT_FILE------------------------------------------------------
    logger.logMessage(LogSeverity::Info, "Starting compilation process...");

    SourceFile sourceFile(this->logger, this->sourceFileName);

    logger.logMessage(LogSeverity::Info, "File with source code has been opened.");


    // ---LEXING------------------------------------------------------
    logger.logMessage(LogSeverity::Info, "Starting lexical analysis...");

    Lexer lexer(this->logger, *sourceFile.getContent());
    lexer.tokenize();
    if (this->flagVerbose) {
        lexer.printAvailableTokens();
    }

    logger.logMessage(LogSeverity::Info, "Lexical analysis has finished.");


    // ---PARSING------------------------------------------------------
    logger.logMessage(LogSeverity::Info, "Starting parsing...");

    Parser parser(this->logger, *lexer.getTokens());
    parser.parse();

    if (this->flagVerbose) {
        parser.printParsingTree();
    }

    logger.logMessage(LogSeverity::Info, "Parsing has finished.");


    // ---CREATING-SYMBOL-TABLE----------------------------------------
    SymbolTable symbolTable;


    // ---SEMANTIC-ANALYSIS------------------------------------------------------
    logger.logMessage(LogSeverity::Info, "Semantic analysis has started...");

    SemanticAnalyzer semanticAnalyzer(this->logger, parser.getParsingTreePointer(), symbolTable);
    semanticAnalyzer.performAnalysis();

    logger.logMessage(LogSeverity::Info, "Semantic analysis has finished.");


    //---CODE-GENERATION------------------------------------------------------
    logger.logMessage(LogSeverity::Info, "Code generation has started...");

    LLVM_IR_Generator codeGenerator(this->logger, parser.getParsingTreePointer(), symbolTable);
    codeGenerator.generateLLVM_IR();

    if (this->flagVerbose) {
        codeGenerator.printCodeGenerationLogs();
    }

    logger.logMessage(LogSeverity::Info, "Code generation has finished.");


    //---EMITTING-OBJECT_FILE------------------------------------------------------
    logger.logMessage(LogSeverity::Info, "Starting emitting object file...");

    LLVM_ExecutableGenerator executableGenerator(this->logger, codeGenerator.getModuleReference());
    executableGenerator.generateObjectFile(this->outputFileName);

    if (this->flagVerbose) {
        symbolTable.printSymbolTable();
    }

    logger.logMessage(LogSeverity::Info, "Object file has been emitted.");


    //---EMITTING-EXECUTABLE_FILE------------------------------------------------------
    if (this->flagLink) {
        logger.logMessage(LogSeverity::Info,"Starting emitting executable file...");

        executableGenerator.generateExecutableFile(this->outputFileName);

        logger.logMessage(LogSeverity::Info,"Executable file has been emitted.");
    }

    logger.logMessage(LogSeverity::Info, "Compilation process has finished.");



    //---DUMPING-COMPILATION-INFO------------------------------------------------
    if (this->flagDumpLogs) {
        logger.logMessage(LogSeverity::Info, "Saving compilation logs to file.");
        this->logger.dumpLogs();
    }
}


void Compiler::printCompilationOptions() {
    if (!std::filesystem::exists(this->sourceFileName)) {
        logger.logMessage(LogSeverity::Error, "Provided source file does not exist: " + this->sourceFileName);
    } else {
        logger.logMessage(LogSeverity::Info, "Source file: " + sourceFileName);
    }

    if (this->outputFileName.empty()) {
        this->outputFileName = std::filesystem::path(this->sourceFileName).replace_extension().string();
        logger.logMessage(LogSeverity::Info, "No output name specified. Source file name will be used: " + this->outputFileName);
    } else {
        logger.logMessage(LogSeverity::Info, "Output executable name: " + this->outputFileName);
    }

    if (this->flagInfo) {
        logger.logMessage(LogSeverity::Info, "Info mode enabled. Standard logs will be printed to console.");
    } else {
        logger.logMessage(LogSeverity::Info, "Info mode disabled. No logs will be printed to console.");
    }

    if (this->flagLink) {
        logger.logMessage(LogSeverity::Info, "Linking enabled. The object file will be linked to executable.");
    } else {
        logger.logMessage(LogSeverity::Info, "Linking disabled. The executable file will not be generated.");
    }

    if (this->flagDumpLogs) {
        logger.logMessage(LogSeverity::Info, "Dumping logs enabled. The logs from compilation process will be dumped to file.");
    } else {
        logger.logMessage(LogSeverity::Info, "Dump logs disabled. Compilation logs will not be dumped to file.");
    }

    if (this->flagVerbose) {
        logger.logMessage(LogSeverity::Info, "Verbose mode enabled. More log info will be printed.");
    } else {
        logger.logMessage(LogSeverity::Info, "Verbose mode disabled.");
    }
}
