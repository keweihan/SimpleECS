#include "BoxCollider.h"
#include "Entity.h"

using namespace SimpleECS;

bool BoxCollider::isColliding(Collider* other)
{
    BoxCollider* otherBox = dynamic_cast<BoxCollider*>(other);

    if (otherBox != nullptr)
    {
        // AABB Collision resolution 
        int aLeft, aRight, aTop, aBottom;
        int bLeft, bRight, bTop, bBottom;

        aLeft = this->entity->transform.posX - width / 2;
        aRight = this->entity->transform.posX + width / 2;
        aBottom = this->entity->transform.posY - height / 2;
        aTop = this->entity->transform.posY + height / 2;

        bLeft = other->entity->transform.posX - otherBox->width / 2;
        bRight = other->entity->transform.posX + otherBox->width / 2;
        bBottom = other->entity->transform.posY - otherBox-> height / 2;
        bTop = other->entity->transform.posY + otherBox-> height / 2;

        //If any of the sides from A are outside of B
        if (aBottom >= bTop)
        {
            return false;
        }

        if (aTop <= bBottom)
        {
            return false;
        }

        if (aRight <= bLeft)
        {
            return false;
        }

        if (aLeft >= bRight)
        {
            return false;
        }

        //If none of the sides from A are outside B
        return true;
    }
    // Other collider types here

    return false;

}
