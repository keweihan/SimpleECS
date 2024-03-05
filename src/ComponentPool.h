#pragma once
#include <vector>
#include <memory>
#include <unordered_set>

namespace SimpleECS {
	class ComponentPoolBase {};

	template<typename T>
	class ComponentPool : public ComponentPoolBase {
	public:
		ComponentPool() {};

		assignComponent(uint32_t entityID) {
			// barebones example
			sparseList[entityID] = componentList.size() - 1;
			componentList.push_back(T());
		};

		deleteComponent(uint32_t entityID) {
			// check if has component

			// remove from sparseList and componentList
		};

		// i.e. [ -1, 0, 2, 1]. where -1 at index 0 means entity 0
		// does NOT have a component of this type T. The 1 at index 3 means
		// entity 1 has a component of this type T stored in the dense list of componentList at index 1.
		std::vector<int> sparseList; 

		// Dense list
		std::vector<T> componentList;
	};
	
	// Planning/skeleton code

	// Entity.Only exists with relation to a scene.
	class Entity {
	public:
		uint32_t id;

		Entity() {

		}

		template<typename T>
		void addComponent() {
			// Game.currActiveScene->allComponents.insert(ComponentPool<T>());
		}

		template<typename T>
		T getComponent() {

		}

		// pool of available ids
		static std::unordered_set<int> availableIds;

		// id to set to new entity if it were instantiated and no availableIds exist. 
		// Incremented after this occurs. 
		static int maxID;
	};

	// Scene AKA EntityManager/container
	class Scene {
		Entity CreateEntity() {};

		// Retrieve list for a specific pool (i.e. collider...)
		template<typename T>
		ComponentPool<T>& getPool() {};

	private:
		// SCENE:
		
		std::vector<std::unique_ptr<ComponentPoolBase>> allComponents;
	};
}
