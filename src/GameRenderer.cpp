#include "GameRenderer.h"
#include <SDL.h>
#include <iostream>
#include <memory>
#include <utility>

using namespace SimpleECS;

// ---------------- GameRenderer definitions ----------------//
void GameRenderer::initGameRenderer()
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

	// Set white window surface
	screenSurface = SDL_GetWindowSurface(window);
	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
	SDL_UpdateWindowSurface(window);
}

SDL_Window* GameRenderer::window			= nullptr;
SDL_Renderer* GameRenderer::renderer		= nullptr;
SDL_Surface* GameRenderer::screenSurface	= nullptr;