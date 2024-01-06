#include "Game.h"
#include "Scene.h"
#include "GameRenderer.h"
#include "Entity.h"
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
		for (auto component : entity->components)
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
		SDL_SetRenderDrawColor(GameRenderer::renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(GameRenderer::renderer);

		// Run update of first scene functions
		for (auto entity : sceneList[0]->entities)
		{
			for (auto component : entity->components)
			{
				component->update();
			};
		}

		SDL_RenderPresent(GameRenderer::renderer);
	}
}
