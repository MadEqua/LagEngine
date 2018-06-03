#include "LogManager.h"
#include "Types.h"

#include <iostream>
#include <ctime>

using namespace Lag;

LogManager::LogManager() {
    initLogFile();

    for (int i = 0; i < LOG_TYPE_COUNT; ++i)
        addFlow(static_cast<LogType>(i), LogOutput::FILE);
}

LogManager::~LogManager() {
    closeLogFile();
}

void LogManager::addFlow(LogType type, LogOutput out) {
    flows[static_cast<int>(type)].insert(out);
}

void LogManager::removeFlow(LogType type, LogOutput out) {
    flows[static_cast<int>(type)].erase(out);
}

void LogManager::log(LogType type, LogVerbosity verbosity, const std::string &title, const std::string &message) {
    std::string formattedMessage;
    formatMessage(verbosity, type, title, message, formattedMessage);

    for (LogOutput out : flows[static_cast<int>(type)])
        printMessage(out, formattedMessage);
}

void LogManager::initLogFile() {
    logFile.open(LOG_FILE_NAME, std::ios::out | std::ios::app);
    if (logFile.is_open()) {
        std::string dateTime;
        appendDateTimeToString(dateTime);
        logFile << "------------------------------------------------------------\n";
        logFile << "LagEngine Session Start - " << dateTime << "\n";
        logFile << "------------------------------------------------------------\n\n";
    }
    else {
        std::cout << "Failed to open log file: " << LOG_FILE_NAME;
    }
}

void LogManager::closeLogFile() {
    if (logFile.is_open()) {
        std::string dateTime;
        appendDateTimeToString(dateTime);
        logFile << "\n------------------------------------------------------------\n";
        logFile << "LagEngine Session End - " << dateTime << "\n";
        logFile << "------------------------------------------------------------\n\n";
        logFile.close();
    }
}

void
LogManager::formatMessage(LogVerbosity verbosity, LogType type, const std::string &title, const std::string &message,
                          std::string &formattedMessage) const {
    formattedMessage.clear();

    uint32 size;
    if (verbosity == LogVerbosity::MINIMAL)
        size = 10;
    else if (verbosity == LogVerbosity::NORMAL)
        size = 20;
    else if (verbosity == LogVerbosity::VERBOSE)
        size = 45;

    formattedMessage.reserve(title.size() + message.size() + size);

    if (verbosity == LogVerbosity::VERBOSE) {
        appendDateTimeToString(formattedMessage);
        formattedMessage += " ";
    }

    if (type == LogType::DEBUG)
        formattedMessage += (verbosity == LogVerbosity::MINIMAL ? "[D]" : "[DEBUG]");
    else if (type == LogType::INFO)
        formattedMessage += (verbosity == LogVerbosity::MINIMAL ? "[I]" : "[INFO]");
    else if (type == LogType::ERROR)
        formattedMessage += (verbosity == LogVerbosity::MINIMAL ? "[*E*]" : "[** ERROR **]");
    else if (type == LogType::WARNING)
        formattedMessage += (verbosity == LogVerbosity::MINIMAL ? "[W]" : "[WARNING]");

    formattedMessage += " [" + title + "] " + message + '\n';
}

void LogManager::printMessage(LogOutput whereTo, const std::string &formattedMessage) {
    if (whereTo == LogOutput::CONSOLE) {
        std::cout << formattedMessage;
    }
    else if (whereTo == LogOutput::FILE) {
        printToFile(formattedMessage);
    }
    else if (whereTo == LogOutput::IDE) {
        //Only on Visual Studio
        //OutputDebugString(formattedMessage.c_str());
    }
}

void LogManager::printToFile(const std::string &formattedMessage) {
    if (logFile.is_open()) {
        logFile << formattedMessage;
        logFile.flush();
    }
}

void LogManager::appendDateTimeToString(std::string &str) const {
    time_t rawtime;
    std::time(&rawtime);

    tm timeinfo;
    localtime_s(&timeinfo, &rawtime);

    char buffer[25];
    strftime(buffer, 25, "[%d-%m-%y - %H:%M:%S]", &timeinfo);
    str += buffer;
}