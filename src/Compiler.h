/* Main class of the project defining the Compiler object.
   The compiler object is responsible for whole compilation process,
   from reading input source code, to generation of executable file.

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


#ifndef THESIS_COMPILER_COMPILER_H
#define THESIS_COMPILER_COMPILER_H


#include <vector>

#include "Utilities/Logger.h"

class Compiler {

public:
    Compiler(int argc, char *argv[]);

    void runCompilation();


private:
    Logger logger;

    // Compiler CommandLine Arguments
    int argCounter{};
    std::vector<std::string> argValues;

    // Compiler Parameters and Flags
    std::string sourceFileName;
    std::string outputFileName;

    bool flagInfo{};
    bool flagVerbose{};
    bool flagHelpInfo{};
    bool flagLinking{};
    bool flagDumpLogs{};

    void parseCommandLineArguments();
    void printCompilationOptions();
};


#endif //THESIS_COMPILER_COMPILER_H
