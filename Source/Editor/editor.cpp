#include "Editor/editor.h"
#include "Editor/editorpanel.h"
#include "Editor/Panels/console.h"
#include "Editor/Panels/explorer.h"
#include "Editor/Panels/properties.h"
#include "Editor/Panels/viewport.h"
#include "Editor/Panels/contentbrowser.h"
#include "graphicscontext.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_opengl3.h>

Editor::Editor(GraphicsContext& graphics) : m_graphics(graphics){
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui_ImplSDL2_InitForOpenGL(graphics.getWindow(), graphics.getGLContext());
	ImGui_ImplOpenGL3_Init();

	m_panels.push_back(std::make_unique<Console>());
	m_panels.push_back(std::make_unique<ContentBrowser>());
	m_panels.push_back(std::make_unique<Viewport>());
	m_panels.push_back(std::make_unique<Explorer>());
	m_panels.push_back(std::make_unique<Properties>());
}

Editor::~Editor() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void Editor::render() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	ImGui::ShowDemoWindow();

	for (auto& panel : m_panels) {
		panel->render();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// for imgui docking
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}
}
