#pragma once

#include "Collision/Collider.h"
#include "ColliderGrid.h"
#include "Physics/PhysicsBody.h"
#include "vector"

namespace SimpleECS
{
	// Library only controller level class for resolving collider logic
	class ColliderSystem
	{
	public:
		/*
		* 
		*/
		static ColliderSystem& getInstance()
		{
			static ColliderSystem instance; // Guaranteed to be destroyed.
			// Instantiated on first use.
			return instance;
		}

		// Singleton class
		ColliderSystem(ColliderSystem const&) = delete;
		void operator=(ColliderSystem const&) = delete;
		
		/*
		* Checks for collisions between all active colliders and invoke
		* collided entities "onCollision" methods.
		*/
		void detectResolve();

	private:
		ColliderSystem() : colliderGrid(ColliderGrid(2, 2)) {
			auto boxPool = Game::getInstance().getCurrentScene()->getComponents<BoxCollider>();
			colliderGrid.registerColliders(new ColliderPoolView<BoxCollider>(boxPool));
		}

		/**
		 * Retrieves collision information between this and another collider.
		 * Populates collide with collision information if there is a collision
		 * between collide.a and collide.b 
		 * 
		 * Normal is calculated with respect to a colliding with b. 
		 * 
		 * @returns false if no collision is present, true otherwise
		 * 
		 * TODO: move logic to concrete Colliders (visitor pattern)
		 */
		bool getCollisionInfo(Collision& collide);

		/**
		* If collide contains two AABB box containers. Populate with collision data
		*
		* TODO: move logic to BoxCollider (visitor pattern)
		*/
		bool getCollisionBoxBox(Collision& collide, BoxCollider* a, BoxCollider* b);

		/*
		* Invoke onCollide functions of collider a
		*/
		void invokeCollision(Collision& collision, Collider* a, Collider* other);

		/*
		* Spatial structure
		*/
		ColliderGrid colliderGrid;
	};
}