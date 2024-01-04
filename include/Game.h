#pragma once
#include <vector>
#include "Scene.h"

#ifdef SIMPLEECS_EXPORTS
#define SIMPLEECS_API __declspec(dllexport)
#else
#define SIMPLEECS_API __declspec(dllimport)
#endif

namespace SimpleECS
{
	class Game {
	public:
		SIMPLEECS_API void StartGame();
		
	private:
		std::vector<Scene> scene;
	};
}