#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include "Component.h"
#include "Transform.h"

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
	public:
		~Entity();

		/**
		* Add a component to this entity
		* 
		*/
		void SIMPLEECS_API addComponent(Component* component);

		/**
		* Retrieve a component attached to entity of type T.
		* 
		* @returns nullptr if no component of such type is attached to entity.
		*/
		template <typename T>
		T* getComponent();

		/**
		* Retrieve list of all components attached to this entity.
		*/
		std::vector<Component*>& getComponents();
		
		/**
		* Entity position in world space.
		*/
		Transform transform;
		
	private:
		/**
		* List of components
		*/
		std::vector<Component*> components;

	};

	template<typename T>
	inline T* Entity::getComponent()
	{
		T* foundComponent = nullptr;
		for (auto component : components)
		{
			foundComponent = dynamic_cast<T*>(component);
			if (foundComponent != nullptr)
			{
				return foundComponent;
			}
		}
		return foundComponent;
	}
}
#endif