#pragma once
#include "Collider.h"
#include <vector>
#include <unordered_set>

namespace SimpleECS
{
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
