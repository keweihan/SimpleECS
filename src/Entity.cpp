#include "Entity.h"
#include "Component.h"
#include <vector>

using namespace SimpleECS;

void Entity::addComponent(Component* component)
{
	//component->setEntity(this);
	components.push_back(component);
}

std::vector<Component*>& Entity::getComponents()
{
	return components;
}
