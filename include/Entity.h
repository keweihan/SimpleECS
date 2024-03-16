#pragma once

#include "Scene.h"
#include "Transform.h"
#include <vector>
#include <string>

#ifdef SIMPLEECS_EXPORTS
#define SIMPLEECS_API __declspec(dllexport)
#else
#define SIMPLEECS_API __declspec(dllimport)
#endif

namespace SimpleECS
{	
	/**
	An object/actor inside scenes. Has a container of components which dictate entity behavior.
	and exists in world space.
	*/
	class Entity {
	private:		

		/**
		* Pointer to scene containing this entity.
		*/
		Scene* scene;

	public:
		// TODO: make private
		SIMPLEECS_API Entity(uint32_t id, Scene* s) : id(id), scene(s) {};
		SIMPLEECS_API ~Entity();

		/**
		* Internal identifier for this entity. Instantiated on construction.
		* TODO make private
		*/
		uint32_t id;

		/**
		* Optional string identifier for this entity
		*/
		std::string tag;

		/**
		* Entity position in world space.
		* TODO: redo...
		*/
		Transform* transform = nullptr;

		/**
		* Add a component to this entity of type T
		* 
		* @returns Component* added to entity.
		*/
		template <typename T>
		T* addComponent();

		/**
		* Retrieve a component attached to entity of type T.
		* 
		* @returns A single component of type T attached to entity.
		* nullptr if no component of such type is attached to entity.
		* 
		*/
		template <typename T>
		T* getComponent();
	};

	template <typename T>
	inline T* Entity::addComponent()
	{
		return scene->addComponent<T>(id);
	}

	template<typename T>
	inline T* Entity::getComponent()
	{
		return scene->getComponent<T>(id);
	}
}
