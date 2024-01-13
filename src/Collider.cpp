#include "Collider.h"
#include "ColliderSystem.h"

SimpleECS::Collider::Collider()
{
	// TODO: should be responsibillity of scene load
	ColliderSystem::registerCollider(this);
}

SimpleECS::Collider::~Collider()
{
	ColliderSystem::deregisterCollider(this);
}
