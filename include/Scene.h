#pragma once
#include "Entity.h"
#include "Color.h"
#include <unordered_set>

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

		/**
		* Add Entity to this scene.
		*/
		void SIMPLEECS_API AddEntity(Entity* entity);

		/**
		* Destroy entity from this scene. Will call entity and component destructors.
		* 
		* @returns true if entity was found and deleted, false otherwise
		*/
		bool SIMPLEECS_API DestroyEntity(Entity* entityToDelete);

		Color backgroundColor;
		//std::unordered_set<Entity*> entities;
		std::vector<Entity*> entities;
	};
}