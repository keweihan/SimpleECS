#pragma once
#include "Entity.h"

#ifdef SIMPLEECS_EXPORTS
#define SIMPLEECS_API __declspec(dllexport)
#else
#define SIMPLEECS_API __declspec(dllimport)
#endif

namespace SimpleECS
{
	class Scene {
	public:
		// Adds an existing entity to this scene
		void SIMPLEECS_API AddEntity(Entity* entity);

		std::vector<Entity*> entities;
	};
}