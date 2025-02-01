#pragma once

#include "SimpleECSAPI.h"

#include <vector>
#include <iostream>

namespace SimpleECS
{
	class BoxCollider;
	
	// Collider class
	class SIMPLEECS_API ColliderVisitor {

	public:
		ColliderVisitor();
		~ColliderVisitor();

        virtual void visit(BoxCollider* collider) = 0;
        // virtual void visit(void* collider) = 0; // stub - for future collider types
	};
}