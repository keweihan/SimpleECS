#include "SimpleECS.h"
#include <gtest/gtest.h>
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
		// Create entity a
		Entity* a = testScene->createEntity();
		a->addComponent<PhysicsBody>();
		a->getComponent<PhysicsBody>()->velocity.x = 1;
		ASSERT_EQ(a->getComponent<PhysicsBody>()->velocity.x, 1);
		
		// Mark A for deletion
		testScene->destroyEntity(a->id);

		// Create entity b
		Entity* b = testScene->createEntity();
		b->addComponent<PhysicsBody>();
		b->getComponent<PhysicsBody>()->velocity.x = 2;

		// Entity b and a components should have correct values
		ASSERT_EQ(b->getComponent<PhysicsBody>()->velocity.x, 2);
		ASSERT_EQ(a->getComponent<PhysicsBody>()->velocity.x, 1);
		
		// Create entity c and delete a immediately
		Entity* c = testScene->createEntity();
		c->addComponent<PhysicsBody>();
		c->getComponent<PhysicsBody>()->velocity.x = 3;
		testScene->destroyEntityImmediate(a->id);

		// Assert no side effects on b
		ASSERT_EQ(b->getComponent<PhysicsBody>()->velocity.x, 2);
	}
}