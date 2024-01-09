#pragma once
#include "Entity.h"
#include "Color.h"

#ifdef SIMPLEECS_EXPORTS
#define SIMPLEECS_API __declspec(dllexport)
#else
#define SIMPLEECS_API __declspec(dllimport)
#endif

namespace SimpleECS
{
	class Scene {
	public:
		Scene() : backgroundColor(Color(255,255,255,255)) {};
		Scene(Color backgrundColor) : backgroundColor(backgrundColor) {};

		// Adds an existing entity to this scene
		void SIMPLEECS_API AddEntity(Entity* entity);

		// Removes an existing entity from this scene
		bool SIMPLEECS_API DestroyEntity(Entity* entity);

		Color backgroundColor;
		std::vector<Entity*> entities;
	};
}