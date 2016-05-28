#pragma once

#include <string>
#include <fstream>
#include <unordered_set>

#include "../../core/SingletonPattern.h"

#define LOG_FILE_NAME "LagEngineLog.txt"

namespace Lag
{
	enum LogOutput
	{
		LAG_LOG_OUT_CONSOLE,
		LAG_LOG_OUT_FILE,
		LAG_LOG_OUT_IDE
	};

	enum LogVerbosity
	{
		LAG_LOG_VERBOSITY_VERBOSE,
		LAG_LOG_VERBOSITY_NORMAL,
		LAG_LOG_VERBOSITY_MINIMAL
	};

	enum LogType
	{
		LAG_LOG_TYPE_DEBUG,
		LAG_LOG_TYPE_INFO, 
		LAG_LOG_TYPE_ERROR, 
		LAG_LOG_TYPE_WARNING
	};
	
	
	class LogManager
	{
		LAG_GENERATE_SINGLETON(LogManager)

	public:
		//Adds another place where each log type goes (All to file by default)
		void addFlow(LogType type, LogOutput out);
		void removeFlow(LogType type, LogOutput out);

		void log(LogType type, LogVerbosity verbosity, const std::string &title, const std::string &message);

	private:
		static const int LOG_TYPE_COUNT = 4;
		std::unordered_set<LogOutput> flows[LOG_TYPE_COUNT];

		std::ofstream logFile;

		void initLogFile();
		void closeLogFile();

		void formatMessage(LogVerbosity verbosity, LogType type, const std::string &title, const std::string &message, std::string &formattedMessage) const;
		void printMessage(LogOutput whereTo, const std::string &formattedMessage);
		void printToFile(const std::string &formattedMessage);

		void appendDateTimeToString(std::string &str) const;
	};
}