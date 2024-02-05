#include "ColliderGrid.h"
#include "GameRenderer.h"
#include <vector>

using namespace SimpleECS;
using namespace UtilSimpleECS;

ColliderGrid::ColliderGrid(const int r, const int c)
{
	cellWidth = ceil(GameRenderer::SCREEN_WIDTH / (double)c);
	cellHeight = ceil(GameRenderer::SCREEN_HEIGHT / (double)r);

	numColumn = c;
	numRow = r;

	grid.resize(r * c, EcsCell(10));
}

void SimpleECS::ColliderGrid::populateGrid()
{
	for (auto collide : colliderList)
	{
		insertToGrid(collide);
	}
}

void SimpleECS::ColliderGrid::registerCollider(Collider* collider)
{
	colliderList.insert(collider);
}

constexpr const int& clamp(const int& v, const int& lo, const int& hi)
{
	if (v < lo) { return lo; }
	if (v > hi) { return hi; }
	return v;
}

void SimpleECS::ColliderGrid::insertToGrid(Collider* collider)
{
	if (collider->entity == NULL) return;

	Collider::AABB bound;
	collider->getBounds(bound);

	// Get the left most column index this collider exists in, rightMost, etc.
	int columnLeft = ceil((bound.xMin + GameRenderer::SCREEN_WIDTH / 2.0) / cellWidth);
	int columnRight = ceil((bound.xMax + GameRenderer::SCREEN_WIDTH / 2.0) / cellWidth);
	int rowTop = ceil((-bound.yMin + GameRenderer::SCREEN_HEIGHT / 2.0) / cellHeight);
	int rowBottom = ceil((-bound.yMax + GameRenderer::SCREEN_HEIGHT / 2.0) / cellHeight);

	int colLeftClamped = clamp(columnLeft, 0, numColumn - 1);
	int colRightClamped = clamp(columnRight, 0, numColumn - 1);
	int rowBotClamped = clamp(rowBottom, 0, numRow - 1);
	int rowTopClamped = clamp(rowTop, 0, numRow - 1);

	// Add to cells this object resides in
	for (int r = rowBotClamped; r <= rowTopClamped; ++r)
	{
		for (int c = colLeftClamped; c <= colRightClamped; ++c)
		{
			// Get effective index
			int index = r * numColumn + c;
			grid[index].insert(collider);
		}
	}

	// If resides in no cells, add to out of bounds
	if (columnLeft != colLeftClamped || columnRight != colRightClamped
		|| rowTop != rowTopClamped || rowBottom != rowBotClamped)
	{
		outbounds.insert(collider);
	}
}

void SimpleECS::ColliderGrid::removeCollider(Collider* collider)
{
	// Remove from general list
	if (colliderList.find(collider) != colliderList.end())
	{
		colliderList.erase(collider);
	}

	// Search grid for references to collider and delete
	for (int i = 0; i < grid.size(); ++i)
	{
		if (grid[i].find(collider) != grid[i].end())
		{
			grid[i].erase(collider);
		}
	}

	// Remove from outbounds list
	if (outbounds.find(collider) != outbounds.end())
	{
		outbounds.erase(collider);
	}
}

void SimpleECS::ColliderGrid::updateGrid()
{
	// Add to cells this object resides in
	Collider::AABB cellBound;
	Collider::AABB colliderBound;

	// Remove collider reference in each cell if collider no longer inhabits cell
	for (int i = 0; i < grid.size(); ++i)
	{
		if (grid[i].size() == 0) continue;
		getCellBounds(cellBound, i);

		for (auto colliderIter = grid[i].begin(); colliderIter != grid[i].end();)
		{
			// If not in this cell, remove reference
			(*colliderIter)->getBounds(colliderBound);
			if (colliderBound.xMin > cellBound.xMax || colliderBound.xMax < cellBound.xMin
				|| colliderBound.yMax < cellBound.yMin || colliderBound.yMin > colliderBound.yMax)
			{
				colliderIter = grid[i].erase(colliderIter);
			}
			else
			{
				colliderIter++;
			}
		}
	}

	// Remove collider reference in outbounds if collider is no longer out of screen bounds.
	for (auto colliderIter = outbounds.begin(); colliderIter != outbounds.end();)
	{
		(*colliderIter)->getBounds(colliderBound);
		if (colliderBound.xMin >= -GameRenderer::SCREEN_WIDTH / 2.0 && colliderBound.xMax <= GameRenderer::SCREEN_WIDTH / 2.0
			&& colliderBound.yMax <= GameRenderer::SCREEN_HEIGHT / 2.0 && colliderBound.yMin >= -GameRenderer::SCREEN_HEIGHT / 2.0)
		{
			colliderIter = outbounds.erase(colliderIter);
		}
		else
		{
			++colliderIter;
		}
	}

	populateGrid();
}

int SimpleECS::ColliderGrid::size()
{
	return grid.size();
}

int SimpleECS::ColliderGrid::cellSize(int index)
{
	return 0;
}

const EcsCell* ColliderGrid::getCellContents(const int index) const
{
	return &grid[index];
}

const EcsCell* ColliderGrid::getOutBoundContent() const
{
	return &outbounds;
}

void SimpleECS::ColliderGrid::getCellBounds(Collider::AABB& output, const int& index)
{
	// index = row * numColumn + c
	int column = index % numColumn;
	int row = (index - column) / numColumn;

	output.xMin = -GameRenderer::SCREEN_WIDTH + column * cellWidth;
	output.xMax = output.xMin + cellWidth;

	output.yMax = GameRenderer::SCREEN_HEIGHT - row * cellHeight;
	output.yMin = output.yMax - cellHeight;
}
