#include "BoxCollider.h"
#include "PhysicsBody.h"
#include "ColliderSystem.h"
#include "Entity.h"

using namespace SimpleECS;

bool BoxCollider::isColliding(Collider* other)
{
    return ColliderSystem::getCollisionInfo(this, other) != nullptr;
}