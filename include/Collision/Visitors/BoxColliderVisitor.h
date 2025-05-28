#pragma once

#include "SimpleECSAPI.h"

#include "Collision/Visitors/ColliderVisitor.h"
#include <vector>
#include <iostream>

namespace SimpleECS
{
	// Collider class
	class SIMPLEECS_API BoxColliderVisitor : public ColliderVisitor {

	public:
		BoxColliderVisitor();
		~BoxColliderVisitor();

        void visitBox(Collision& out, BoxCollider* collider) override;
	};
}