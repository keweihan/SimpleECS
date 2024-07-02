#define IMGUI_DEFINE_MATH_OPERATORS // For imvec2 operators

#include "GuiManager.h"
#include "Core/GameRenderer.h"
#include "Core/Game.h"
#include "Core/Entity.h"
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include "RobotoMedium.h"

using namespace UtilSimpleECS;
using namespace SimpleECS;

ImFont* font = nullptr; 

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

	font = io.Fonts->AddFontFromMemoryCompressedTTF(RobotoMedium_compressed_data, RobotoMedium_compressed_size, 13);
	
	applyTheme();
}

void GuiManager::update()
{
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	//ImGui::DockSpaceOverViewport();

	if(font) { ImGui::PushFont(font); }

	bool showDemo = true;

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	ImGui::ShowDemoWindow(&showDemo);
	
	ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_HorizontalScrollbar);
	{
		float portWidth = static_cast<double>(GameRenderer::SCREEN_WIDTH / 1.5);
		float portHeight = static_cast<double>(GameRenderer::SCREEN_HEIGHT / 1.5);
		ImVec2 portSize = ImVec2(portWidth, portHeight);

		ImVec2 center_pos = (ImGui::GetWindowSize() - portSize) * 0.5f;
		ImGui::SetCursorPos(center_pos);
		ImGui::Image((ImTextureID)GameRenderer::gameTexture, portSize);
	};
	ImGui::End();

	ImGui::Begin("Entities");
	{
		std::vector<Entity*> items = Game::getInstance().getCurrentScene()->entities;
		static int item_current_idx = 0; // Here we store our selection data as an index.
		if (ImGui::BeginListBox("##listbox 1", ImVec2(-FLT_MIN, 10 * ImGui::GetTextLineHeightWithSpacing())))
		{
			for (int n = 0; n < items.size(); n++)
			{
				const bool is_selected = (item_current_idx == n);
				if (ImGui::Selectable((items[n]->tag + "##" + std::to_string(n)).c_str(), is_selected))
					item_current_idx = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}
		//static int item_current_idx = 0;
		//
		//if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, 10 * ImGui::GetTextLineHeightWithSpacing())))
		//{
		//	for (int n = 0; n < Game::getInstance().getCurrentScene()->entities.size(); n++)
		//	{
		//		const bool is_selected = (item_current_idx == n);
		//		if (ImGui::Selectable(Game::getInstance().getCurrentScene()->entities[n]->tag.c_str(), is_selected)) {
		//			item_current_idx = n;
		//		}
		//		// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
		//		if (is_selected)
		//			ImGui::SetItemDefaultFocus();
		//	}
		//	ImGui::EndListBox();
		//}
	};
	ImGui::End();

	ImGui::Begin("Statistics");
	{
		ImGui::Text("TODO");
	};
	ImGui::End();

	ImGui::Begin("File Explorer");
	{
		ImGui::Text("TODO");
	};
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

void UtilSimpleECS::GuiManager::applyTheme()
{
	// From: https://github.com/ocornut/imgui/issues/2529
	ImGuiStyle& style = ImGui::GetStyle();
	style.TabRounding = 0.0f;
	style.FrameBorderSize = 1.0f;
	style.ScrollbarRounding = 0.0f;
	style.ScrollbarSize = 10.0f;
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.04f, 0.04f, 0.04f, 0.50f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.12f, 0.12f, 0.12f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.27f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.22f, 0.50f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.27f, 0.75f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.30f, 0.33f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.04f, 0.04f, 0.04f, 0.75f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.18f, 0.18f, 0.19f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.24f, 0.24f, 0.26f, 0.75f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.41f, 0.41f, 0.41f, 0.75f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.62f, 0.62f, 0.62f, 0.75f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.94f, 0.92f, 0.94f, 0.75f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.41f, 0.41f, 0.41f, 0.75f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.62f, 0.62f, 0.62f, 0.75f);
	colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.25f, 0.27f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.18f, 0.18f, 0.19f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.27f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.25f, 0.25f, 0.27f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.62f, 0.62f, 0.62f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.30f, 0.30f, 0.33f, 0.75f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.41f, 0.41f, 0.41f, 0.75f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.62f, 0.62f, 0.62f, 0.75f);
	colors[ImGuiCol_Tab] = ImVec4(0.21f, 0.21f, 0.22f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.37f, 0.37f, 0.39f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.30f, 0.30f, 0.33f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.12f, 0.12f, 0.12f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.18f, 0.18f, 0.19f, 1.00f);
	colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.50f);
	colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.50f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	style.WindowMenuButtonPosition = ImGuiDir_Right;
}
