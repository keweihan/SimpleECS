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
		* 
		* @returns false if entity is already contained by the scene and was not added. 
		* Otherwise returns true if successfully added.
		*/
		bool SIMPLEECS_API AddEntity(Entity* entity);

		/**
		* Destroy entity contained by this scene. Will call entity and component destructors
		* and entityToDelete will be deleted.
		* 
		* @returns false if entity is not contained by the scene and was not deleted. 
		* Otherwise returns true if successfuly removed.
		* 
		*/
		bool SIMPLEECS_API DestroyEntity(Entity* entityToDelete);

		/*
		* Main background render color.
		*/
		Color backgroundColor;

		/*
		* Main background render color.
		*/
		std::unordered_set<Entity*> entities;
	};
}