#pragma once
#include <vector>
#include "Scene.h"

class Game {
public
	Scene GetActiveScene();

	void SetActiveScene(int sceneIndex);

	// Adds an existing entity to this scene
	void AddScene(Scene scene);


private:
	std::vector<Scene> scene;
};