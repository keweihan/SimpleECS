#include "Collision/BoxCollider.h"
#include "Physics/PhysicsBody.h"
#include "ColliderSystem.h"
#include "Core/Entity.h"
#include "Collision/Visitors/BoxColliderVisitor.h"

using namespace SimpleECS;

void SimpleECS::BoxCollider::update()
{
    Vector position = entity->transform->position;

    // TODO: why calculate bounds every frame?
    bound.xMin = position.x - width / 2;
    bound.xMax = position.x + width / 2;

    bound.yMin = position.y - height / 2;
    bound.yMax = position.y + height / 2;
}

bool BoxCollider::isColliding(Collider* other)
{
    // STUB
    //return ColliderSystem::getCollisionInfo(new Collide);
    // TODO: what is this
    return false;
}

void SimpleECS::BoxCollider::getBounds(Collider::AABB& bounds) const
{
    bounds = bound;
}

bool SimpleECS::BoxCollider::accept(Collision& out, std::shared_ptr<SimpleECS::ColliderVisitor> visitor)
{
    return visitor->visitBox(out);
}

std::shared_ptr<SimpleECS::ColliderVisitor> SimpleECS::BoxCollider::getVisitor()
{
    return visitor;
}
