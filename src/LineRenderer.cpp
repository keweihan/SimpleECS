#include "LineRenderer.h"
#include "GameRenderer.h"
#include "TransformUtil.h"
#include <tuple>
#include <cmath>
#include "SDL.h"

using namespace SimpleECS;
using namespace UtilSimpleECS;

void LineRenderer::update()
{
	if (spacing == 0)
	{
		drawECSLine(start, end);
	}
	else
	{
		Vector line(end.x - start.x, end.y - start.y);
		double slope = line.y / line.x;

		int xRise = sqrt(pow(spacing, 2) / (pow(slope, 2) + 1));
		int yRise = sqrt(pow(spacing, 2) / (1/pow(slope, 2) + 1));
		Vector currStart = start;
		Vector nextEnd = Vector(currStart.x + xRise, currStart.y + yRise);

		while (nextEnd.distance(end) > spacing * 2)
		{
			drawECSLine(currStart, nextEnd);
			currStart = Vector(nextEnd.x + xRise, nextEnd.y + yRise);
			nextEnd = Vector(currStart.x + xRise, currStart.y + yRise);
		}
	}
}

void SimpleECS::LineRenderer::drawECSLine(Vector startPoint, Vector endPoint)
{
	Vector line(endPoint.x - startPoint.x, endPoint.y - startPoint.y);
	Vector orth = line.orthogonalVec();
	orth.normalize();

	for (int i = 0; i < width; i++)
	{
		int xOffset = -orth.x * width / 2 + i * (orth.x);
		int yOffset = -orth.y * width / 2 + i * (orth.y);

		auto startCoord = TransformUtil::worldToScreenSpace(startPoint.x + xOffset, startPoint.y + yOffset);
		auto endCoord = TransformUtil::worldToScreenSpace(endPoint.x + xOffset, endPoint.y + yOffset);
		SDL_SetRenderDrawColor(GameRenderer::renderer, renderColor.r, renderColor.g, renderColor.b, renderColor.a);
		SDL_RenderDrawLine(GameRenderer::renderer, startCoord.x, startCoord.y, endCoord.x, endCoord.y);
	}
}
