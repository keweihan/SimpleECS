#include "Scene.h"
#include "Game.h"
#include "GameRenderer.h"
#include "ComponentPool.h"
#include "Entity.h"
#include <vector>
#include <memory>
#include <type_traits>
#include <stdexcept>
#include <cassert>

using namespace SimpleECS;

/* ---------Scene Impl----- */
class Scene::SceneImpl {
public:
	SceneImpl(Scene* s) : scene(s) {}

	/**
	* Add Entity to this scene.
	*
	* @returns false if entity is already contained by the scene and was not added.
	* Otherwise returns true if successfully added.
	*/
	Entity createEntity();

	/*
	* Given an entity contained in this scene
	* create and attach component T to entity.
	*
	* @throws if entity id is not contained in this scene.
	* @returns Original component attached to entity
	*/
	template<typename T>
	T& addComponent(Entity e);

	/*
	* Return list of components of type T.
	* 
	* @throws if type T is not a component
	* @returns pointer original list of components
	*/
	template<typename T>
	std::vector<T>* getComponents();

	/**
	* IMMEDIATELY Destroy entity contained by this scene. Proceed with caution, as
	* references can be broken
	*
	* @returns false if entity is not contained by the scene and was not deleted.
	* Otherwise returns true if successfuly removed.
	*
	*/
	bool destroyEntityImmediate(Entity entityToDelete);
	bool destroyEntityImmediate(int entityId);

	/**
	* Mark entity to be deleted at end of frame. Will call entity and component destructors
	* and entityToDelete will be deleted.
	*
	* @returns false if entity is not contained by the scene and was not deleted.
	* Otherwise returns true if successfuly removed.
	*
	*/
	bool destroyEntity(Entity entityToDelete);

	/**
	*  IMMEDIATELY destroys all entities marked for destruction (i.e. in toDestroyEntities)
	*  Proceed with caution, as references can be broken.
	*/
	void destroyAllMarkedEntities();

	/*
	* Return list of ALL component pools
	* TODO: abstract out component pool list to be a "ComponentPools"
	*/
	std::vector<std::shared_ptr<ComponentPoolBase>> getComponentPools();

private:
	/* Allow access
	*/
	friend Entity;

	/*
	* Pool of available entity ids. If empty, use max.
	*/
	std::unordered_set<uint32_t> availableEntityIds;

	/*
	* Entities marked for destruction. Cleared at end of every frame to be deleted.
	*/
	std::unordered_set<uint32_t> toDestroyEntities;
	
	/*
	* Pool of available entity ids. If empty, use max.
	*/
	int maxID;

	/*
	* Pointer to scene object this implements.
	*/
	Scene* scene;

	/*
	* Component pool list for scene
	* Stores components in a contiguous storage for fast iteration. 
	* Access component pool for type T with allComponents[getComponentID<T>()]
	*/
	std::vector<std::shared_ptr<ComponentPoolBase>> allComponents;

	/*
	* Return id unique to the type of component.
	* 
	* @throws If T does not inherit from Component
	*/
	template<typename T>
	static std::size_t getComponentID();

	/*
	* Return new id unique to the type of component
	*/
	static std::size_t nextComponentID();

	/*
	* Return the ID of the next entity to be created.
	*/
	std::uint32_t nextEntityID();
};

template<typename T>
T& Scene::SceneImpl::addComponent(Entity e)
{
	// Check if T is of type component
	if (!std::is_base_of<Component, T>())
	{
		throw std::invalid_argument("Type called for addComponent is not a component.");
	}

	// Check if component pool exists
	if (getComponentID<T>() >= allComponents.size())
	{
		// Pool does not exist yet. Create component pool for type first
		allComponents.emplace_back(new T());
	}

	// Assign component
	ComponentPoolBase pool = *allComponents[getComponentID<Transform>()];
	ComponentPool<T> poolConv = static_cast<ComponentPool<T>>(pool);
	pool.createComponent(e->id);
	return poolConv.getComponent(e.id);
}

template<typename T>
std::vector<T>* Scene::SceneImpl::getComponents()
{
	return *allComponents[getComponentID<T>()];
}

template<typename T>
std::size_t Scene::SceneImpl::getComponentID()
{
	// Call nextComponentID per unique type T
	static const std::size_t id = nextComponentID();
	return id;
}

std::size_t Scene::SceneImpl::nextComponentID()
{
	static std::size_t lastID = 0;
	return lastID++;
}

std::vector<std::shared_ptr<ComponentPoolBase>> Scene::SceneImpl::getComponentPools()
{
	return allComponents;
}

Entity Scene::SceneImpl::createEntity()
{
	Entity created = Entity(nextEntityID(), scene);
	if (created.id >= scene->entities.size()) {
		scene->entities.push_back(created);
	}
	return created;
}

bool Scene::SceneImpl::destroyEntityImmediate(Entity e)
{
	return destroyEntityImmediate(e.id);
}

bool Scene::SceneImpl::destroyEntityImmediate(int entityId)
{
	// Delete components
	for (auto& pool : allComponents)
	{
		(*pool).deleteComponent(entityId);
	}

	// Release id back into pool.
	availableEntityIds.insert(entityId);
}

bool Scene::SceneImpl::destroyEntity(Entity entityToDelete)
{
	return toDestroyEntities.insert(entityToDelete.id).second;
}

void Scene::SceneImpl::destroyAllMarkedEntities()
{
	for (auto iter = toDestroyEntities.begin(); iter != toDestroyEntities.end();)
	{
		destroyEntityImmediate(*iter);
		iter = toDestroyEntities.erase(iter);
	}
}

std::uint32_t Scene::SceneImpl::nextEntityID()
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

/* ---------Scene----- */
Entity Scene::createEntity()
{
	return pImpl->createEntity();
}

bool Scene::destroyEntityImmediate(Entity entityToDelete)
{
	return pImpl->destroyEntityImmediate(entityToDelete);
}

bool SimpleECS::Scene::destroyEntity(Entity entityToDelete)
{
	return pImpl->destroyEntity(entityToDelete);
}

void SimpleECS::Scene::destroyAllMarkedEntities()
{
	return pImpl->destroyAllMarkedEntities();
}

template<typename T>
inline T& Scene::addComponent(Entity e)
{
	return pImpl->addComponent(e);
}

template<typename T>
std::vector<T>* SimpleECS::Scene::getComponents()
{
	return pImpl->getComponents();
}
