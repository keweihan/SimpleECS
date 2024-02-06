#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
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
	using ColliderCellIterator = std::vector<Collider*>::iterator;

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

		ColliderCellIterator begin() const;

		ColliderCellIterator end() const;

		ColliderCellIterator erase(ColliderCellIterator o);

		ColliderCellIterator erase(Collider* col);

		Collider* back();

		int size();
		
		void insert(Collider* col);

	private:
		class ColliderCellImpl;
		std::unique_ptr<ColliderCellImpl> pImpl;
	};
}