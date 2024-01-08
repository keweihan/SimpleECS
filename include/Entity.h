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
	class Entity {
	public:
		// transform
		Transform transform;

		void SIMPLEECS_API addComponent(Component* component);

		template <typename T>
		T* getComponent();

		std::vector<Component*>& getComponents();
		
		// Component list
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