#include "LogManager.h"
#include "../../Types.h"

#include <iostream>
#include <ctime>

using namespace Lag;

LogManager::LogManager()
{
	initLogFile();
}

LogManager::~LogManager()
{
	closeLogFile();
}

void LogManager::log(LogOutput whereTo, LogVerbosity verbosity, LogPriority priority, const std::string &title, const std::string &message)
{
	std::string formattedMessage;
	formatMessage(verbosity, priority, title, message, formattedMessage);
	printMessage(whereTo, formattedMessage);
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
		log(CONSOLE, NORMAL, ERROR, LOG_FILE_NAME, "Failed to open log file.");
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

void LogManager::formatMessage(LogVerbosity verbosity, LogPriority priority, const std::string &title, const std::string &message, std::string &formattedMessage) const
{
	formattedMessage.clear();

	uint32 size;
	if (verbosity == MINIMAL)
		size = 10;
	else if (verbosity == NORMAL)
		size = 15;
	else if (verbosity == VERBOSE)
		size = 40;

	formattedMessage.reserve(title.size() + message.size() + size);

	if (verbosity == VERBOSE)
	{
		appendDateTimeToString(formattedMessage);
		formattedMessage += " ";
	}

	if (priority == DEBUG)
		formattedMessage += (verbosity == MINIMAL ? "[D]" : "[DEBUG]");
	else if (priority == INFO)
		formattedMessage += (verbosity == MINIMAL ? "[I]" : "[INFO]");
	else if (priority == ERROR)
		formattedMessage += (verbosity == MINIMAL ? "[E]" : "[ERROR]");
	else if (priority == WTF)
		formattedMessage += (verbosity == MINIMAL ? "[?]" : "[WTF!?]");

	formattedMessage += " [" + title + "] " + message;
}

void LogManager::printMessage(LogOutput whereTo, const std::string &formattedMessage)
{
	if (whereTo == CONSOLE)
	{
		std::cout << formattedMessage << std::endl;
	}
	else if (whereTo == FILE)
	{
		printToFile(formattedMessage);
	}
	else if (whereTo == IDE)
	{
		//TODO
	}
}

void LogManager::printToFile(const std::string &formattedMessage)
{
	if (logFile.is_open())
	{
		logFile << formattedMessage << std::endl;
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