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
	class SIMPLEECS_API EcsCellIterator;

	/*
	An list Collider storing structure providing the following:
	- Contiguous data storage and access
	- O(1) access
	- O(1) element insert
	- O(1) removal
	*/
	class SIMPLEECS_API EcsCell
	{
	public:
		friend EcsCellIterator;

		EcsCell(const EcsCell& other);
		EcsCell(int defaultSize);
		EcsCell();
		~EcsCell();

		EcsCell& operator=(const EcsCell& other);

		EcsCellIterator find(Collider* col);

		EcsCellIterator begin();

		EcsCellIterator back();

		EcsCellIterator end();

		EcsCellIterator begin() const;

		EcsCellIterator end() const;

		EcsCellIterator erase(EcsCellIterator o);

		EcsCellIterator erase(Collider* col);

		int size();
		
		void insert(Collider* col);

	private:
		class EcsCellImpl;
		std::shared_ptr<EcsCellImpl> pImpl;
	};

	class SIMPLEECS_API EcsCellIterator
	{
	public:
		friend EcsCell;

		EcsCellIterator(EcsCell* cell) : cellPtr(cell) {}
		EcsCellIterator(const EcsCell* cell) : cellPtr(cell) {}

		EcsCellIterator operator++(int);
		EcsCellIterator& operator++();
		EcsCellIterator& operator--();

		bool operator!=(const EcsCellIterator& other) const;
		bool operator==(const EcsCellIterator& other) const;
		bool operator<(const EcsCellIterator& other) const;

		Collider* operator*();

	private:
		const EcsCell* cellPtr;
		int index = 0;
	};
}