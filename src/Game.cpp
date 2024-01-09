#include "Game.h"
#include "Scene.h"
#include "GameRenderer.h"
#include "Entity.h"
#include "ColliderSystem.h"
#include "Timer.h"
#include "Color.h"
#include <SDL.h>
#include <iostream>

using namespace SimpleECS;
using namespace UtilSimpleECS;

Game::Game()
{

}

void Game::startGame()
{
	init();
	mainLoop();
}

int Game::addScene(Scene* scene)
{
	sceneList.push_back(scene);
	return static_cast<int>(sceneList.size() - 1);
}

void Game::init()
{
	GameRenderer::initGameRenderer();
}

void Game::mainLoop()
{
	SDL_Event e;
	bool quit = false;

	if (sceneList.size() == 0) {
		throw std::runtime_error("No scenes in game! ");
	}

	// Run initialize of first scene components
	for (auto entity : sceneList[0]->entities)
	{

		for (auto component : entity->getComponents())
		{
			component->initialize();
		};
	}

	// Game loop
	while (!quit)
	{
		// Check for closing window
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		//Clear screen
		Color sceneColor = sceneList[0]->backgroundColor;
		SDL_SetRenderDrawColor(GameRenderer::renderer, sceneColor.r, sceneColor.g, sceneColor.b, sceneColor.a);
		SDL_RenderClear(GameRenderer::renderer);

		// Run update of first scene functions
		for (auto entity : sceneList[0]->entities)
		{
			for (auto component : entity->getComponents())
			{
				component->update();
			};
		}
		
		// Run collision functions
		ColliderSystem::invokeCollisions();

		// Mark end of frame
		Timer::endFrame();

		SDL_RenderPresent(GameRenderer::renderer);
	}
}
