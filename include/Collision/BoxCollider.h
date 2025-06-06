#pragma once

#include "SimpleECSAPI.h"

#include "Core/Component.h"
#include "Collision/Collider.h"
#include "Collision/Visitors/BoxColliderVisitor.h"
#include <vector>
#include <memory>

namespace SimpleECS
{
	/**
	 * Box shaped implementation of Collider. 
	 */
	class SIMPLEECS_API BoxCollider : public Collider {

	public:
		BoxCollider() : Collider(), width(40), height(40), visitor(std::make_shared<BoxColliderVisitor>(this)) {}
		BoxCollider(int w, int h) : Collider(), width(w), height(h), visitor(std::make_shared<BoxColliderVisitor>(this)) {}

		void update() override;
		void initialize() override {}

		/**
		 * Returns whether this collider is colliding with another collider
		 */
		bool isColliding(Collider* other) override;

		void getBounds(AABB& bounds) const override;

		void accept(Collision& out, std::shared_ptr<SimpleECS::ColliderVisitor> visitor) override;

		std::shared_ptr<ColliderVisitor> getVisitor() override;

		/**
		 * Collider boundaries, centered around transform center.
		 */
		int width, height;
	private:
		AABB bound = {};
		std::shared_ptr<BoxColliderVisitor> visitor;
	};
}
