#pragma once

#include <string>
#include <fstream>

#include "LogTypes.h"

#define LOG_FILE_NAME "LagEngineLog.txt"

//TODO: add more flexibility
namespace Lag
{
	class LogManager
	{
	public:
		LogManager();
		~LogManager();

		void log(LogOutput whereTo, LogVerbosity verbosity, LogPriority priority, const std::string &title, const std::string &message);

	private:
		std::ofstream logFile;

		void initLogFile();
		void closeLogFile();

		void formatMessage(LogVerbosity verbosity, LogPriority priority, const std::string &title, const std::string &message, std::string &formattedMessage);
		void printMessage(LogOutput whereTo, const std::string &formattedMessage);
		void printToFile(const std::string &formattedMessage);

		void appendDateTimeToString(std::string &str);
	};
}