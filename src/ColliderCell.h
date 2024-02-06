#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include "Collider.h"
#include "boost/container/small_vector.hpp"

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
	using ColliderCellIterator = boost::container::small_vector<Collider*, 3>::iterator;
	using ColliderConstCellIterator = boost::container::small_vector<Collider*, 3>::const_iterator;

	/*
	An list Collider storing structure providing the following:
	- Contiguous unordered data storage and traversal
	- O(1) access
	- O(1) element insert
	- O(1) removal
	*/
	class SIMPLEECS_API ColliderCell
	{
	public:
		ColliderCell(const ColliderCell& other);
		ColliderCell(int defaultSize);
		ColliderCell();
		~ColliderCell();

		ColliderCell& operator=(const ColliderCell& other);

		ColliderCellIterator find(Collider* col);

		ColliderCellIterator begin();

		ColliderCellIterator end();

		ColliderConstCellIterator begin() const;

		ColliderConstCellIterator end() const;

		ColliderCellIterator erase(ColliderCellIterator o);

		ColliderCellIterator erase(Collider* col);

		Collider* back();

		int size();
		
		void insert(Collider* col);

	private:
		boost::container::small_vector<Collider*, 3> colList;
	};
}