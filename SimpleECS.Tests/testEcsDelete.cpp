#pragma once

#include "SimpleECS.h"
#include "gtest/gtest.h"
#include "Collision/ColliderCell.h"
#include "boost/container/small_vector.hpp"
#include <vector>

using namespace SimpleECS;
using namespace std;

namespace EcsDeleteTests {

	// Test fixture. See https://google.github.io/googletest/primer.html#same-data-multiple-tests
	class DeleteEntityTest : public testing::Test {
	protected:
		void SetUp() override {
			testScene = new Scene(Color(0, 0, 0, 255));
		}
		Scene* testScene;
	};

	/*
	* Test basic entity delete
	*/
	TEST_F(DeleteEntityTest, DeleteHandleGeneric) {
		Entity* a = testScene->createEntity();
		a->addComponent<PhysicsBody>();
		a->getComponent<PhysicsBody>()->velocity.x = 1;

		ASSERT_EQ(a->getComponent<PhysicsBody>()->velocity.x, 1);
		
		testScene->destroyEntity(a->id);

		Entity* b = testScene->createEntity();
		b->addComponent<PhysicsBody>();
		b->getComponent<PhysicsBody>()->velocity.x = 2;

		ASSERT_EQ(b->getComponent<PhysicsBody>()->velocity.x, 2);
		ASSERT_EQ(a->getComponent<PhysicsBody>()->velocity.x, 1);

		Entity* c = testScene->createEntity();
		c->addComponent<PhysicsBody>();
		c->getComponent<PhysicsBody>()->velocity.x = 3;
		testScene->destroyEntityImmediate(a->id);

		ASSERT_EQ(b->getComponent<PhysicsBody>()->velocity.x, 2);
	}
}