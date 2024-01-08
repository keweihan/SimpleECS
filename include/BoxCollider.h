#pragma once
#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H

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
	struct Collision; // forward declaration

	// Collider class
	class SIMPLEECS_API BoxCollider : public Collider {

	public:
		BoxCollider() : Collider(), width(40), height(40) {}
		BoxCollider(int w, int h) : Collider(), width(w), height(h) {}

		/**
		 * Collider size, centered around transform center. 
		 */
		int width, height;

		void update() override {}
		void initialize() override {}

		/**
		 * Returns whether this collider is colliding with another collider
		 */
		bool isColliding(Collider* other) override;
	};
}

#endif
