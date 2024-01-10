#pragma once

#ifdef SIMPLEECS_EXPORTS
#define SIMPLEECS_API __declspec(dllexport)
#else
#define SIMPLEECS_API __declspec(dllimport)
#endif

#include "Component.h"
#include "Vector.h"
#include <vector>

namespace SimpleECS
{
	// Collider class
	class SIMPLEECS_API Collider : public Component {

	public:
		/**
		 * Register and deregister collider against ColliderSystem
		 * on construction/deconstruction
		 */
		Collider();
		~Collider();

		void update() override {}
		void initialize() override {}

		/**
		 * Returns whether this collider is colliding with another collider
		 */
		virtual bool isColliding(Collider* other) = 0;
	};

	/**
	* Data container for collision information
	*/
	class SIMPLEECS_API Collision {
	public:
		Collider* a = nullptr;
		Collider* b = nullptr;
		double penetration = 0;
		Vector normal;
	};
}
