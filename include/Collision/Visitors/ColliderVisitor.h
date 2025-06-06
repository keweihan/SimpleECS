#pragma once

#include "SimpleECSAPI.h"

#include "Collision/Collision.h"
#include <vector>
#include <iostream>

namespace SimpleECS
{
	class BoxCollider;
	class Collider;
	
	// Collider class
	class SIMPLEECS_API ColliderVisitor {

	public:
		ColliderVisitor() {};
		~ColliderVisitor() {};

        /// Resolve collision with a box
        /// @param collider 
        virtual bool visitBox(Collision& out) = 0;
        
		// virtual void visitCircle(void* collider) = 0; // stub - for future collider types
	};
}