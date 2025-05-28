#pragma once

#include "SimpleECSAPI.h"

#include "Collision/Collision.h"
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

        /// Resolve collision with a box
        /// @param collider 
        virtual void visitBox(Collision& out, BoxCollider* collider) = 0;
        
		// virtual void visitCircle(void* collider) = 0; // stub - for future collider types
	};
}