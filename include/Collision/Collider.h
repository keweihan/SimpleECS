#pragma once

#include "SimpleECSAPI.h"

#include "Core/Component.h"
#include "Collision/Collision.h"
#include <vector>
#include <iostream>

namespace SimpleECS
{
	class BoxCollider;

	// Collider class
	class SIMPLEECS_API Collider : public Component {

	public:
		struct AABB {
			double xMin, yMin;
			double xMax, yMax;
		};

		/**
		 * Register and deregister collider against ColliderSystem
		 * on construction/deconstruction
		 */
		Collider() ;
		~Collider();

		void update() override {}
		void initialize() override { std::cout << "wtf" << std::endl; }

		/**
		 * Returns whether this collider is colliding with another collider
		 */
		virtual bool isColliding(Collider* other) = 0;

		/**
		 * Gets AABB bounds of this collider
		 */
		virtual void getBounds(AABB& bounds) const = 0;

		/**
		 * Accept visitor to allow for type specific processing
		 */
		virtual bool accept(Collision& out, Collider* visitor) = 0;

		/**
		 * Resolve collision between this collider and a box
		 */
		virtual bool visitBox(Collision& out, BoxCollider* box) = 0;
	};
}
