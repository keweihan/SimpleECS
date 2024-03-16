#pragma once
#include "Color.h"
#include "ComponentPool.h"
#include <unordered_set>
#include <vector>
#include <memory>

#ifdef SIMPLEECS_EXPORTS
#define SIMPLEECS_API __declspec(dllexport)
#else
#define SIMPLEECS_API __declspec(dllimport)
#endif

namespace SimpleECS
{
	class Entity;

	/**
	* Scene class represents a collection of Entities. A Game instance
	* can only display one scene at a time.
	*/
	class Scene {
	public:
		Scene() : backgroundColor(Color(255,255,255,255)) {};
		Scene(Color backgrundColor) : backgroundColor(backgrundColor) {};
		~Scene();
		/**
		* Add Entity to this scene.
		* 
		* @returns false if entity is already contained by the scene and was not added. 
		* Otherwise returns true if successfully added.
		*/
		SIMPLEECS_API Entity* createEntity();

		/*
		* Given an entity contained in this scene
		* create and attach component T to entity.
		* 
		* @throws if entity id is not contained in this scene. 
		* @returns Original component attached to entity
		*/
		template<typename T>
		T* addComponent(uint32_t e);

		/*
		* Return list of components of type T.
		*
		* @throws if type T is not a component
		* @returns pointer original list of components
		*/
		template<typename T>
		std::vector<T*>& getComponents();

		/*
		* Return component of type T attached to entity with id eid.
		*
		* @throws if type T is not a component
		* @returns pointer original list of components
		*/
		template<typename T>
		T* getComponent(uint32_t eid);

		/**
		* IMMEDIATELY Destroy entity contained by this scene. Proceed with caution, as
		* references can be broken 
		* 
		* @returns false if entity is not contained by the scene and was not deleted. 
		* Otherwise returns true if successfuly removed.
		* 
		*/
		SIMPLEECS_API bool destroyEntityImmediate(uint32_t eid);

		/**
		* Mark entity to be deleted at end of frame. Will call entity and component destructors
		* and entityToDelete will be deleted.
		*
		* @returns false if entity is not contained by the scene and was not deleted.
		* Otherwise returns true if successfuly removed.
		*
		*/
		SIMPLEECS_API bool destroyEntity(uint32_t eid);

		/**
		*  IMMEDIATELY destroys all entities marked for destruction (i.e. in toDestroyEntities) 
		*  Proceed with caution, as references can be broken.
		*/
		void destroyAllMarkedEntities();

		/*
		* Main background render color.
		*/
		Color backgroundColor;

		/*
		* Entities contained by the scene
		*/
		std::vector<Entity*> entities;

	private:
		/*
		* Hidden implementation class.
		*/
		friend Entity;
		class SceneImpl;
		struct SceneImplDeleter
		{
			SIMPLEECS_API void operator()(SceneImpl* p);
		};

		std::unique_ptr<SceneImpl, SceneImplDeleter> pImpl;

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
	};

	template<typename T>
	inline T* Scene::addComponent(uint32_t e)
	{
		//// Check if T is of type component
		//if (!std::is_base_of<Component, T>())
		//{
		//	throw std::invalid_argument("Type called for addComponent is not a component.");
		//}
	
		//// Check if component pool exists
		//if (getComponentID<T>() >= allComponents.size())
		//{
		//	// Pool does not exist yet. Create component pool for type first
		//	allComponents.emplace_back(new T());
		//}
	
		//// Assign component
		//ComponentPoolBase pool = *allComponents[getComponentID<T>()];
		//ComponentPool<T> poolConv = static_cast<ComponentPool<T>>(pool);
		//pool.createComponent(e->id);
		//return poolConv.getComponent(e.id);
	}

	template<typename T>
	std::vector<T*>& SimpleECS::Scene::getComponents()
	{
		//auto pools = pImpl->getComponentPools();
		//return pools[getComponentID<T>()];
	}

	template<typename T>
	inline T* Scene::getComponent(uint32_t e)
	{
		return nullptr;
	}

	template<typename T>
	inline std::size_t Scene::getComponentID()
	{
		// Call nextComponentID per unique type T
		static const std::size_t id = pImpl->nextComponentID();
		return id;
	}
}
