#pragma once

namespace DXP
{

struct CyclicBuffer
{
    CyclicBuffer(size_t size);

    void Write(std::string_view data);
    void Write(const char *data, size_t length);

    const std::vector<char>& GetContentRef();

private:
    size_t BytesToEnd();

    size_t max_size;
    std::vector<char> buffer;   // Data goes here
    size_t currentPos = 0;      // Current write position

    std::vector<char> ordered;  // Ordered copy to return
};

};
