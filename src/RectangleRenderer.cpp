#include "RectangleRenderer.h"
#include "GameRenderer.h"
#include "TransformUtil.h"
#include <tuple>
#include <SDL.h>

using namespace SimpleECS;
using namespace UtilSimpleECS;

void RectangleRenderer::initialize()
{

}

void RectangleRenderer::update()
{
    //Temp
    entity->transform.posX += 0.01;
    entity->transform.posY += 0.01;

    // Get transform coordinate
    std::pair<int, int> screenCoord = TransformUtil::worldToScreenSpace(entity->transform.posX, entity->transform.posY);

    // Get rectangle dimensions
    int width = GameRenderer::SCREEN_WIDTH / 2;
    int height = GameRenderer::SCREEN_HEIGHT / 2;

    // Rectangle renders from top left corner. Center.
    int xPos = screenCoord.first - width/2;
    int yPos = screenCoord.second - height/2;

    SDL_Rect fillRect = { xPos, yPos, width, height };
    SDL_SetRenderDrawColor(GameRenderer::renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_RenderFillRect(GameRenderer::renderer, &fillRect);
}