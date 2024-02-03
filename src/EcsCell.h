#pragma once
#include <vector>
#include "Collider.h"

namespace SimpleECS
{
	/*
	An list data type providing the following:
	- Contiguous data storage
	- O(1) access
	- O(1) element insert
	- O(1) removal
	*/
	class EcsCell
	{
		class Iterator
		{
			Iterator(EcsCell* cell) : ptr(cell) {}

			Iterator& operator++();
			Iterator& operator--();

			bool operator!=(const Iterator& other);
			bool operator<(const Iterator& other);

			Collider& operator*();

		private:
			EcsCell* ptr;
			int index = 0;
		};

		EcsCell(int defaultSize);

		Iterator begin();

		Iterator end();

		Iterator erase(Iterator& o);

	private:
		std::vector<Collider> list;
	};
}