#include "Core/Game.h"
#include "Core/Scene.h"
#include "GameRenderer.h"
#include "Core/Entity.h"
#include "../Collision/ColliderSystem.h"
#include "Core/ComponentPool.h"
#include "Core/Timer.h"
#include "Utility/Color.h"
#include "GUI/GuiManager.h"
#include <SDL.h>

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

#include <iostream>

using namespace SimpleECS;
using namespace UtilSimpleECS;



Scene* Game::getCurrentScene()
{
	return sceneList[activeSceneIndex];
}

Game::Game()
{
	GameRenderer::SCREEN_WIDTH = 640;
	GameRenderer::SCREEN_HEIGHT = 480;
}

void Game::configureWindow(int width, int height)
{
	GameRenderer::SCREEN_WIDTH = width;
	GameRenderer::SCREEN_HEIGHT = height;
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

void SimpleECS::Game::setName(std::string name)
{
	gameName = name;
}

void Game::init()
{
	GameRenderer::initGameRenderer();
	SDL_SetWindowTitle(GameRenderer::window, gameName.c_str());
	GuiManager::getInstance().init();
}

void Game::mainLoop()
{
	SDL_Event e;
	bool quit = false;

	if (sceneList.size() == 0) {
		throw std::runtime_error("No scenes in game! ");
	}

	// Run initialize of first scene components
	for (auto& pool : sceneList[0]->getComponentPools())
	{
		(*pool).invokeStart();
	}

	// Game loop
	while (!quit)
	{
		// Check for closing window
		while (SDL_PollEvent(&e))
		{
			// TODO: Move/abstract logic to GuiManager.
			ImGui_ImplSDL2_ProcessEvent(&e);

			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		// Define engine GUI components
		GuiManager::getInstance().update();

		//Clear screen
		Color sceneColor = sceneList[0]->backgroundColor;
		SDL_SetRenderDrawColor(GameRenderer::renderer, sceneColor.r, sceneColor.g, sceneColor.b, sceneColor.a);
		SDL_RenderClear(GameRenderer::renderer);

		// Run update of first scene functions
		for (auto& pool : sceneList[0]->getComponentPools())
		{
			(*pool).invokeUpdate();
		}

		// Run collision functions
		ColliderSystem::getInstance().invokeCollisions();

		// Run late update
		for (auto& pool : sceneList[0]->getComponentPools())
		{
			(*pool).invokeLateUpdate();
		}

		// Delete objects
		sceneList[0]->destroyAllMarkedEntities();

		// Mark end of frame
		Timer::endFrame();
	
		// Render engine GUI components
		GuiManager::getInstance().render();
		SDL_RenderPresent(GameRenderer::renderer);
	}
}
