#include "pch.h"

namespace DXP
{

void CyclicLog::Write(std::string_view log)
{
    buffer.Write(log);
}

const std::vector<char>& CyclicLog::GetLogBufferRef()
{
    return buffer.GetContentRef();
}

};
