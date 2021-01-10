#include "pch.h"
#include <fstream>
#include <filesystem>

namespace DXP
{

std::string FileGetContent(std::string_view path)
{
    using namespace std;
    std::string result;

    ifstream stream(path, ios::in | ios::binary | ios::ate);
    assert(stream.is_open());

    result.resize(stream.tellg());
    stream.seekg(ios::beg);
    stream.read(&result[0], result.size());
    stream.close();

    return result;
}

};
