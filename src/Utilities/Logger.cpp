/* Source file for Logger class

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


#include "Logger.h"

Logger::Logger() {}

Logger::Logger(std::string name) : loggerName(std::move(name)) {}

std::string to_string(LogSeverity severity) {
    switch(severity) {
        case LogSeverity::Info:
            return LT_LOGGER_SEVERITY_LEVEL_INFO;
        case LogSeverity::Warning:
            return LT_LOGGER_SEVERITY_LEVEL_WARNING;
        case LogSeverity::Error:
            return LT_LOGGER_SEVERITY_LEVEL_ERROR;
    }
}

void Logger::logMessage(LogSeverity severity, const std::string& message) {
    std::time_t now = std::time(nullptr);
    std::string logTimestamp = std::string(std::ctime(&now));
    logTimestamp.pop_back(); // Removes the default new line at the end

    std::string prefix = to_string(severity);

    std::string fullMessage = "[" + logTimestamp + "]: " + prefix + " " + message + "\n";

    this->fullLogs.emplace_back(fullMessage);
    if(this->flagInfo || severity == LogSeverity::Error) {
        std::cout << fullMessage;
    }

    if(severity == LogSeverity::Error) {
        exit(1);
    }
}

void Logger::dumpLogs() {
    std::ofstream logFile;
    logFile.open(this->loggerName + LT_LOGS_OUTPUT_FILE_EXTENSION);

    for (const std::string& logMessage : this->fullLogs) {
        logFile << logMessage;
    }

    logFile.close();
}

void Logger::setLoggerName(std::string name) { this->loggerName = std::move(name); }

void Logger::setInfoModeFlag(bool flagInfo) {
    this->flagInfo = flagInfo;
}
