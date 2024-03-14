#pragma once
#include "Entity.h"
#include <vector>

using namespace SimpleECS;

SimpleECS::Entity::~Entity()
{
	// Free all components attached to entity
	//scene->destroyEntity(*this);
}