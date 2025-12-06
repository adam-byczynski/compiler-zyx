/* This is a source file which keeps definitions of methods of Compiler class.
   The main method is runCompilation(), where the whole compilation process is conducted.
   Apart from that it handles parsing of command line input and other utils stuff.

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


#include "Compiler.h"

#include "ExecutableGenerator/LLVM_ExecutableGenerator.h"
#include "IOHandling/SourceFile.h"
#include "IRCodeGenerator/LLVM_IR_Generator.h"
#include "Lexer/Lexer.h"
#include "Parser/Parser.h"
#include "SemanticAnalyzer/SemanticAnalyzer.h"


Compiler::Compiler(int argc, char *argv[]) {
    this->argCounter = argc;
    for (int i = 0; i < argc; ++i) {
        this->argValues.emplace_back(argv[i]);
    }

    this->parseCommandLineArguments();
    this->printCompilationOptions();

    this->logger.setLoggerName(this->outputFileName + "_compilation");
    this->logger.setInfoModeFlag(this->flagInfo);
};


void Compiler::parseCommandLineArguments() {
    if(this->argCounter <= 1) {
        this->logger.logMessage(LogSeverity::Error, "No flags provided. Run compiler with flag "
                                                    "--help to see syntax and supported flags.");
    }

    for (int i = 1; i < this->argCounter; ++i) {  // Start from 1 to skip the compiler program name
        std::string arg = this->argValues[i];

        if (arg == "--help" || arg == "-h") {
            this->flagHelpInfo = true;
        } else if (arg == "--verbose" || arg == "-v") {
            this->flagVerbose = true;
        } else if (arg == "--info" || arg == "-i") {
            this->flagInfo = true;
        } else if (arg == "--linking" || arg == "-l") {
            this->flagLinking = true;
        } else if (arg == "--dump-logs" || arg == "-d") {
            this->flagDumpLogs = true;
        } else if (arg == "--source" || arg == "-s") {
            if (i + 1 < this->argCounter) {  // Ensures there's a value after '--source' or '-s'
                this->sourceFileName = this->argValues[++i];  // Get the source file name
            } else {
                std::cerr << "Error: --source requires an argument.\n";
                return;
            }
        } else if (arg == "--output" || arg == "-o") {
            if (i + 1 < this->argCounter) {  // Ensures there's a value after '--output-name' or '-o'
                this->outputFileName = this->argValues[++i];  // Get the output file name
            } else {
                std::cerr << "Error: --output-name requires an argument.\n";
                return;
            }
        } else {
            std::cerr << "Error: Unknown argument '" << arg << "'. Use --help for usage information.\n";
            return;
        }
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
    if (this->flagLinking) {
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
    if (this->flagHelpInfo) {
        std::cout << "Basic usage: zyx -s <file> -o <file> [optional-arguments]\n"
                  << "Mandatory arguments:\n"
                  << "  --source, -s <file>      Specify source file\n"
                  << "  --output, -o <file>      Specify output executable name\n"
                  << "\n"
                  << "Optional arguments:\n"
                  << "  --info, -i               Enables printing standard logs to terminal.\n"
                  << "  --link, -l               By default compiler emits object file. Use this flag to enable automatic linking using Clang linker from $PATH\n"
                  << "  --dump-logs, -d          Dump compilation logs to file\n"
                  << "  --verbose, -v            Enable verbose logging, both to terminal and log file.\n"
                  << "  --help, -h               Display this help message\n";
        exit(1);
    }

    if (!this->sourceFileName.empty()) {
        logger.logMessage(LogSeverity::Info, "Source file: " + sourceFileName);
    } else {
        logger.logMessage(LogSeverity::Error, "No source file provided. Use --source or -s to specify.");
    }

    if (!this->outputFileName.empty()) {
        logger.logMessage(LogSeverity::Info, "Output executable name: " + this->outputFileName);
    } else {
        logger.logMessage(LogSeverity::Error, "No output name provided. Use --output-name or -o to specify.");
    }

    if (this->flagInfo) {
        logger.logMessage(LogSeverity::Info, "Info mode enabled. Standard logs will be printed to console");
    } else {
        logger.logMessage(LogSeverity::Info, "Info mode disabled. No logs will be printed to console.");
    }

    if (this->flagLinking) {
        logger.logMessage(LogSeverity::Info, "Linking enabled. The object file will be linked to executable.");
    } else {
        logger.logMessage(LogSeverity::Info, "Linking disabled.");
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



