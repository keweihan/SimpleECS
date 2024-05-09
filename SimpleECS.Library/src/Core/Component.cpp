#include "Component.h"

void SimpleECS::Component::setEntity(Entity* entity)
{
	this->entity = entity;
}
