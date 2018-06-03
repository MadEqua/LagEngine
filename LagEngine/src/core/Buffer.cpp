#include "Buffer.h"
#include "LogManager.h"

using namespace Lag;

Buffer::Buffer(uint32 sizeBytes) :
        sizeBytes(sizeBytes),
        isLocked(false),
        isMapped(false) {
}

void Buffer::lock(uint32 offset, uint32 length) {
    if (isLocked) {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                      "Buffer", "Trying to lock an already locked buffer.");
        return;
    }
    else if (offset + length > sizeBytes || offset < 0) {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                      "Buffer", "Trying to lock an area beyond buffer boundaries.");
        return;
    }
    else if (length <= 0) {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                      "Buffer", "Trying to lock an area with zero size.");
        return;
    }

    isLocked = true;
    offsetLocked = offset;
    lengthLocked = length;
    lockImplementation(offset, length);
}

void Buffer::lock() {
    lock(0, sizeBytes);
}

void Buffer::unlock() {
    if (!isLocked) {
        LogManager::getInstance().log(LogType::WARNING, LogVerbosity::NORMAL,
                                      "Buffer", "Trying to unlock a non-locked buffer.");
        return;
    }

    unlockImplementation();
    isMapped = false;
    isLocked = false;
}

byte *Buffer::map() {
    if (!isLocked) {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                      "Buffer", "Trying to map a non-locked buffer.");
        return nullptr;
    }
    else if (isMapped) {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                      "Buffer", "Trying to map an already mapped buffer.");
        return nullptr;
    }

    isMapped = true;
    return mapImplementation();
}

void Buffer::unmap() {
    if (!isLocked) {
        LogManager::getInstance().log(LogType::WARNING, LogVerbosity::NORMAL,
                                      "Buffer", "Trying to unmap a non-locked buffer.");
        return;
    }
    else if (!isMapped) {
        LogManager::getInstance().log(LogType::WARNING, LogVerbosity::NORMAL,
                                      "Buffer", "Trying to unmap an non-mapped buffer.");
        return;
    }

    isMapped = false;
    unmapImplementation();
}

void Buffer::write(uint32 offset, uint32 length, byte *src) {
    if (!isLocked) {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                      "Buffer", "Trying to write into a non-locked buffer.");
        return;
    }
    else if (offset + length > sizeBytes || offset < 0) {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                      "Buffer", "Trying to write beyond buffer boundaries.");
        return;
    }
    else if (length < 0) {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                      "Buffer", "Trying to write to an area with zero size.");
        return;
    }
    else if (src == nullptr) {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                      "Buffer", "Trying to write from null source data.");
        return;
    }

    writeImplementation(offset, length, src);
}

void Buffer::read(uint32 offset, uint32 length, byte *dst) {
    if (!isLocked) {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                      "Buffer", "Trying to read from a non-locked buffer.");
        return;
    }
    else if (offset + length > sizeBytes) {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                      "Buffer", "Trying to read beyond buffer boundaries.");
        return;
    }
    else if (length < 0) {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                      "Buffer", "Trying to read an area with zero size.");
        return;
    }
    else if (dst == nullptr) {
        LogManager::getInstance().log(LogType::ERROR, LogVerbosity::NORMAL,
                                      "Buffer", "Trying to read to a null destination.");
        return;
    }

    readImplementation(offset, length, dst);
}