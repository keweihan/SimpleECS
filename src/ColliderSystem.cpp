#include "ColliderSystem.h"
#include "Collider.h"
#include "Entity.h"
#include <vector>
using namespace SimpleECS;

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
	for (int i = 0; i < colliderList.size(); ++i)
	{
		for (int j = i + 1; j < colliderList.size(); ++j)
		{
			if (colliderList[i]->isColliding(colliderList[j]))
			{
				// Invoke onCollide of colliding entity components
				for (auto component : colliderList[i]->entity->getComponents())
				{
					component->onCollide(*colliderList[j]);
				}

				for (auto component : colliderList[j]->entity->getComponents())
				{
					component->onCollide(*colliderList[i]);
				}
			}
		}
	}

}

Collision* SimpleECS::ColliderSystem::getCollisionInfo(Collider* a, Collider* b)
{
    BoxCollider* aBox = dynamic_cast<BoxCollider*>(a);
    BoxCollider* bBox = dynamic_cast<BoxCollider*>(b);

    if (bBox != nullptr && aBox != nullptr)
    {
        // AABB Collision 
        double aLeft, aRight, aTop, aBottom;
        double bLeft, bRight, bTop, bBottom;

        aLeft = aBox->entity->transform.posX - aBox->width / 2;
        aRight = aBox->entity->transform.posX + aBox->width / 2;
        aBottom = aBox->entity->transform.posY - aBox->height / 2;
        aTop = aBox->entity->transform.posY + aBox-> height / 2;

        bLeft = bBox->entity->transform.posX - bBox->width / 2;
        bRight = bBox->entity->transform.posX + bBox->width / 2;
        bBottom = bBox->entity->transform.posY - bBox->height / 2;
        bTop = bBox->entity->transform.posY + bBox->height / 2;

        //If any of the sides from A are outside of B, no collision occuring.
        if (aBottom >= bTop || aTop <= bBottom || aRight <= bLeft || aLeft >= bRight)
        {
            return nullptr;
        }

		// Boxes are colliding. Find axis of least penetration
		double aExtentX = (aRight - aLeft) / 2;
		double bExtentX = (bRight - bLeft) / 2;
		double xDistance = std::abs(a->entity->transform.posX - b->entity->transform.posX);
		double xOverlap = (aExtentX + bExtentX) - xDistance;

		double aExtentY = (aRight - aLeft) / 2;
		double bExtentY = (bRight - bLeft) / 2;
		double yDistance = std::abs(a->entity->transform.posY - b->entity->transform.posY);
		double yOverlap = (aExtentY + bExtentY) - yDistance;

		if (yOverlap < xOverlap)
		{
			if (a->entity->transform.posY < b->entity->transform.posY)
			{
				return new Collision{ a, b, 0, Vector(0, -1) };
			}
			else
			{
				return new Collision{ a, b, 0, Vector(0, 1) };
			}
		}
		else
		{
			if (a->entity->transform.posX < b->entity->transform.posX)
			{
				return new Collision{ a, b, 0, Vector(-1, 0) };
			}
			else
			{
				return new Collision{ a, b, 0, Vector(1, 0) };
			}
		}
    }
    // Other collider types here

    return nullptr;
}
