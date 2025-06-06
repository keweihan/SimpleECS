#include "Collision/Visitors/BoxColliderVisitor.h"
#include "Collision/BoxCollider.h"
#include "Collision/Collider.h"
#include "Core/Entity.h"

#include <iostream>

bool SimpleECS::BoxColliderVisitor::visitBox(Collision &collide)
{
    if (collide.a == nullptr || collide.b == nullptr) return false;


    // BoxCollider* aBox = collider;
	// BoxCollider* bBox = parentCollide;
    // Transform& aTransform = *(aBox->entity->transform);
	// Transform& bTransform = *(bBox->entity->transform);

    // Static cast is allowed - can make this assumption in this function.
    Transform& aTransform = *(collide.a->entity->transform);
	Transform& bTransform = *(collide.b->entity->transform);
	BoxCollider* aBox = static_cast<BoxCollider*>(collide.a);
	BoxCollider* bBox = static_cast<BoxCollider*>(collide.b);

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

		//if ((yOverlap < xOverlap ? yOverlap : xOverlap) == 0) {
		//	return false;
		//}

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

