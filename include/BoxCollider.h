#pragma once

#ifdef SIMPLEECS_EXPORTS
#define SIMPLEECS_API __declspec(dllexport)
#else
#define SIMPLEECS_API __declspec(dllimport)
#endif

#include "Component.h"
#include "Collider.h"
#include <vector>

namespace SimpleECS
{
	/**
	 * Box shaped implementation of Collider. 
	 */
	class SIMPLEECS_API BoxCollider : public Collider {

	public:
		BoxCollider() : Collider(), width(40), height(40) {}
		BoxCollider(int w, int h) : Collider(), width(w), height(h) {}

		void update() override;
		void initialize() override {}

		/**
		 * Returns whether this collider is colliding with another collider
		 */
		bool isColliding(Collider* other) override;

		void getBounds(AABB& bounds) const override;

		/**
		 * Collider boundaries, centered around transform center.
		 */
		int width, height;
	private:
		AABB bound = {};
	};
}
