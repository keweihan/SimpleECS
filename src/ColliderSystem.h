#pragma once

#include "Collider.h"
#include "PhysicsBody.h"
#include "vector"

namespace SimpleECS
{
	/**
	 * Data container for collision information
	 */
	struct SIMPLEECS_API Collision {
		Collider* a = nullptr;
		Collider* b = nullptr;
		double penetration;
		Vector normal;
	};

	class ColliderSystem
	{
	public:
		/*
		* Register to list of colliders to check collisions for
		*/
		static void registerCollider(Collider* collider);

		/*
		* Deregister from list of colliders to check collisions for
		*/
		static void deregisterCollider(Collider* collider);
		
		/*
		* Checks for collisions between all active colliders and invoke
		* collided entities "onCollision" methods.
		*/
		static void invokeCollisions();

		/**
		 * Returns collision information between this and another collider.
		 *
		 * @returns nullptr if no collision is present.
		 */
		static Collision* getCollisionInfo(Collider* a, Collider *b);

	private:
		/*
		* Maintains list of all active colliders in scene. 
		*/
		static std::vector<Collider*> colliderList;
	};


}