#include "pch.h"

namespace Simworld
{

void Game::PreRenderLoop(DXP::Engine* engine)
{
    log = engine->CreateLogger("game");
    log->info("Starting");
}

void Game::PostRenderLoop(DXP::Engine* engine)
{

}

};
