#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "Collider.h"

#ifdef _TESTING
	#ifdef SIMPLEECS_EXPORTS
		#define SIMPLEECS_API __declspec(dllexport)
	#else
		#define SIMPLEECS_API __declspec(dllimport)
	#endif
#else
	#define SIMPLEECS_API // Leave it blank in release mode
#endif

namespace SimpleECS
{
	/*
	An list Collider storing structure providing the following:
	- Contiguous data storage and access
	- O(1) access
	- O(1) element insert
	- O(1) removal
	*/
	class SIMPLEECS_API EcsCell
	{
		class SIMPLEECS_API Iterator
		{
		public:
			friend EcsCell;

			Iterator(EcsCell* cell) : cellPtr(cell) {}
			Iterator(const EcsCell* cell) : cellPtr(cell) {}

			Iterator operator++(int);
			Iterator& operator++();
			Iterator& operator--();

			bool operator!=(const Iterator& other) const;
			bool operator==(const Iterator& other) const;
			bool operator<(const Iterator& other) const;

			Collider* operator*();

		private:
			const EcsCell* cellPtr;
			int index = 0;
		};

	public:
		EcsCell(int defaultSize);

		Iterator find(Collider* col);

		Iterator begin();

		Iterator back();

		Iterator end();

		Iterator begin() const;

		Iterator end() const;

		Iterator erase(Iterator o);

		Iterator erase(Collider* col);

		int size();
		
		void insert(Collider* col);

	private:
		// index of rightmost populated in colList
		int backIndex = 0;

		// first cell with value
		int beginIndex = 0;
		
		// sparse list of colliders in this cell 
		std::vector<Collider*> colList;

		// map of collider to index position in colList
		std::unordered_map<Collider*, int> colMap;

		// set of empty indices in colList 
		std::unordered_set<int> openIndices;

		int insertions = 0;
		int erasures = 0;
	};
}