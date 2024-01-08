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