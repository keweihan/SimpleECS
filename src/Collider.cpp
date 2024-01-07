#include "Collider.h"
#include "ColliderSystem.h"

SimpleECS::Collider::Collider()
{
	ColliderSystem::registerCollider(this);
}

SimpleECS::Collider::~Collider()
{
	ColliderSystem::deregisterCollider(this);
}
