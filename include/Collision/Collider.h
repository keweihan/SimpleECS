#pragma once

#include "SimpleECSAPI.h"

#include "Core/Component.h"
#include "Visitors/ColliderVisitor.h"
#include "Collision/Collision.h"
#include <vector>
#include <iostream>
#include <memory> 

namespace SimpleECS
{
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
		virtual void accept(Collision& out, std::shared_ptr<ColliderVisitor> visitor) = 0;

		/**
		 * Return visitor object for this Collider type
		 */
		virtual std::shared_ptr<ColliderVisitor> getVisitor() = 0; 
	};
}
