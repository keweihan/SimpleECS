#pragma once
#include "Color.h"
#include <unordered_set>
#include "ComponentPool.h"
#include "Entity.h"
#include <vector>
#include <memory>

#ifdef SIMPLEECS_EXPORTS
#define SIMPLEECS_API __declspec(dllexport)
#else
#define SIMPLEECS_API __declspec(dllimport)
#endif

namespace SimpleECS
{

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
		Entity SIMPLEECS_API createEntity();

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
		bool SIMPLEECS_API destroyEntityImmediate(Entity entityToDelete);

		/**
		* Mark entity to be deleted at end of frame. Will call entity and component destructors
		* and entityToDelete will be deleted.
		*
		* @returns false if entity is not contained by the scene and was not deleted.
		* Otherwise returns true if successfuly removed.
		*
		*/
		bool SIMPLEECS_API destroyEntity(Entity entityToDelete);

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
		std::vector<Entity> entities;

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
	};

	template<typename T>
	inline T& Scene::addComponent(Entity e)
	{
		return nullptr;
	}

	template<typename T>
	std::vector<T>* SimpleECS::Scene::getComponents()
	{
		return nullptr;
	}
}
