#include "pch.h"

namespace DXP
{

CyclicBuffer::CyclicBuffer(size_t size) :
    max_size(size)
{
    buffer.reserve(size);
}

void CyclicBuffer::Write(std::string_view data)
{
    Write(data.data(), data.size());
}

void CyclicBuffer::Write(const char* data, size_t length)
{
    if (buffer.size() < max_size)
        buffer.resize(std::min(buffer.size() + length, max_size));

    // This is here, and not after write so writes can continue
    // from last position after buffer resize
    if (currentPos == buffer.size())
        currentPos = 0;

    size_t bytes_to_write = std::min(BytesToEnd(), length);
    memcpy(&buffer[currentPos], data, bytes_to_write);
    currentPos += bytes_to_write;

    if (length > bytes_to_write)
        Write(data + bytes_to_write, length - bytes_to_write);

    // Signal that our ordered copy needs to be recreated
    ordered.clear();
}

size_t CyclicBuffer::BytesToEnd()
{
    return buffer.size() - currentPos;
}

const std::vector<char>& CyclicBuffer::GetContentRef() 
{
    // Recreate if needed
    if (ordered.size() != buffer.size()) {
        ordered.resize(buffer.size());

        if (currentPos == buffer.size()) {
            memcpy(&ordered[0], &buffer[0], buffer.size());
        }
        else {
            memcpy(&ordered[0], &buffer[currentPos], BytesToEnd());
            memcpy(&ordered[BytesToEnd()], &buffer[0], currentPos);
        }
    }

    return ordered;
}

};
