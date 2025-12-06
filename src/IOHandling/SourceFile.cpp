/*
    This is a source file which defines methods used by SourceFile class.
    The methods mainly refer to opening and analysis of source code in the compilation process.


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


#include "SourceFile.h"

#include <fstream>
#include <sstream>
#include <string>


SourceFile::SourceFile(Logger &logger, const std::string &filePath) : logger(logger), filePath(filePath), lineCount(0), fileSize(0) {
    if (!this->hasValidExtension()) {
        logger.logMessage(LogSeverity::Error, "Incorrect source code file format. Supported format: " + this->requiredExtension);
    }
    logger.logMessage(LogSeverity::Info, "The source code file has supported file format .zyx");

    if (!this->openFile()) {
        logger.logMessage(LogSeverity::Error, "Could not open file: " + this->filePath);
    }
    logger.logMessage(LogSeverity::Info, "The file " + this->filePath + " has been opened.");

    if (!this->readFileContent()) {
        logger.logMessage(LogSeverity::Error, "Source code file " + this->filePath + "has not been opened or is empty.");
    }
    logger.logMessage(LogSeverity::Info, "The content of file " + this->filePath + " has been read.");

    this->file.close();
    logger.logMessage(LogSeverity::Info, "The file " + this->filePath + " has been closed.");
}

const std::string &SourceFile::getFilePath() const {
    return filePath;
}

std::size_t SourceFile::getLineCount() const {
    return lineCount;
}

std::ifstream *SourceFile::getFile() const {
    throw std::runtime_error("NotImplemented");
}

std::size_t SourceFile::getFileSize() const {
    return fileSize;
}

void SourceFile::printContentToTerminal() {
    std::cout << this->fileContent;
}

bool SourceFile::openFile() {
    try {
        file.open(this->filePath, std::ios::binary);
    } catch (const std::exception &e) {
        return false;
    }
    if (!file.is_open()) {
        return false;
    }
    return true;
}

bool SourceFile::readFileContent() {
    if (!file.is_open()) {
        return false;
    }

    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());

    if (content.empty()) {
        return false;
    }
    this->fileContent = content;
    return true;
}

const std::string *SourceFile::getContent() {
    return &this->fileContent;
}

bool SourceFile::hasValidExtension() const {
    size_t pos = this->filePath.rfind('.');
    return pos != std::string::npos && this->filePath.substr(pos) == requiredExtension;
}
