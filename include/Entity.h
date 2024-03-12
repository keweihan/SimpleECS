#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include "Component.h"
#include "Scene.h"
#include "Transform.h"
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
		friend Scene::SceneImpl;
		Entity(uint32_t id, Scene* s) : id(id), scene(s) {};

		/**
		* Internal identifier for this entity. Instantiated on construction.
		*/
		uint32_t id;

		/**
		* Pointer to scene containing this entity.
		*/
		Scene* scene;

	public:
		/**
		* Optional string identifier for this entity
		*/
		std::string tag;

		/**
		* Entity position in world space.
		* TODO: redo...
		*/
		Transform* transform;

		/**
		* Add a component to this entity of type T
		* 
		* @returns Component* added to entity.
		*/
		template <typename T>
		T* SIMPLEECS_API addComponent();

		/**
		* Retrieve a component attached to entity of type T.
		* 
		* @returns A single component of type T attached to entity.
		* nullptr if no component of such type is attached to entity.
		* 
		*/
		template <typename T>
		T* SIMPLEECS_API getComponent();

		/**
		* Retrieve list of all components of type T attached to this entity.
		* Use T as Component base class to return all components attached to entity. 
		* 
		* @throws Error if T is not of component type
		*/
		template <typename T>
		std::vector<T*>& SIMPLEECS_API getComponents();
	};
}
#endif