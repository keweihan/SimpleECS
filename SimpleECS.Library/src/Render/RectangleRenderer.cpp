#include "Render/RectangleRenderer.h"
#include "Core/GameRenderer.h"
#include "Utility/TransformUtil.h"
#include "Core/Entity.h"
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
    int xPos = static_cast<int>(screenCoord.x - width/2);
    int yPos = static_cast<int>(screenCoord.y - height/2);

    SDL_Rect fillRect = { xPos, yPos, width, height };
    SDL_SetRenderDrawColor(GameRenderer::renderer, renderColor.r, renderColor.g, renderColor.b, renderColor.a);
    SDL_RenderFillRect(GameRenderer::renderer, &fillRect);
}