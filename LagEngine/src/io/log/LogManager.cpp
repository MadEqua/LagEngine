#include "LogManager.h"
#include "../../Types.h"

#include <iostream>
#include <ctime>
#include "Windows.h"

using namespace Lag;

LogManager::LogManager()
{
	initLogFile();

	for (int i = 0; i < LOG_TYPE_COUNT; ++i)
		addFlow(static_cast<LogType>(i), LAG_LOG_OUT_FILE);
}

LogManager::~LogManager()
{
	closeLogFile();
}

void LogManager::addFlow(LogType type, LogOutput out)
{
	flows[type].insert(out);
}

void LogManager::removeFlow(LogType type, LogOutput out)
{
	flows[type].erase(out);
}

void LogManager::log(LogType type, LogVerbosity verbosity, const std::string &title, const std::string &message)
{
	std::string formattedMessage;
	formatMessage(verbosity, type, title, message, formattedMessage);

	for(LogOutput out : flows[type])
		printMessage(out, formattedMessage);
}

void LogManager::initLogFile()
{
	logFile.open(LOG_FILE_NAME, std::ios::out | std::ios::app);
	if (logFile.is_open())
	{
		std::string dateTime;
		appendDateTimeToString(dateTime);
		logFile << "------------------------------------------------------------\n";
		logFile << "LagEngine Session Start - " << dateTime << "\n";
		logFile << "------------------------------------------------------------\n\n";
	}
	else
	{
		log(LAG_LOG_TYPE_ERROR, LAG_LOG_VERBOSITY_NORMAL, LOG_FILE_NAME, "Failed to open log file.");
	}
}

void LogManager::closeLogFile()
{
	if (logFile.is_open())
	{
		std::string dateTime;
		appendDateTimeToString(dateTime);
		logFile << "\n------------------------------------------------------------\n";
		logFile << "LagEngine Session End - " << dateTime << "\n";
		logFile << "------------------------------------------------------------\n\n";
		logFile.close();
	}
}

void LogManager::formatMessage(LogVerbosity verbosity, LogType type, const std::string &title, const std::string &message, std::string &formattedMessage) const
{
	formattedMessage.clear();

	uint32 size;
	if (verbosity == LAG_LOG_VERBOSITY_MINIMAL)
		size = 10;
	else if (verbosity == LAG_LOG_VERBOSITY_NORMAL)
		size = 20;
	else if (verbosity == LAG_LOG_VERBOSITY_VERBOSE)
		size = 45;

	formattedMessage.reserve(title.size() + message.size() + size);

	if (verbosity == LAG_LOG_VERBOSITY_VERBOSE)
	{
		appendDateTimeToString(formattedMessage);
		formattedMessage += " ";
	}

	if (type == LAG_LOG_TYPE_DEBUG)
		formattedMessage += (verbosity == LAG_LOG_VERBOSITY_MINIMAL ? "[D]" : "[DEBUG]");
	else if (type == LAG_LOG_TYPE_INFO)
		formattedMessage += (verbosity == LAG_LOG_VERBOSITY_MINIMAL ? "[I]" : "[INFO]");
	else if (type == LAG_LOG_TYPE_ERROR)
		formattedMessage += (verbosity == LAG_LOG_VERBOSITY_MINIMAL ? "[*E*]" : "[** ERROR **]");
	else if (type == LAG_LOG_TYPE_WARNING)
		formattedMessage += (verbosity == LAG_LOG_VERBOSITY_MINIMAL ? "[W]" : "[WARNING]");

	formattedMessage += " [" + title + "] " + message + '\n';
}

void LogManager::printMessage(LogOutput whereTo, const std::string &formattedMessage)
{
	if (whereTo == LAG_LOG_OUT_CONSOLE)
	{
		std::cout << formattedMessage;
	}
	else if (whereTo == LAG_LOG_OUT_FILE)
	{
		printToFile(formattedMessage);
	}
	else if (whereTo == LAG_LOG_OUT_IDE)
	{
		OutputDebugString(formattedMessage.c_str());
	}
}

void LogManager::printToFile(const std::string &formattedMessage)
{
	if (logFile.is_open())
	{
		logFile << formattedMessage;
		logFile.flush();
	}
}

void LogManager::appendDateTimeToString(std::string &str) const
{
	time_t rawtime;
	std::time(&rawtime);

	tm timeinfo;
	localtime_s(&timeinfo, &rawtime);

	char buffer[25];
	strftime(buffer, 25, "[%d-%m-%y - %H:%M:%S]", &timeinfo);
	str += buffer;
}