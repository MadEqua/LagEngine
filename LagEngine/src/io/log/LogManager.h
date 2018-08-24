#pragma once

#include <string>
#include <fstream>
#include <unordered_set>

#include "Types.h"
#include "Constants.h"
#include "SingletonPattern.h"

namespace Lag {
    enum class LogOutput : uint8 {
        CONSOLE,
        FILE,
        IDE
    };

    enum class LogVerbosity : uint8 {
        VERBOSE,
        NORMAL,
        MINIMAL
    };

#undef ERROR
#undef DEBUG
    enum class LogType : uint8 {
        DEBUG,
        INFO,
        ERROR,
        WARNING
    };

    constexpr int LOG_TYPE_COUNT = 4;

    class LogManager {
    LAG_GENERATE_SINGLETON(LogManager)

    public:
        //Adds another place where each log type goes (All to file by default)
        void addFlow(LogType type, LogOutput out);
        void removeFlow(LogType type, LogOutput out);

        void log(LogType type, LogVerbosity verbosity, const std::string &title, const std::string &message);

    private:
        std::unordered_set<LogOutput> flows[LOG_TYPE_COUNT];

        std::ofstream logFile;

        void initLogFile();
        void closeLogFile();

        void formatMessage(LogVerbosity verbosity, LogType type, const std::string &title, const std::string &message,
                           std::string &formattedMessage) const;
        void printMessage(LogOutput whereTo, const std::string &formattedMessage);
        void printToFile(const std::string &formattedMessage);

        void appendDateTimeToString(std::string &str) const;
    };
}