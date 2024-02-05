#pragma once
#include "Collider.h"
#include <vector>
#include <unordered_set>
#include "boost/container/flat_set.hpp"
#include "boost/container/small_vector.hpp"

namespace SimpleECS
{
	/*
	* Boost structure for faster iteration.
	* 
	* Reduces bottleneck of collider iteration. Improvement over unordered_set due to
	* contiguous memory storage, frequent traversal of colliders and usually small number
	* of colliders per cell. 
	* 
	* Needs testing verification of performance benefits. 
	* 
	* Idea from: 
	* https://stackoverflow.com/questions/24506789/set-vs-unordered-set-for-fastest-iteration
	* Reference:
	* https://www.boost.org/doc/libs/1_73_0/doc/html/boost/container/flat_set.html
	*/
	using EcsCell = boost::container::flat_set<
		Collider*,
		std::less<Collider*>,
		boost::container::small_vector<Collider*, 10>
	>;

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
		const EcsCell& const getCellContents(const int index);

		/*
		* Get the colliders populating single out of bounds cell
		*/
		const EcsCell& const getOutBoundContent();

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

		/*
		* Spatial grid of cells containing colliders
		*/
		std::vector<EcsCell> grid;

		/*
		* Single cell representing out of bounds colliders
		*/
		EcsCell outbounds;

		// list of all colliders
		std::unordered_set<Collider*> colliderList;
	};
}
