#pragma once
#include <vector>
#include "Scene.h"

namespace SimpleECS
{
	class Game {
	public:
		void StartGame();
		
	private:
		std::vector<Scene> scene;
	};
}