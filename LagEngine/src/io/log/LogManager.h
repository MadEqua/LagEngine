#pragma once

#include <string>
#include <fstream>

#include "../../core/SingletonPattern.h"

#define LOG_FILE_NAME "LagEngineLog.txt"

//TODO: add more flexibility
namespace Lag
{
	enum LogOutput
	{
		CONSOLE,
		FILE,
		IDE
	};

	enum LogVerbosity
	{
		VERBOSE,
		NORMAL,
		MINIMAL
	};

	enum LogPriority
	{
		DEBUG,
		INFO, 
		ERROR, 
		WARNING
	};
	
	
	class LogManager
	{
		GENERATE_SINGLETON(LogManager)

	public:
		void log(LogOutput whereTo, LogVerbosity verbosity, LogPriority priority, const std::string &title, const std::string &message);

	private:
		std::ofstream logFile;

		void initLogFile();
		void closeLogFile();

		void formatMessage(LogVerbosity verbosity, LogPriority priority, const std::string &title, const std::string &message, std::string &formattedMessage) const;
		void printMessage(LogOutput whereTo, const std::string &formattedMessage);
		void printToFile(const std::string &formattedMessage);

		void appendDateTimeToString(std::string &str) const;
	};
}