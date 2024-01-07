#pragma once

#include "Collider.h"
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

	private:
		/*
		* Maintains list of all active colliders in scene. 
		*/
		static std::vector<Collider*> colliderList;
	};
}