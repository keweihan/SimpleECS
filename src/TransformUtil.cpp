#include "TransformUtil.h"
#include "GameRenderer.h"
#include "Vector.h"
#include <cmath>

using namespace UtilSimpleECS;

SimpleECS::Vector TransformUtil::screenToWorldSpace(int x, int y)
{
	double worldX = x - (GameRenderer::SCREEN_WIDTH / 2);
	double worldY = y - GameRenderer::SCREEN_HEIGHT / 2;

	return SimpleECS::Vector(worldX, worldY);
}

SimpleECS::Vector TransformUtil::worldToScreenSpace(double x, double y)
{
	int screenX	= static_cast<int>(round(x + GameRenderer::SCREEN_WIDTH / 2));
	
	// -y due to inverted Y of SDL coordinates
	int screenY	= static_cast<int>(round(-y + GameRenderer::SCREEN_HEIGHT / 2));

	return SimpleECS::Vector(screenX, screenY);
}