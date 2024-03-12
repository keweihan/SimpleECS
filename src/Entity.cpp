#include "Entity.h"
#include "Scene.h"
#include "Component.h"
#include <vector>

using namespace SimpleECS;

SimpleECS::Entity::~Entity()
{
	// Free all components attached to entity
	scene->destroyEntity(*this);
}

template <typename T>
T* Entity::addComponent()
{
	// TODO: insert return statement here
}

template<typename T>
inline T* Entity::getComponent()
{
	// TODO: insert return statement here
}

template<typename T>
std::vector<T*>& SimpleECS::Entity::getComponents()
{
	// TODO: insert return statement here
}
