#include "GameRenderer.h"
#include <SDL.h>
#include <iostream>
#include <memory>
#include <utility>

using namespace SimpleECS;

class GameRenderer::GameRendererImpl
{
public:
	GameRendererImpl();
	~GameRendererImpl();

	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Surface* screenSurface = nullptr;
};

// ---------------- GameRenderer definitions ----------------//
void GameRenderer::initGameRenderer()
{
	pImpl = std::make_unique<GameRendererImpl>();
}

std::unique_ptr<GameRenderer::GameRendererImpl> GameRenderer::pImpl = nullptr;

// ---------------- GameRendererImpl definitions ----------------//
GameRenderer::GameRendererImpl::GameRendererImpl()
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

GameRenderer::GameRendererImpl::~GameRendererImpl()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_FreeSurface(screenSurface);
}