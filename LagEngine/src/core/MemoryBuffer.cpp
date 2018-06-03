#include "MemoryBuffer.h"

#include <iostream>

using namespace Lag;

MemoryBuffer::MemoryBuffer(uint32 sizeBytes, byte *data) :
        MemoryBuffer(sizeBytes) {
    memcpy(this->data, data, sizeBytes);
}

MemoryBuffer::MemoryBuffer(uint32 sizeBytes) :
        Buffer(sizeBytes) {
    data = new byte[sizeBytes];
}

MemoryBuffer::~MemoryBuffer() {
    delete[] data;
}

void MemoryBuffer::lockImplementation(uint32 offset, uint32 length) {
}

void MemoryBuffer::unlockImplementation() {
}

byte *Lag::MemoryBuffer::mapImplementation() {
    return data + offsetLocked;
}

void Lag::MemoryBuffer::unmapImplementation() {
}

void MemoryBuffer::writeImplementation(uint32 offset, uint32 length, byte *src) {
    memcpy(data + offsetLocked + offset, src, length);
}

void MemoryBuffer::readImplementation(uint32 offset, uint32 length, byte *dst) {
    memcpy(dst, data + offsetLocked + offset, length);
}


