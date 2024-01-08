#include "ColliderSystem.h"
#include "Collider.h"
#include "Component.h"
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
		for (int j = 0; j < colliderList.size(); ++j)
		{
			if (j == i) continue;

			// TODO: constructors need not be called for every possible collision.
			Collision* collide = new Collision{ colliderList[i], colliderList[j], 0, Vector()};

			if (getCollisionInfo(collide))
			{
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

bool SimpleECS::ColliderSystem::getCollisionInfo(Collision* collide)
{
	if (collide->a == nullptr || collide->b == nullptr) return false;

	Transform aTransform = collide->a->entity->transform;
	Transform bTransform = collide->b->entity->transform;

    BoxCollider* aBox = dynamic_cast<BoxCollider*>(collide->a);
    BoxCollider* bBox = dynamic_cast<BoxCollider*>(collide->b);

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
            return false;
        }

		// Boxes are colliding. Find axis of least penetration
		double aExtentX = (aRight - aLeft) / 2;
		double bExtentX = (bRight - bLeft) / 2;
		double xDistance = std::abs(collide->a->entity->transform.posX - collide->b->entity->transform.posX);
		double xOverlap = (aExtentX + bExtentX) - xDistance;

		double aExtentY = (aRight - aLeft) / 2;
		double bExtentY = (bRight - bLeft) / 2;
		double yDistance = std::abs(collide->a->entity->transform.posY - collide->b->entity->transform.posY);
		double yOverlap = (aExtentY + bExtentY) - yDistance;

		if (yOverlap < xOverlap)
		{
			collide->penetration = yOverlap;
			if (aTransform.posY < bTransform.posY)
			{
				collide->normal = Vector(0, -1);
			}
			else
			{
				collide->normal = Vector(0, 1);
			}
		}
		else
		{
			collide->penetration = xOverlap;
			if (aTransform.posX < bTransform.posX)
			{
				collide->normal = Vector(-1, 0);
			}
			else
			{
				collide->normal = Vector(1, 0);
			}
		}
    }
    // Other collider types here
	// else if(...)

    return true;
}
