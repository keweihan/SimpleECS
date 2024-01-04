#include "Game.h"
#include <SDL.h>
#include <iostream>

using namespace SimpleECS;

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
	// Initialize SDL
	if (SDL_InitSubSystem(SDL_INIT_VIDEO))
	{
		throw std::runtime_error("Failure to initialize SDL. SDL_Error: " + std::string(SDL_GetError()));
	}
	
	// Create window
	window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		throw std::runtime_error("Failure to create window. SDL_Error: " + std::string(SDL_GetError()));
	}
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
