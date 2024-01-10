#include "Entity.h"
#include "Component.h"
#include <vector>

using namespace SimpleECS;

SimpleECS::Entity::~Entity()
{
	// Free all components attached to entity
	for (auto component : components)
	{
		delete component;
	}
}

void Entity::addComponent(Component* component)
{
	component->setEntity(this);
	components.push_back(component);
}

std::vector<Component*>& Entity::getComponents()
{
	return components;
}