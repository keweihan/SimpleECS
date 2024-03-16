#pragma once
#include "Entity.h"
#include "Scene.h"
#include "Component.h"
#include <stdexcept>

using namespace SimpleECS;


Scene::~Scene() = default;

Entity* Scene::createEntity()
{
	Entity* created = new Entity(nextEntityID(), this);
	if (created->id >= entities.size()) {
		entities.push_back(created);
	}
	return created;
}

bool Scene::destroyEntityImmediate(uint32_t eid)
{
	if (eid >= entities.size())
	{
		return false;
	}

	// Delete components
	for (auto& pool : allComponents)
	{
		(*pool).deleteComponent(eid);
	}

	// Release id back into pool.
	availableEntityIds.insert(eid);

	return true;
}

bool Scene::destroyEntity(uint32_t eid)
{
	return toDestroyEntities.insert(eid).second;
}

void Scene::destroyAllMarkedEntities()
{
	for (auto iter = toDestroyEntities.begin(); iter != toDestroyEntities.end();)
	{
		destroyEntityImmediate(*iter);
		iter = toDestroyEntities.erase(iter);
	}
}

std::vector<std::shared_ptr<ComponentPoolBase>>& Scene::getComponentPools()
{
	return allComponents;
}

std::uint32_t Scene::nextEntityID()
{
	if (availableEntityIds.empty())
	{
		return maxID++;
	}
	else
	{
		uint32_t id = *availableEntityIds.begin();
		availableEntityIds.erase(availableEntityIds.begin());
		return id;
	}
}

std::size_t SimpleECS::Scene::nextComponentID()
{
	static std::size_t lastID = 0;
	return lastID++;
}
