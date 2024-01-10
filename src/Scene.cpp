#include "Scene.h"
#include "Game.h"
#include "Scene.h"
#include "GameRenderer.h"
#include "Entity.h"

using namespace SimpleECS;

bool Scene::AddEntity(Entity* entity)
{
	// Scene cannot contain two instances of the same entity
	if (entities.find(entity) == entities.end())
	{
		entities.insert(entity);
		return true;
	}

	return false;
}

bool Scene::DestroyEntity(Entity* entityToDelete)
{
	// Scene can only delete an entity it contains
	if (entities.find(entityToDelete) != entities.end())
	{
		entities.erase(entityToDelete);
		delete entityToDelete;

		return true;
	}

	return false;
}
