#include "Core/Game.h"
#include "Core/Scene.h"
#include "GameRenderer.h"
#include "Core/Entity.h"
#include "../Collision/ColliderSystem.h"
#include "Core/ComponentPool.h"
#include "Core/Timer.h"
#include "Utility/Color.h"
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

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForSDLRenderer(GameRenderer::window, GameRenderer::renderer);
	ImGui_ImplSDLRenderer2_Init(GameRenderer::renderer);
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
			ImGui_ImplSDL2_ProcessEvent(&e);
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		//ImGui::DockSpaceOverViewport();
		bool showDemo = true;
		ImGui::ShowDemoWindow(&showDemo);
		ImGui::Begin("Hello, world!");
		ImGui::Text("Hello, world!");
		ImGui::End();


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

		
		ImGui::Render();
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
		SDL_RenderPresent(GameRenderer::renderer);

		// Update and Render additional Platform Windows
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}
}
