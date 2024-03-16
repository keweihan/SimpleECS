#pragma once
#include "Component.h"
#include <vector>
#include <memory>
#include <unordered_set>
#include <iterator>
#include <stdexcept>

namespace SimpleECS {
	class ComponentPoolBase {
	public:
		virtual void deleteComponent(uint32_t entityID) = 0;
		virtual void invokeStart() = 0;
		virtual void invokeUpdate() = 0;
	};

	template<typename T>
	class ComponentPool : public ComponentPoolBase {
	public:
		ComponentPool() {};

		/*
		* Create component and assign it to the entity entityID.
		* @throws If entity already has component
		*/
		template <typename... Args>
		void createComponent(uint32_t entityID, Args&&... args);

		// check if has component
		// remove from sparseList and componentList
		void deleteComponent(uint32_t entityID);

		T* getComponent(uint32_t entityID);

		void invokeStart();

		void invokeUpdate();

		/*
		Get list of components of this pool
		*/
		std::vector<T>* getComponents();

		// i.e. [ -1, 0, 2, 1]. where -1 at index 0 means entity 0
		// does NOT have a component of this type T. The 1 at index 3 means
		// entity 1 has a component of this type T stored in the dense list of componentList at index 1.
		// Note: Entity can only have one of each component type. 
		std::vector<int> sparseList; 

		// Dense list
		std::vector<T> componentList;
	};
	
	// Planning/skeleton code

	// Entity.Only exists with relation to a scene.
	//class Entity {
	//public:
	//	uint32_t id;

	//	Entity() {

	//	}

	//	template<typename T>
	//	void addComponent() {
	//		// Game.currActiveScene->allComponents.insert(ComponentPool<T>());
	//	}

	//	template<typename T>
	//	T getComponent() {

	//	}
	//};

	// Scene AKA EntityManager/container
	//class Scene {
	//	Entity CreateEntity() {};

	//	// Retrieve list for a specific pool (i.e. collider...)
	//	template<typename T>
	//	ComponentPool<T>& getPool() {};

	//private:
	//	// SCENE:
	//	
	//	std::vector<std::unique_ptr<ComponentPoolBase>> allComponents;

	//	// pool of available ids
	//	static std::unordered_set<int> availableIds;

	//	// id to set to new entity if it were instantiated and no availableIds exist. 
	//	// Incremented after this occurs. 
	//	static int maxID;
	//};

	template<typename T>
	template <typename... Args>
	void SimpleECS::ComponentPool<T>::createComponent(uint32_t entityID, Args&&... args)
	{
		if (entityID >= sparseList.size())
		{
			sparseList.resize(entityID + 1, -1);
		}

		componentList.push_back(T(std::forward<Args>(args)...));
		sparseList[entityID] = componentList.size() - 1;
	}

	template<typename T>
	void SimpleECS::ComponentPool<T>::deleteComponent(uint32_t entityID)
	{
		// Check if entity has this component
		if (sparseList[entityID] == -1)
		{
			throw std::logic_error("Entity does not have component to delete.");
		}

		// Mark entity as not having this component.
		sparseList[entityID] = -1;
	}

	template<typename T>
	T* SimpleECS::ComponentPool<T>::getComponent(uint32_t entityID)
	{
		if (sparseList[entityID] == -1)
		{
			return nullptr;
		}
		else
		{
			return &componentList[sparseList[entityID]];
		}
	}

	template<typename T>
	void SimpleECS::ComponentPool<T>::invokeStart()
	{
		for(auto& component : componentList)
		{
			component.initialize();
		}
	}

	template<typename T>
	void SimpleECS::ComponentPool<T>::invokeUpdate()
	{
		for (auto& component : componentList)
		{
			component.update();
		}
	}

	template<typename T>
	std::vector<T>* SimpleECS::ComponentPool<T>::getComponents()
	{
		return componentList;
	}

}
