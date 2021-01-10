#include "pch.h"

namespace Simworld
{

DXP::RendererState state;

void Game::PreRenderLoop(DXP::Engine* engine)
{
    log = engine->CreateLogger("game");
    log->info("Starting");

    state = engine->renderer->CreateState("render-state-1");
}

void Game::PostRenderLoop(DXP::Engine* engine)
{

}

void Game::Frame(DXP::Engine* engine, const DXP::FrameInfo& frame)
{

}

};
