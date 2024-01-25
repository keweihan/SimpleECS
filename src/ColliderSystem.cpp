#include "ColliderSystem.h"
#include "Collider.h"
#include "Component.h"
#include "Entity.h"
#include "GameRenderer.h"
#include "TransformUtil.h"
#include <vector>

using namespace SimpleECS;
using namespace UtilSimpleECS;

std::vector<Collider*> ColliderSystem::colliderList;
ColliderGrid ColliderSystem::colliderGrid(ColliderSystem::GRID_ROWS, ColliderSystem::GRID_COLUMNS);

void ColliderSystem::registerCollider(Collider* collider)
{
	colliderList.push_back(collider);
	colliderGrid.registerCollider(collider);
}

void ColliderSystem::deregisterCollider(Collider* collider)
{
	// Find and remove registered instances of this collider
	for (auto iter = colliderList.begin(); iter != colliderList.end();)
	{
		if (*iter == collider)
		{
			iter = colliderList.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	colliderGrid.removeCollider(collider);
}

void SimpleECS::ColliderSystem::invokeCollisions()
{
	colliderGrid.updateGrid();
	Collision collision = {};

	// TODO: Repeats collision calculations. More repeats if objects span more than one
	// grid.
	for (int i = 0; i < colliderGrid.size(); ++i)
	{
		for (auto& colliderA : colliderGrid.getCellContents(i))
		{
			for (auto& colliderB : colliderGrid.getCellContents(i))
			{
				if (colliderA == colliderB) continue;
				collision.a = colliderA;
				collision.b = colliderB;
				if (getCollisionInfo(collision))
				{
					// Invoke onCollide of colliding entity components
					for (auto component : colliderA->entity->getComponents())
					{
						component->onCollide(*colliderB);
						component->onCollide(collision);
					}
				}
			}
		}
	}

	for (auto& colliderA : colliderGrid.getOutBoundContent())
	{
		for (auto& colliderB : colliderGrid.getOutBoundContent())
		{
			if (colliderA == colliderB) continue;
			collision.a = colliderA;
			collision.b = colliderB;
			if (getCollisionInfo(collision))
			{
				// Invoke onCollide of colliding entity components
				for (auto component : colliderA->entity->getComponents())
				{
					component->onCollide(*colliderB);
					component->onCollide(collision);
				}
			}
		}
	}
}

bool SimpleECS::ColliderSystem::getCollisionBoxBox(Collision& collide)
{
	if (collide.a == nullptr || collide.b == nullptr) return false;

	Transform aTransform = collide.a->entity->transform;
	Transform bTransform = collide.b->entity->transform;

	BoxCollider* aBox = dynamic_cast<BoxCollider*>(collide.a);
	BoxCollider* bBox = dynamic_cast<BoxCollider*>(collide.b);

	if (bBox != nullptr && aBox != nullptr)
	{
		// AABB Collision 
		Collider::AABB aBounds;
		Collider::AABB bBounds;
		aBox->getBounds(aBounds);
		bBox->getBounds(bBounds);

		//If any of the sides from A are outside of B, no collision occuring.
		if (aBounds.yMin >= bBounds.yMax || aBounds.yMax <= bBounds.yMin 
			|| aBounds.xMax <= bBounds.xMin || aBounds.xMin >= bBounds.xMax)
		{
			return false;
		}

		// Boxes are colliding. Find axis of least penetration
		double aExtentX = aBox->width / 2.0;
		double bExtentX = bBox->width / 2.0;
		double aExtentY = aBox->height / 2.0;
		double bExtentY = bBox->height / 2.0;

		double xDistance = std::abs(aTransform.position.x - bTransform.position.x);
		double xOverlap = (aExtentX + bExtentX) - xDistance;

		double yDistance = std::abs(aTransform.position.y - bTransform.position.y);
		double yOverlap = (aExtentY + bExtentY) - yDistance;

		//if ((yOverlap < xOverlap ? yOverlap : xOverlap) == 0) {
		//	return false;
		//}

		// Least penetration is on y-axis
		if (yOverlap < xOverlap)
		{
			collide.penetration = yOverlap;
			if (aTransform.position.y < bTransform.position.y)
			{
				collide.normal = Vector(0, -1);
			}
			else
			{
				collide.normal = Vector(0, 1);
			}
		}
		// Least penetration is on x-axis
		else
		{
			collide.penetration = xOverlap;
			if (aTransform.position.x < bTransform.position.x)
			{
				collide.normal = Vector(-1, 0);
			}
			else
			{
				collide.normal = Vector(1, 0);
			}
		}
	}


	return true;
}


bool SimpleECS::ColliderSystem::getCollisionInfo(Collision& collide)
{
	if (collide.a == nullptr || collide.b == nullptr) return false;

	// Breaking principles of polymorphism (likely) necessary. 
	// Different collider collisions (i.e. sphere-sphere, sphere-box, box-box) 
	// require different implementation.
    if (dynamic_cast<BoxCollider*>(collide.a) != nullptr && 
		dynamic_cast<BoxCollider*>(collide.b) != nullptr)
    {
		return getCollisionBoxBox(collide);
    }
	// Other collider types here
	// else if(sphere-sphere...)

    return false;
}

ColliderGrid::ColliderGrid(const int r, const int c)
{
	cellWidth	= ceil(GameRenderer::SCREEN_WIDTH / (double)c);
	cellHeight	= ceil(GameRenderer::SCREEN_HEIGHT / (double)r);

	numColumn = c;
	numRow = r;

	grid.resize(r * c);
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
	int columnLeft	= ceil((bound.xMin + GameRenderer::SCREEN_WIDTH / 2.0) / cellWidth);
	int columnRight = ceil((bound.xMax + GameRenderer::SCREEN_WIDTH / 2.0) / cellWidth);
	int rowTop		= ceil((-bound.yMin + GameRenderer::SCREEN_HEIGHT / 2.0) / cellHeight);
	int rowBottom	= ceil((-bound.yMax + GameRenderer::SCREEN_HEIGHT / 2.0) / cellHeight);

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
			colliderIter++;
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

const std::unordered_set<Collider*>& SimpleECS::ColliderGrid::getCellContents(const int index)
{
	return grid[index];
}

const std::unordered_set<Collider*>& const SimpleECS::ColliderGrid::getOutBoundContent()
{
	return outbounds;
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


