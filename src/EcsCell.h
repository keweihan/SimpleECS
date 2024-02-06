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
	using EcsCellIterator = std::vector<Collider*>::iterator;

	/*
	An list Collider storing structure providing the following:
	- Contiguous unordered data storage and traversal
	- O(1) access
	- O(1) element insert
	- O(1) removal
	*/
	class SIMPLEECS_API EcsCell
	{
	public:
		EcsCell(const EcsCell& other);
		EcsCell(int defaultSize);
		EcsCell();
		~EcsCell();

		EcsCell& operator=(const EcsCell& other);

		EcsCellIterator find(Collider* col);

		EcsCellIterator begin();

		EcsCellIterator end();

		EcsCellIterator begin() const;

		EcsCellIterator end() const;

		EcsCellIterator erase(EcsCellIterator o);

		EcsCellIterator erase(Collider* col);

		Collider* back();

		int size();
		
		void insert(Collider* col);

	private:
		class EcsCellImpl;
		std::unique_ptr<EcsCellImpl> pImpl;
	};
}