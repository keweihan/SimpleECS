#include "TransformUtil.h"
#include "GameRenderer.h"
#include <tuple>
#include <cmath>

using namespace UtilSimpleECS;


std::pair<double, double> TransformUtil::screenToWorldSpace(int x, int y)
{
	// TODO: custom coordinate data structure may be needed
	std::pair<double, double> value;

	value.first = x - (GameRenderer::SCREEN_WIDTH / 2);
	value.second = y - GameRenderer::SCREEN_HEIGHT / 2;

	return value;
}

std::pair<int, int> TransformUtil::worldToScreenSpace(double x, double y)
{
	std::pair<int, int> value;

	value.first		= static_cast<int>(round(x + GameRenderer::SCREEN_WIDTH / 2));
	value.second	= static_cast<int>(round(-y + GameRenderer::SCREEN_HEIGHT / 2));

	return value;
}