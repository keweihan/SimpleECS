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
		double aLeft, aRight, aTop, aBottom;
		double bLeft, bRight, bTop, bBottom;

		double aExtentX = aBox->width / 2;
		double bExtentX = bBox->width / 2;
		double aExtentY = aBox->height / 2;
		double bExtentY = bBox->height / 2;


		aLeft = aBox->entity->transform.position.x - aExtentX;
		aRight = aBox->entity->transform.position.x + aExtentX;
		aBottom = aBox->entity->transform.position.y - aExtentY;
		aTop = aBox->entity->transform.position.y + aExtentY;

		bLeft = bBox->entity->transform.position.x - bExtentX;
		bRight = bBox->entity->transform.position.x + bExtentX;
		bBottom = bBox->entity->transform.position.y - bExtentY;
		bTop = bBox->entity->transform.position.y + bExtentY;

		//If any of the sides from A are outside of B, no collision occuring.
		if (aBottom >= bTop || aTop <= bBottom || aRight <= bLeft || aLeft >= bRight)
		{
			return false;
		}

		// Boxes are colliding. Find axis of least penetration
		double xDistance = std::abs(collide->a->entity->transform.position.x - collide->b->entity->transform.position.x);
		double xOverlap = (aExtentX + bExtentX) - xDistance;

		double yDistance = std::abs(collide->a->entity->transform.position.y - collide->b->entity->transform.position.y);
		double yOverlap = (aExtentY + bExtentY) - yDistance;

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

