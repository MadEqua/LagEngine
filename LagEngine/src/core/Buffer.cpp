#include "Buffer.h"
#include "../io/log/LogManager.h"

using namespace Lag;

Buffer::Buffer(uint32 sizeBytes) :
	sizeBytes(sizeBytes),
	isLocked(false),
	isMapped(false)
{
}

Buffer::~Buffer()
{
}

void Buffer::lock(uint32 offset, uint32 length)
{
	if (isLocked)
	{
		LogManager::getInstance().log(LogOutput::LAG_LOG_OUT_FILE, LogVerbosity::LAG_LOG_VERBOSITY_NORMAL, LogType::LAG_LOG_TYPE_ERROR,
			"Buffer", "Trying to lock an already locked buffer.");
		return;
	}
	else if (offset + length > sizeBytes)
	{
		LogManager::getInstance().log(LogOutput::LAG_LOG_OUT_FILE, LogVerbosity::LAG_LOG_VERBOSITY_NORMAL, LogType::LAG_LOG_TYPE_ERROR,
			"Buffer", "Trying to lock an area beyond buffer boundaries.");
		return;
	}

	isLocked = true;
	offsetLocked = offset;
	lengthLocked = length;
	lockImplementation(offset, length);
}

void Buffer::lock()
{
	lock(0, sizeBytes);
}

void Buffer::unlock()
{
	if (!isLocked)
	{
		LogManager::getInstance().log(LogOutput::LAG_LOG_OUT_FILE, LogVerbosity::LAG_LOG_VERBOSITY_NORMAL, LogType::LAG_LOG_TYPE_WARNING,
			"Buffer", "Trying to unlock a non-locked buffer.");
		return;
	}

	unlockImplementation();
	isLocked = false;
}

byte* Buffer::map()
{
	if (!isLocked)
	{
		LogManager::getInstance().log(LogOutput::LAG_LOG_OUT_FILE, LogVerbosity::LAG_LOG_VERBOSITY_NORMAL, LogType::LAG_LOG_TYPE_ERROR,
			"Buffer", "Trying to map a non-locked buffer.");
		return nullptr;
	}
	else if (isMapped)
	{
		LogManager::getInstance().log(LogOutput::LAG_LOG_OUT_FILE, LogVerbosity::LAG_LOG_VERBOSITY_NORMAL, LogType::LAG_LOG_TYPE_ERROR,
			"Buffer", "Trying to map an already mapped buffer.");
		return nullptr;
	}
	
	isMapped = true;
	return mapImplementation();
}

void Buffer::unmap()
{
	if (!isLocked)
	{
		LogManager::getInstance().log(LogOutput::LAG_LOG_OUT_FILE, LogVerbosity::LAG_LOG_VERBOSITY_NORMAL, LogType::LAG_LOG_TYPE_WARNING,
			"Buffer", "Trying to unmap a non-locked buffer.");
		return;
	}
	else if (!isMapped)
	{
		LogManager::getInstance().log(LogOutput::LAG_LOG_OUT_FILE, LogVerbosity::LAG_LOG_VERBOSITY_NORMAL, LogType::LAG_LOG_TYPE_WARNING,
			"Buffer", "Trying to unmap an non-mapped buffer.");
		return;
	}
	
	isMapped = false;
	unmapImplementation();
}

void Buffer::write(uint32 offset, uint32 length, byte* src)
{
	if (!isLocked)
	{
		LogManager::getInstance().log(LogOutput::LAG_LOG_OUT_FILE, LogVerbosity::LAG_LOG_VERBOSITY_NORMAL, LogType::LAG_LOG_TYPE_ERROR,
			"Buffer", "Trying to write into a non-locked buffer.");
		return;
	}
	else if (offset + length > sizeBytes)
	{
		LogManager::getInstance().log(LogOutput::LAG_LOG_OUT_FILE, LogVerbosity::LAG_LOG_VERBOSITY_NORMAL, LogType::LAG_LOG_TYPE_ERROR,
			"Buffer", "Trying to write beyond buffer boundaries.");
		return;
	}

	writeImplementation(offset, length, src);
}

void Buffer::read(uint32 offset, uint32 length, byte* dst)
{
	if (!isLocked)
	{
		LogManager::getInstance().log(LogOutput::LAG_LOG_OUT_FILE, LogVerbosity::LAG_LOG_VERBOSITY_NORMAL, LogType::LAG_LOG_TYPE_ERROR,
			"Buffer", "Trying to read from a non-locked buffer.");
		return;
	}
	else if (offset + length > sizeBytes)
	{
		LogManager::getInstance().log(LogOutput::LAG_LOG_OUT_FILE, LogVerbosity::LAG_LOG_VERBOSITY_NORMAL, LogType::LAG_LOG_TYPE_ERROR,
			"Buffer", "Trying to read beyond buffer boundaries.");
		return;
	}

	readImplementation(offset, length, dst);
}