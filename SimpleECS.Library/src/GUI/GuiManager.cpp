#include "GuiManager.h"
#include "Core/GameRenderer.h"
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

using namespace UtilSimpleECS;


void GuiManager::init()
{
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

void GuiManager::update()
{
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	//ImGui::DockSpaceOverViewport();
	bool showDemo = true;

	ImGui::ShowDemoWindow(&showDemo);

	ImGui::Begin("Viewport");
	float portWidth = static_cast<double>(GameRenderer::SCREEN_WIDTH / 1.5);
	float portHeight = static_cast<double>(GameRenderer::SCREEN_HEIGHT / 1.5);
	ImGui::Image((ImTextureID)GameRenderer::gameTexture, { portWidth, portHeight });
	ImGui::End();
}

void GuiManager::render()
{
	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}
