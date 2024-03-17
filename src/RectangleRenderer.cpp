#include "RectangleRenderer.h"
#include "GameRenderer.h"
#include "TransformUtil.h"
#include "Entity.h"
#include <tuple>
#include <SDL.h>
#include <iostream>

using namespace SimpleECS;
using namespace UtilSimpleECS;

void RectangleRenderer::initialize()
{

}

void RectangleRenderer::update()
{
    // Get transform coordinate
    auto screenCoord = TransformUtil::worldToScreenSpace(entity->transform->position.x, entity->transform->position.y);

    // Rectangle renders from top left corner. Center.
    int xPos = screenCoord.x - width/2;
    int yPos = screenCoord.y - height/2;

    SDL_Rect fillRect = { xPos, yPos, width, height };
    SDL_SetRenderDrawColor(GameRenderer::renderer, renderColor.r, renderColor.g, renderColor.b, renderColor.a);
    SDL_RenderFillRect(GameRenderer::renderer, &fillRect);
}