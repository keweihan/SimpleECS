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
	// TODO:
	// Eventaully collision resolution needs to all occur at the SAME TIME AT END OF FRAME
	// i.e. setting first before getting. 
	// 
	// Consider two physics body going towards each other in
	// in a collision. If the first body's velocity is set from collision resolution
	// before the second resolves its collision, the second will resolve as if the first was already
	// flying away.
	
	// Shift position out of overlap
	entity->transform.position.x += (collide.normal * collide.penetration).x;
	entity->transform.position.y += (collide.normal * collide.penetration).y;

	// Calculate new velocity (reflect across collision normal)
	Vector newVelocity = velocity - (collide.normal * (velocity.dotProduct(collide.normal))) * 2;
	velocity = newVelocity;
}