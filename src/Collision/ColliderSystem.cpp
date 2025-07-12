#include "ColliderSystem.h"
#include "Collision/Collider.h"
#include "Collision/BoxCollider.h"
#include "Core/Component.h"
#include "Core/Entity.h"
#include "Utility/TransformUtil.h"
#include "boost/functional/hash.hpp"
#include <vector>
#include <thread>
#include <iostream>

using namespace SimpleECS;
using namespace UtilSimpleECS;

//------------------- Collision invocation ---------------------//

template<typename T1, typename T2>
struct PairHash {
	std::size_t operator()(const std::pair<T1, T2>& p) const {
		std::size_t seed = 0;
		boost::hash_combine(seed, boost::hash<T1>()(p.first));
		boost::hash_combine(seed, boost::hash<T2>()(p.second));
		return seed;
	}
};

inline void SimpleECS::ColliderSystem::invokeCollision(Collision& collision, Collider* a, Collider* b)
{
	collision.a = a;
	collision.b = b;

	// TODO: getComponents is an expensive operation. 
	if (ColliderSystem::getInstance().getCollisionInfo(collision)) {
		for (auto component : collision.a->entity->getComponents())
		{
			component->onCollide(*collision.b);
			component->onCollide(collision);
		}
	}
}

void SimpleECS::ColliderSystem::detectResolve()
{
	colliderGrid.updateGrid();
	Collision collision = {};

	// Set of potential collision pairs
	std::unordered_set<std::pair<Collider*, Collider*>, PairHash<Collider*, Collider*>>
		potentialPairs;

	// Populate with potential pairs
	try {
		for (int i = 0; i < colliderGrid.size(); ++i)
		{
			auto cell = *colliderGrid.getCellContents(i);
			for (auto iterA = cell.begin(); iterA != cell.end(); ++iterA)
			{
				for (auto iterB = iterA + 1; iterB != cell.end(); ++iterB)
				{
					potentialPairs.insert({ *iterA, *iterB });
				}
			}
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Exception occurred while populating potential pairs: " << e.what() << std::endl;
	}

	// Invoke onCollide of colliding entity components
	for (const auto& collisionPair : potentialPairs)
	{
		// Invoke from both sides
		invokeCollision(collision, collisionPair.first, collisionPair.second);
		invokeCollision(collision, collisionPair.second, collisionPair.first);
	}
}

bool SimpleECS::ColliderSystem::getCollisionInfo(Collision& collide)
{
	if (collide.a == nullptr || collide.b == nullptr) return false;
	
	// Use visitor pattern to resolve types
	return collide.b->accept(collide, collide.a); // i.e. CircleCollider

    return false;
}

