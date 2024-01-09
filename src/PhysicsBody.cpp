#include "PhysicsBody.h"
#include "Timer.h"

void SimpleECS::PhysicsBody::update()
{
	entity->transform.position.x += velocity.x * Timer::getDeltaTime()/1000;
	entity->transform.position.y += velocity.y * Timer::getDeltaTime()/1000;
}

void SimpleECS::PhysicsBody::onCollide(const Collider& other)
{

}

void SimpleECS::PhysicsBody::onCollide(const Collision& collide)
{
	// Shift position out of overlap
	entity->transform.position.x += (collide.normal * collide.penetration).x;
	entity->transform.position.y += (collide.normal * collide.penetration).y;

	// Calculate new velocity (reflect across collision normal)
	Vector newVelocity = velocity - (collide.normal * (velocity.dotProduct(collide.normal))) * 2;
	velocity = newVelocity;
}