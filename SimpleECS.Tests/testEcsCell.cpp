#pragma once

#include "SimpleECS.h"
#include "gtest/gtest.h"
#include "Collision/ColliderCell.h"
#include "boost/container/small_vector.hpp"
#include <vector>

using namespace SimpleECS;
using namespace std;

namespace EcsCellTests {

	// Test fixture. See https://google.github.io/googletest/primer.html#same-data-multiple-tests
	class ColliderCellTest : public testing::Test {
	protected:
		void SetUp() override {
			// Create hundred cell
			for (int i = 0; i < 100; ++i)
			{
				hundredCols.push_back(new BoxCollider());
				hundredCell.insert(hundredCols.back());
			}
			
			// Create fiveCell
			for (int i = 0; i < 5; ++i)
			{
				cols.push_back(new BoxCollider());
				fiveCell.insert(cols.back());
			}

			// Create oneCell
			col = new BoxCollider();
			oneCell.insert(col);
		}

		void TearDown() override {
			// Cleanup
			for (int i = 0; i < hundredCols.size(); ++i)
			{
				delete hundredCols[i];
			}

			for (int i = 0; i < cols.size(); ++i)
			{
				delete cols[i];
			}

			delete col;
		}

		ColliderCell hundredCell = ColliderCell(10);
		vector<BoxCollider*> hundredCols;

		ColliderCell fiveCell = ColliderCell(10);
		vector<BoxCollider*> cols;
		
		ColliderCell oneCell  = ColliderCell(10);
		BoxCollider* col = nullptr;

		ColliderCell zeroCell = ColliderCell(10);
	public:

		ColliderCellTest() = default;
	};

	/* 
	* Test basic cell size
	*/
	TEST_F(ColliderCellTest, SizeGeneric) {
		// Cell size should be 5
		EXPECT_EQ(fiveCell.size(), 5);
	}

	/* 
	* Test correct cell size after erase
	*/
	TEST_F(ColliderCellTest, SizeErase) {
		// Cell size should be 5
		fiveCell.erase(cols[1]);
		EXPECT_EQ(fiveCell.size(), 4);
		
		fiveCell.erase(cols[1]);
		EXPECT_EQ(fiveCell.size(), 3);
		
		fiveCell.erase(cols[4]);
		EXPECT_EQ(fiveCell.size(), 2);
		
		fiveCell.erase(cols[0]);
		EXPECT_EQ(fiveCell.size(), 1);

		fiveCell.erase(cols[3]);
		EXPECT_EQ(fiveCell.size(), 0);
	}

	/* 
	* Test correct number of iterations
	*/
	TEST_F(ColliderCellTest, RangeIter) {
		// Cell size should be 5
		int iterCount = 0;
		for (auto i : fiveCell)
		{
			iterCount++;
		}
		EXPECT_EQ(iterCount, 5);
	}

	/* 
	* Test correct number of iterations after erase
	*/
	TEST_F(ColliderCellTest, RangeIterErase) {
		fiveCell.erase(cols[3]);
		EXPECT_EQ(fiveCell.size(), 4);

		int iterCount = 0;
		for (auto i : fiveCell)
		{
			iterCount++;
		}
		EXPECT_EQ(iterCount, 4);
	}

	/*
	* Test correct number of iterations after erase
	*/
	TEST_F(ColliderCellTest, RangeIterEraseMultiple) {
		fiveCell.erase(cols[3]);
		fiveCell.erase(cols[4]);
		fiveCell.erase(cols[2]);
		EXPECT_EQ(fiveCell.size(), 2);

		int iterCount = 0;
		for (auto i : fiveCell)
		{
			iterCount++;
		}
		EXPECT_EQ(iterCount, 2);
	}

	/*
	* Test correct erase return after erasing first
	*/
	TEST_F(ColliderCellTest, IterEraseFirst) {
		auto valid = fiveCell.erase(fiveCell.begin());
		EXPECT_EQ(fiveCell.begin(), valid);
	}

	/*
	* Test correct erase return after erasing last by value
	*/
	TEST_F(ColliderCellTest, IterEraseLast) {
		fiveCell.erase(cols[4]);
		auto end = fiveCell.find(cols[3]);
		++end;
		EXPECT_EQ(fiveCell.end(), end);
		EXPECT_EQ(fiveCell.size(), 4);
	}

	/*
	* Test correct erase return after erasing one sized array
	*/
	TEST_F(ColliderCellTest, IterEraseOne) {
		auto valid = oneCell.erase(oneCell.begin());
		EXPECT_EQ(oneCell.end(), valid);
		EXPECT_EQ(oneCell.begin(), oneCell.end());
		EXPECT_EQ(oneCell.size(), 0);
	}

	/*
	* Tests general case with manual iteration with erase
	*/
	TEST_F(ColliderCellTest, IterGeneral) {		
		
		// Remove items from cell
		int expectedSize = 100;
		EXPECT_EQ(hundredCell.size(), expectedSize);
		for (auto iter = hundredCell.begin(); iter != hundredCell.end();)
		{
			// Randomly erase
			if (int(*iter) % 3 == 0)
			{
				expectedSize--;
				iter = hundredCell.erase(iter);
			}
			else
			{
				++iter;
			}
		}
		EXPECT_EQ(hundredCell.size(), expectedSize);

		// Iterate through sparser cell
		int iterCount = 0;
		for (auto iter = hundredCell.begin(); iter != hundredCell.end(); ++iter)
		{
			iterCount++;
		}
		EXPECT_EQ(iterCount, expectedSize);

		// Reinsert elements to cell
		vector<BoxCollider*> newCol;
		for (int i = 0; i < 10; ++i)
		{
			expectedSize++;
			newCol.push_back(new BoxCollider());
			hundredCell.insert(newCol.back());
		}
		EXPECT_EQ(hundredCell.size(), expectedSize);

		// Iterate through reinserted cell cell
		iterCount = 0;
		for (auto iter = hundredCell.begin(); iter != hundredCell.end(); ++iter)
		{
			iterCount++;
		}
		EXPECT_EQ(iterCount, expectedSize);

		// Cleanup
		for (auto col : newCol) { delete col; }
	}
}