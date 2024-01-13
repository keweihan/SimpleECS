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
	colliderGrid.addCollider(collider);
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
	//Collision* collide = new Collision{ NULL, NULL, 0, Vector() };

	//for (int i = 0; i < colliderList.size(); ++i)
	//{
	//	for (int j = 0; j < colliderList.size(); ++j)
	//	{
	//		if (j == i) continue;
	//		collide->a = colliderList[i];
	//		collide->b = colliderList[j];
	//		if (getCollisionInfo(collide))
	//		{
	//			// Invoke onCollide of colliding entity components
	//			for (auto component : colliderList[i]->entity->getComponents())
	//			{
	//				component->onCollide(*colliderList[j]);
	//				component->onCollide(*collide);
	//			}
	//		}
	//	}
	//}

	colliderGrid.updateGrid();
	Collision* collide = new Collision{ NULL, NULL, 0, Vector() };

	for (int i = 0; i < colliderGrid.gridSize(); ++i)
	{
		for (auto colliderA : colliderGrid.getCellContents(i))
		{
			for (auto colliderB : colliderGrid.getCellContents(i))
			{
				if (colliderA == colliderB) continue;
				collide->a = colliderA;
				collide->b = colliderB;
				if (getCollisionInfo(collide))
				{
					// Invoke onCollide of colliding entity components
					for (auto component : colliderA->entity->getComponents())
					{
						component->onCollide(*colliderB);
						component->onCollide(*collide);
					}
				}
			}
		}
	}

	for (auto colliderA : colliderGrid.getOutBoundContent())
	{
		for (auto colliderB : colliderGrid.getOutBoundContent())
		{
			if (colliderA == colliderB) continue;
			collide->a = colliderA;
			collide->b = colliderB;
			if (getCollisionInfo(collide))
			{
				// Invoke onCollide of colliding entity components
				for (auto component : colliderA->entity->getComponents())
				{
					component->onCollide(*colliderB);
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
	cellWidth	= ceil(GameRenderer::SCREEN_WIDTH / c);
	cellHeight	= ceil(GameRenderer::SCREEN_HEIGHT / r);

	numColumn = c;
	numRow = r;

	grid.resize(r * c);
}

void SimpleECS::ColliderGrid::populateGrid()
{
	for (auto collide : colliderList)
	{
		addCollider(collide);
	}
}

void SimpleECS::ColliderGrid::addCollider(Collider* collider)
{
	colliderList.insert(collider);
	if (collider->entity == NULL) return;

	Collider::AABB bound;
	collider->getBounds(bound);

	// Get SDL space coordinates of this collider
	Vector minBound = UtilSimpleECS::TransformUtil::worldToScreenSpace(bound.xMin, bound.yMin);
	Vector maxBound = UtilSimpleECS::TransformUtil::worldToScreenSpace(bound.xMax, bound.yMax);

	// Get the left most column index this collider exists in, rightMost, etc.
	int columnLeft = (bound.xMin + GameRenderer::SCREEN_WIDTH / 2) / cellWidth;
	int columnRight = (bound.xMax + GameRenderer::SCREEN_WIDTH / 2) / cellWidth;
	int rowTop = maxBound.y / cellHeight;
	int rowBottom = minBound.y / cellHeight;

	// Add to cells this object resides in
	for (int r = std::max(rowTop, 0); r <= std::min(rowBottom, numRow - 1); ++r)
	{
		for (int c = std::max(columnLeft, 0); c <= std::min(columnRight, numColumn - 1); ++c)
		{
			// Get effective index
			int index = r * numColumn + c;
			grid[index].insert(collider);
		}
	}

	if (columnLeft < 0 || columnRight > numColumn || rowTop < 0 || rowBottom > numRow)
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

	for (int r = 0; r < numRow; ++r)
	{
		for (int c = 0; c < numColumn; ++c)
		{
			// Get effective index
			int index = r * numColumn + c;
			getCellBounds(cellBound, index);
			for (auto colliderIter = grid[index].begin(); colliderIter != grid[index].end();)
			{
				// If not in this cell, remove reference
				(*colliderIter)->getBounds(colliderBound);
				if (colliderBound.xMin > cellBound.xMax || colliderBound.xMax < cellBound.xMin
					|| colliderBound.yMax < cellBound.yMin || colliderBound.yMin > colliderBound.yMax)
				{
					colliderIter = grid[index].erase(colliderIter);
				}
				else
				{
					colliderIter++;
				}

				// TODO: only one call to this per unique collider is necessary. Currently potentially
				// calls more. 
				//addCollider(*colliderIter);
			}
		}
	}

	for (auto colliderIter = outbounds.begin(); colliderIter != outbounds.end();)
	{
		(*colliderIter)->getBounds(colliderBound);
		if (colliderBound.xMin >= -GameRenderer::SCREEN_WIDTH / 2 && colliderBound.xMax <= GameRenderer::SCREEN_WIDTH / 2
			&& colliderBound.yMax <= GameRenderer::SCREEN_HEIGHT / 2 && colliderBound.yMin >= -GameRenderer::SCREEN_HEIGHT / 2)
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

int SimpleECS::ColliderGrid::gridSize()
{
	return grid.size();
}

int SimpleECS::ColliderGrid::cellSize(int index)
{
	return 0;
}

const std::unordered_set<Collider*>& const SimpleECS::ColliderGrid::getCellContents(const int index)
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


