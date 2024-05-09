#include "BoxCollider.h"
#include "PhysicsBody.h"
#include "ColliderSystem.h"
#include "Entity.h"

using namespace SimpleECS;

void SimpleECS::BoxCollider::update()
{
    Vector position = entity->transform->position;

    bound.xMin = static_cast<int>(position.x) - width / 2;
    bound.xMax = static_cast<int>(position.x) + width / 2;

    bound.yMin = static_cast<int>(position.y) - height / 2;
    bound.yMax = static_cast<int>(position.y) + height / 2;
}

bool BoxCollider::isColliding(Collider* other)
{
    // STUB
    //return ColliderSystem::getCollisionInfo(new Collide);
    return false;
}

void SimpleECS::BoxCollider::getBounds(Collider::AABB& bounds) const
{
    bounds = bound;
}
