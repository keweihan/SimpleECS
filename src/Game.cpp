#include "Game.h"
#include "Scene.h"
#include "GameRenderer.h"
#include <SDL.h>
#include <iostream>

using namespace SimpleECS;

Game::Game()
{

}

void Game::startGame()
{
	init();
	mainLoop();
}

int Game::addScene(Scene scene)
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

	// Game loop
	while (!quit)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}
	}
}
