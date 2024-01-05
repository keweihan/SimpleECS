#include "RectangleRenderer.h"
#include "GameRenderer.h"
#include <SDL.h>

using namespace SimpleECS;


void RectangleRenderer::initialize()
{

}

void RectangleRenderer::update()
{
    SDL_Rect fillRect = { GameRenderer::SCREEN_WIDTH / 4, GameRenderer::SCREEN_HEIGHT / 4, GameRenderer::SCREEN_HEIGHT /2, GameRenderer::SCREEN_HEIGHT /2};
    SDL_SetRenderDrawColor(GameRenderer::renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(GameRenderer::renderer, &fillRect);
}