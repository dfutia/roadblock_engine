#include "Editor/editor.h"
#include "Editor/editorpanel.h"
#include "Editor/Panels/console.h"
#include "Editor/Panels/explorer.h"
#include "Editor/Panels/properties.h"
#include "Editor/Panels/viewport.h"
#include "Editor/Panels/contentbrowser.h"
#include "Editor/Panels/scripteditor.h"
#include "Graphic/graphicscontext.h"
#include "Common/script.h"

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

	//io.Fonts->Clear();
	//io.Fonts->AddFontDefault(new ImFontConfig())->Scale = 1.5f;
	//ImGui_ImplOpenGL3_CreateFontsTexture();

	m_panels.push_back(std::make_unique<Console>());
	m_panels.push_back(std::make_unique<ContentBrowser>());
	m_panels.push_back(std::make_unique<Viewport>());
	m_panels.push_back(std::make_unique<Explorer>());
	m_panels.push_back(std::make_unique<Properties>());

	scriptOpenConnection = EngineEvents::OpenScriptEvent.Connect([this](Script& script) {
		openScriptEditor(script);
	});
}

Editor::~Editor() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	scriptOpenConnection.Disconnect();
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

	for (auto& editor : m_scriptEditors) {
		editor->render();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// update and render additional windows
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

void Editor::openScriptEditor(Script& script) {
	// Check if the script is already open
	auto it = std::find_if(m_scriptEditors.begin(), m_scriptEditors.end(),
		[&script](const std::unique_ptr<ScriptEditor>& editor) {
			return &editor->getScript() == &script;
		});

	if (it == m_scriptEditors.end()) {
		// If not open, create a new TextEditor
		m_scriptEditors.push_back(std::make_unique<ScriptEditor>(script));
	}
}
