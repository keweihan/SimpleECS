#include "PhysicsBody.h"
#include "Timer.h"

void SimpleECS::PhysicsBody::update()
{
	entity->transform.posX += velocity.x * Timer::getDeltaTime()/1000;
	entity->transform.posY += velocity.y * Timer::getDeltaTime()/1000;
}

void SimpleECS::PhysicsBody::onCollide(const Collider& other)
{

}

void SimpleECS::PhysicsBody::onCollide(const Collision& collide)
{
	// Shift position out of overlap
	entity->transform.posX += (collide.normal * collide.penetration).x;
	entity->transform.posY += (collide.normal * collide.penetration).y;

	// Calculate new velocity (reflect across collision normal)
	Vector newVelocity = velocity - (collide.normal * (velocity.dotProduct(collide.normal))) * 2;
	velocity = newVelocity;
}