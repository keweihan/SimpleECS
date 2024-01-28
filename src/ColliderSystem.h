#pragma once

#include "Collider.h"
#include "PhysicsBody.h"
#include "vector"

namespace SimpleECS
{
	class ColliderGrid;

	// TODO: Class containing only static classes can be considered bad practice.
	// Consider alternatives.

	// Library only class for resolving collider logic
	class ColliderSystem
	{
	public:
		/*
		* Constants for grid size
		*/
		static const int GRID_COLUMNS = 60;
		static const int GRID_ROWS = 40;

		/*
		* Register to list of colliders to check collisions for
		*/
		static void registerCollider(Collider* collider);

		/*
		* Deregister from list of colliders to check collisions for
		*/
		static void deregisterCollider(Collider* collider);
		
		/*
		* Checks for collisions between all active colliders and invoke
		* collided entities "onCollision" methods.
		*/
		static void invokeCollisions();

		/**
		 * Retrieves collision information between this and another collider.
		 * Populates collide with collision information if there is a collision
		 * between collide.a and collide.b 
		 * 
		 * Normal is calculated with respect to a colliding with b. 
		 * 
		 * @returns false if no collision is present, true otherwise
		 */
		static bool getCollisionInfo(Collision& collide);

	private:
		/*
		* Maintains list of all active colliders in scene. 
		*/
		static std::vector<Collider*> colliderList;

		/*
		*
 		*/
		static ColliderGrid colliderGrid;

		/*
		* If collide contains two AABB box containers. Populate with collision data
		*/
		static bool getCollisionBoxBox(Collision& collide);
	};

	
	/*
	* Define a spacial grid to segment possible colliding objects
	*/
	class ColliderGrid
	{
	public: 
		/* 
		* Construct grid 
		*/
		ColliderGrid(const int r, const int c);

		/*
		* Add a collider to this grid
		*/
		void registerCollider(Collider*);

		/*
		* Remove a collider from this grid
		*/
		void removeCollider(Collider*);

		/*
		* Update collider grid positions
		*/
		void updateGrid();

		/*
		* Get number of cells
		*/
		int size();

		/*
		* Get the number of colliders in a given cell
		*/
		int cellSize(int index);

		/*
		* Get the colliders populating a given cell
		*/
		const std::unordered_set<Collider*>& const getCellContents(const int index);

		/*
		* Get the colliders populating single out of bounds cell
		*/
		const std::unordered_set<Collider*>& const getOutBoundContent();

		/*
		* Get the bounds of a given cell
		*/
		void getCellBounds(Collider::AABB& output, const int& index);

	private:
		/*
		* Add a collider to this grid
		*/
		void insertToGrid(Collider*);

		/*
		* Populate grid based on internal list of colliders.
		*/
		void populateGrid();

		int cellWidth, cellHeight;
		int numRow, numColumn;

		// TODO: change to vector of CELLS. Define cells separately using contiguous memory.
		// index x = c + r * numColumn;
		std::vector<std::unordered_set<Collider*>> grid;

		std::unordered_set<Collider*> outbounds;

		// list of all colliders
		std::unordered_set<Collider*> colliderList;
	};
}