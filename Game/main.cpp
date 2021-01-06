#include "pch.h"
#include "main.h"

namespace Simworld
{

std::unique_ptr<DXP::Simulation> Create()
{
    return std::make_unique<Game>();
}

};
