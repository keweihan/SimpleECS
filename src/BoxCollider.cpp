#include "BoxCollider.h"
#include "PhysicsBody.h"
#include "ColliderSystem.h"
#include "Entity.h"

using namespace SimpleECS;

bool BoxCollider::isColliding(Collider* other)
{
    // STUB
    //return ColliderSystem::getCollisionInfo(new Collide);
    return false;
}

void SimpleECS::BoxCollider::getBounds(Collider::AABB& bounds)
{
    Vector position = entity->transform.position;

    bounds.xMin = position.x - width / 2;
    bounds.xMax = position.x + width / 2;

    bounds.yMin = position.y - height / 2;
    bounds.yMax = position.y + height / 2;
}
