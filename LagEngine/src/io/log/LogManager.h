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
		void log(LogOutput whereTo, LogVerbosity verbosity, LogType type, const std::string &title, const std::string &message);

	private:
		std::ofstream logFile;

		void initLogFile();
		void closeLogFile();

		void formatMessage(LogVerbosity verbosity, LogType type, const std::string &title, const std::string &message, std::string &formattedMessage) const;
		void printMessage(LogOutput whereTo, const std::string &formattedMessage);
		void printToFile(const std::string &formattedMessage);

		void appendDateTimeToString(std::string &str) const;
	};
}