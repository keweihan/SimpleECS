#include "Scene.h"
#include "Game.h"
#include "Scene.h"
#include "GameRenderer.h"
#include "Entity.h"

using namespace SimpleECS;

void Scene::AddEntity(Entity* entity)
{
	//for (auto entityPtr : entities)
	//{
	//	// Find entity reference
	//	if (entityPtr == entityToDelete)
	//	{
	//		// Free all components attached to entity
	//		for (auto component : entityToDelete->getComponents())
	//		{
	//			delete component;
	//			return true;
	//		}
	//	}
	//}
	entities.push_back(entity);
}

bool Scene::DestroyEntity(Entity* entityToDelete)
{
	for (auto entityPtr : entities)
	{
		// Find entity reference
		if (entityPtr == entityToDelete)
		{
			// Free all components attached to entity
			for (auto component : entityToDelete->getComponents())
			{
				delete component;
				return true;
			}
		}
	}

	return false;
}
