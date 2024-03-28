#pragma once
#include "BoxCollider.h"
#include "Collider.h"
#include "ColliderCell.h"
#include <vector>

namespace SimpleECS
{
	/*
	* Define a spacial grid to segment possible colliding objects
	*/
	class ColliderGrid
	{
	public:
		/*
		* Construct grid with given cell dimensions
		*/
		ColliderGrid(const int w, const int h);

		/*
		* Update collider grid positions
		*/
		void updateGrid();

		/*
		* Get number of cells
		*/
		int size() const;

		/*
		* Get the number of colliders in a given cell
		*/
		int cellSize(int index);

		/*
		* Get the colliders populating a given cell. Input grid.size() - 1 to get out of bounds.
		*/
		const ColliderCell* getCellContents(const int index) const;

		/*
		* Get the colliders populating single out of bounds cell
		*/
		const ColliderCell* getOutBoundContent() const;

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

		// Grid parameters
		int cellWidth, cellHeight;
		int numRow, numColumn;

		/* 
		* Spatial collider storage
		* index x = c + r * numColumn
		* (c * r + 1)th cell contains out of bounds colliders
		*/
		std::vector<ColliderCell> grid;

		// list of all active colliders
		//std::vector<Collider*> colliderList;

		// 
		// ference to boxCollider pool
		std::vector<BoxCollider>* boxPool;
	};
}
