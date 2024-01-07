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

void SimpleECS::ColliderSystem::resolveCollisions()
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
