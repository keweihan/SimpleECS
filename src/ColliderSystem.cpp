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

//------------------- Collision invocation ---------------------//

// Custom hash functions for pair of colliders
template<typename T>
void hashCombine(std::size_t& seed, T const& key) {
	// TODO: somewhat arbitrary from stackoverflow. 
	// https://stackoverflow.com/questions/28367913/how-to-stdhash-an-unordered-stdpair
	// Run testing for potential better hashing?
	std::hash<T> hasher;
	seed ^= hasher(key) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template<typename T1, typename T2>
struct PairHash {
	std::size_t operator()(std::pair<T1, T2> const& p) const {
		std::size_t seed(0);
		::hashCombine(seed, p.first);
		::hashCombine(seed, p.second);

		return seed;
	}
};

void SimpleECS::ColliderSystem::invokeCollisions()
{
	colliderGrid.updateGrid();
	Collision collision = {};

	// Set of potential collision pairs
	// NOTE: This is populated by potential pairs to collider INTENTIONALLY WITH ORDERING
	// I.e. (c1, c2) and (c2, c1) are considered different pairs as "onCollision" calls
	// themselves are ordered and resolved in the perspective of the object itself.
	std::unordered_set<std::pair<Collider*, Collider*>, PairHash<Collider*, Collider*>>
		potentialPairs;

	// Populate with potential pairs from main scene
	for (int i = 0; i < colliderGrid.size(); ++i)
	{
		// TODO: Idea - instead of iterating through all, change potentialPairs to be unique
		// by order so iteration will only insert unique ordered pairs. This needs getCellContents
		// to return an ORDERED data structure. This also has benefits of using a structure that 
		// iterates in contiguous memory.

		auto cell = colliderGrid.getCellContents(i);
		for (auto iterA = cell->begin(); iterA != cell->end(); ++iterA)
		{
			for ( auto iterB = iterA; iterB != cell->end(); ++iterB )
			{
				potentialPairs.insert({ *iterA, *iterB });
			}
		}
	}

	// Populate with potential pairs from out of bounds.
	for (const auto& colliderA : *colliderGrid.getOutBoundContent())
	{
		for (const auto& colliderB : *colliderGrid.getOutBoundContent())
		{
			potentialPairs.insert({ colliderA, colliderB });
		}
	}

	// Invoke onCollide of colliding entity components
	for (const auto& collisionPair : potentialPairs)
	{
		if (collisionPair.first != collisionPair.second) 
		{
			// Only invoke one sided, as potentialPairs has symmetric pairs.
			collision.a = collisionPair.first;
			collision.b = collisionPair.second;
			if (getCollisionInfo(collision)) {
				for (auto component : collision.a->entity->getComponents())
				{
					component->onCollide(*collision.b);
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

