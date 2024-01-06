#include "TransformUtil.h"
#include "GameRenderer.h"
#include <tuple>

using namespace UtilSimpleECS;


std::pair<int, int> TransformUtil::screenToWorldSpace(int x, int y)
{
	// TODO: custom coordinate data structure may be needed
	std::pair<int, int> value;

	value.first = x + (GameRenderer::SCREEN_WIDTH / 2);
	value.second = y + GameRenderer::SCREEN_HEIGHT / 2;

	return value;
}

std::pair<int, int> TransformUtil::worldToScreenSpace(int x, int y)
{
	std::pair<int, int> value;

	value.first = x - (GameRenderer::SCREEN_WIDTH / 2);
	value.second = y - GameRenderer::SCREEN_HEIGHT / 2;

	return value;
}