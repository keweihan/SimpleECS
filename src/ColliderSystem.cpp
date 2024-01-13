#include "ColliderSystem.h"
#include "Collider.h"
#include "Component.h"
#include "Entity.h"
#include "GameRenderer.h"
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

	grid.resize(r * c);
}

void SimpleECS::ColliderGrid::populateGrid(const std::vector<Collider*>& list)
{
	for (auto collide : list)
	{
		addCollider(collide);
	}
}

void SimpleECS::ColliderGrid::addCollider(const Collider*)
{

}

void SimpleECS::ColliderGrid::removeCollider(const Collider*&)
{
}

void SimpleECS::ColliderGrid::updateGrid()
{
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


