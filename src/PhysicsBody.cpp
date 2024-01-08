#include "PhysicsBody.h"

void SimpleECS::PhysicsBody::update()
{
	// TODO : normalize agianst frame time!!
	entity->transform.posX += velocity.x;
	entity->transform.posY += velocity.y;
}

void SimpleECS::PhysicsBody::onCollide(const Collider& other)
{

}