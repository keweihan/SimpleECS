#include "Collision/BoxCollider.h"
#include "Physics/PhysicsBody.h"
#include "ColliderSystem.h"
#include "Core/Entity.h"

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

void SimpleECS::BoxCollider::getBounds(Collider::AABB& bounds) const
{
    bounds = bound;
}

bool SimpleECS::BoxCollider::visitBox(Collision &collide, BoxCollider *box)
{
    if (collide.a == nullptr || collide.b == nullptr) return false;

    // TODO: check correct ordering
	BoxCollider* aBox = this;
	BoxCollider* bBox = box;
    Transform& aTransform = *(aBox->entity->transform);
	Transform& bTransform = *(bBox->entity->transform);

	if (bBox != nullptr && aBox != nullptr)
	{
		// AABB Collision 
		Collider::AABB aBounds;
		Collider::AABB bBounds;
		aBox->getBounds(aBounds);
		bBox->getBounds(bBounds);

		//If any of the sides from A are outside of B, no collision occuring.
		double epsilon = 0.01;  // Small margin value
		if (aBounds.yMin >= bBounds.yMax - epsilon || aBounds.yMax <= bBounds.yMin + epsilon
			|| aBounds.xMax <= bBounds.xMin + epsilon || aBounds.xMin >= bBounds.xMax - epsilon)
		{
			return false;
		}

		// Boxes are colliding. Find axis of least penetration
		double aExtentX = aBox->width / 2.0;
		double bExtentX = bBox->width / 2.0;
		double aExtentY = aBox->height / 2.0;
		double bExtentY = bBox->height / 2.0;

		double xDistance = std::abs(aTransform.position.x - bTransform.position.x);
		double xOverlap = (aExtentX + bExtentX) - xDistance;

		double yDistance = std::abs(aTransform.position.y - bTransform.position.y);
		double yOverlap = (aExtentY + bExtentY) - yDistance;

		// Least penetration is on y-axis
		if (yOverlap < xOverlap)
		{
			collide.penetration = yOverlap;
			if (aTransform.position.y < bTransform.position.y)
			{
				collide.normal = Vector(0, -1);
			}
			else
			{
				collide.normal = Vector(0, 1);
			}
		}
		// Least penetration is on x-axis
		else
		{
			collide.penetration = xOverlap;
			if (aTransform.position.x < bTransform.position.x)
			{
				collide.normal = Vector(-1, 0);
			}
			else
			{
				collide.normal = Vector(1, 0);
			}
		}
	}

	return true;
}

bool SimpleECS::BoxCollider::accept(Collision& out, Collider* visitor)
{
    return visitor->visitBox(out, this);
}