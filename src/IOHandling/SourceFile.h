/*
    This is a main class used for opening source code files,
    keeping their content and tracking their properties.

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


#ifndef SOURCEFILE_H
#define SOURCEFILE_H


#include <fstream>
#include <string>

#include "../basicComponents/LanguageLiterals.h"
#include "../utilities/Logger.h"


class SourceFile {
public:
    SourceFile(Logger& logger, const std::string& filePath);

    const std::string& getFilePath() const;
    std::size_t getLineCount() const;
    std::ifstream* getFile() const;
    std::size_t getFileSize() const;
    void printContentToTerminal();
    const std::string* getContent();

private:
    Logger& logger;

    std::string requiredExtension = LT_REQUIRED_SOURCE_CODE_FILE_EXTENSION;

    std::string filePath;
    std::string fileContent;
    std::ifstream file;
    std::size_t lineCount;
    std::size_t fileSize;

    bool openFile();
    bool readFileContent();

    bool hasValidExtension() const;
};

#endif

