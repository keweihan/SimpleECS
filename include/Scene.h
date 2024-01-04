#pragma once
#include "Entity.h"

class Scene {
public:
	// Adds an existing entity to this scene
	void AddEntity(Entity entity);


private:
	std::vector<Entity> entities;
};