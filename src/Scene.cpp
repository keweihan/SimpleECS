#include "Scene.h"
#include "Game.h"
#include "Scene.h"
#include "GameRenderer.h"
#include "Entity.h"

using namespace SimpleECS;

void Scene::AddEntity(Entity* entity)
{
	entities.push_back(entity);
}