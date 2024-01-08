#pragma once

#include "Collider.h"
#include "PhysicsBody.h"
#include "vector"

namespace SimpleECS
{
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
		 * Retrieves collision information between this and another collider.
		 * Populates collide with collision information if there is a collision
		 * between collide.a and collide.b 
		 * 
		 * Normal is calculated with respect to a colliding with b. 
		 * 
		 * @returns false if no collision is present, true otherwise
		 */
		static bool getCollisionInfo(Collision* collide);

	private:
		/*
		* Maintains list of all active colliders in scene. 
		*/
		static std::vector<Collider*> colliderList;
	};


}