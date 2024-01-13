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

void ColliderSystem::registerCollider(Collider* collider)
{
	colliderList.push_back(collider);
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
}

void SimpleECS::ColliderSystem::invokeCollisions()
{
	// O(n^2) basic implementation. See quad trees for performance improvement.
	Collision* collide = new Collision{ NULL, NULL, 0, Vector() };

	for (int i = 0; i < colliderList.size(); ++i)
	{
		for (int j = 0; j < colliderList.size(); ++j)
		{
			if (j == i) continue;
			collide->a = colliderList[i];
			collide->b = colliderList[j];
			if (getCollisionInfo(collide))
			{
				Collision* collideInfo = new Collision{ colliderList[i], colliderList[j], 0, Vector() };
				// Invoke onCollide of colliding entity components
				for (auto component : colliderList[i]->entity->getComponents())
				{
					component->onCollide(*colliderList[j]);
					component->onCollide(*collide);
				}
			}
		}
	}
}

bool SimpleECS::ColliderSystem::getCollisionBoxBox(Collision* collide)
{
	if (collide->a == nullptr || collide->b == nullptr) return false;

	Transform aTransform = collide->a->entity->transform;
	Transform bTransform = collide->b->entity->transform;

	BoxCollider* aBox = dynamic_cast<BoxCollider*>(collide->a);
	BoxCollider* bBox = dynamic_cast<BoxCollider*>(collide->b);

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
		double aExtentX = aBox->width / 2;
		double bExtentX = bBox->width / 2;
		double aExtentY = aBox->height / 2;
		double bExtentY = bBox->height / 2;

		double xDistance = std::abs(collide->a->entity->transform.position.x - collide->b->entity->transform.position.x);
		double xOverlap = (aExtentX + bExtentX) - xDistance;

		double yDistance = std::abs(collide->a->entity->transform.position.y - collide->b->entity->transform.position.y);
		double yOverlap = (aExtentY + bExtentY) - yDistance;

		// Least penetration is on y-axis
		if (yOverlap < xOverlap)
		{
			collide->penetration = yOverlap;
			if (aTransform.position.y < bTransform.position.y)
			{
				collide->normal = Vector(0, -1);
			}
			else
			{
				collide->normal = Vector(0, 1);
			}
		}
		// Least penetration is on x-axis
		else
		{
			collide->penetration = xOverlap;
			if (aTransform.position.x < bTransform.position.x)
			{
				collide->normal = Vector(-1, 0);
			}
			else
			{
				collide->normal = Vector(1, 0);
			}
		}
	}


	return true;
}


bool SimpleECS::ColliderSystem::getCollisionInfo(Collision* collide)
{
	if (collide->a == nullptr || collide->b == nullptr) return false;

	// Breaking principles of polymorphism (likely) necessary. 
	// Different collider collisions (i.e. sphere-sphere, sphere-box, box-box) 
	// require different implementation.
    if (dynamic_cast<BoxCollider*>(collide->a) != nullptr && 
		dynamic_cast<BoxCollider*>(collide->b) != nullptr)
    {
		return getCollisionBoxBox(collide);
    }
	// Other collider types here
	// else if(sphere-sphere...)

    return false;
}

ColliderGrid::ColliderGrid(const int r, const int c)
{
	cellWidth	= ceil(GameRenderer::SCREEN_HEIGHT/ c);
	cellHeight	= ceil(GameRenderer::SCREEN_WIDTH / r);

	numColumn = c;
	numRow = r;

	grid.resize(r * c);
}

void SimpleECS::ColliderGrid::populateGrid(const std::vector<Collider*>& list)
{
	for (auto collide : list)
	{
		addCollider(collide);
	}
}

void SimpleECS::ColliderGrid::addCollider(Collider* collider)
{
	Collider::AABB bound;
	collider->getBounds(bound);

	// Get SDL space coordinates
	Vector minBound = UtilSimpleECS::TransformUtil::worldToScreenSpace(bound.xMin, bound.yMin);
	Vector maxBound = UtilSimpleECS::TransformUtil::worldToScreenSpace(bound.xMax, bound.yMax);

	// Get the left most column index this collider exists in, rightMost, etc.
	int columnLeft = minBound.x / cellWidth;
	int columnRight = maxBound.x / cellWidth;
	int rowTop = maxBound.y / cellHeight;
	int rowBottom = minBound.y / cellHeight;

	// Add to cells this object resides in
	for (int c = columnLeft; c < columnRight; ++c)
	{
		for (int r = rowTop; r < rowBottom; ++r)
		{
			// Get effective index
			int index = r * numColumn + c;
			grid[index].insert(collider);
		}
	}
}

void SimpleECS::ColliderGrid::removeCollider(Collider* collider)
{
	// Search grid for references to collider and delete
	for (int i = 0; i < grid.size(); ++i)
	{
		if (grid[i].find(collider) != grid[i].end())
		{
			grid[i].erase(collider);
		}
	}
}

void SimpleECS::ColliderGrid::updateGrid()
{
	// Add to cells this object resides in
	Collider::AABB cellBound;
	Collider::AABB colliderBound;

	for (int c = 0; c < numColumn; ++c)
	{
		for (int r = 0; r < numRow; ++r)
		{
			// Get effective index
			int index = r * numRow + c;
			getCellBounds(cellBound, index);
			for (auto colliderIter = grid[index].begin(); colliderIter != grid[index].end(); ++colliderIter)
			{
				// If not in this cell, remove reference
				(*colliderIter)->getBounds(colliderBound);
				if (colliderBound.xMin > cellBound.xMax || colliderBound.xMax < cellBound.xMin
					|| colliderBound.yMax < cellBound.yMin || colliderBound.yMin > colliderBound.yMax)
				{
					colliderIter = grid[index].erase(colliderIter);
				}

				// TODO: only one call to this per unique collider is necessary. Currently potentially
				// calls more. 
				addCollider(*colliderIter);
			}
		}
	}

}

void SimpleECS::ColliderGrid::gridSize()
{
}

int SimpleECS::ColliderGrid::cellSize(int index)
{
	return 0;
}

const std::unordered_set<Collider*>& const SimpleECS::ColliderGrid::getCellContents(const int index)
{
	return grid[index];
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


