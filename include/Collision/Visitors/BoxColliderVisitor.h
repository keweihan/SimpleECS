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
		BoxColliderVisitor(BoxCollider* parent) : parentCollide(parent) {};
		~BoxColliderVisitor() {};

        void visitBox(Collision& out, BoxCollider* collider) override;
    
    private:
        BoxCollider* parentCollide;
	};
}