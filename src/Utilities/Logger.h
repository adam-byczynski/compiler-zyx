/*  This is a Logger class which is used for logging almost every information across the whole project.
    Instance of a Logger is defined in Compiler class and then shared to each class responsible for each
    stage of compilation.

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


#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <format>
#include <iostream>
#include <utility>
#include <fstream>
#include <vector>

#include "../BasicComponents/LanguageLiterals.h"

enum class LogSeverity {
    Info = 0,
    Warning = 1,
    Error = 2
};

class Logger {
public:
    explicit Logger();
    explicit Logger(std::string name);

    void logMessage(LogSeverity severity, const std::string& message);
    void dumpLogs();
    void setLoggerName(std::string name);
    void setInfoModeFlag(bool flagInfo);

private:
    bool flagInfo;

    std::vector<std::string> fullLogs;
    std::string loggerName;
};

#endif // LOGGER_H
