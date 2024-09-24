#include "Editor/editor.h"
#include "Editor/editorpanel.h"
#include "Editor/Panels/console.h"
#include "Editor/Panels/explorer.h"
#include "Editor/Panels/properties.h"
#include "Editor/Panels/viewport.h"
#include "Editor/Panels/contentbrowser.h"
#include "Editor/Panels/scripteditor.h"
#include "Editor/Panels/camerasettings.h"
#include "Graphic/graphicscontext.h"
#include "Graphic/renderer.h"
#include "Scene/scene.h"
#include "Scene/SceneGraph/script.h"
#include "filesystem.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_opengl3.h>

Editor::Editor(GraphicsContext& graphics, Renderer& renderer, Scene& scene) : m_graphics(graphics), m_scene(scene) {
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

	registerType("Script", []() { return std::make_unique<Script>(); });
	registerType("Part", []() { return std::make_unique<Part>(); });
	registerType("Model", []() { return std::make_unique<Model>(); });

	m_panels.push_back(std::make_unique<Console>());
	//m_panels.push_back(std::make_unique<ContentBrowser>());
	m_panels.push_back(std::make_unique<Viewport>(renderer));
	m_panels.push_back(std::make_unique<Properties>(m_editorContext));
	m_panels.push_back(std::make_unique<Explorer>(*this, m_editorContext));
	m_panels.push_back(std::make_unique<CameraSettings>(m_scene.getCamera()));

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

void Editor::update(Scene& scene) {
	if (m_editorContext.action == EditorAction::ADD_INSTANCE) {
		auto newInstance = create(m_editorContext.instanceTypeToAdd);
		if (newInstance) {
			newInstance->setParent(m_editorContext.targetInstance);
			scene.addInstance(std::move(newInstance));
		}
	}
	m_editorContext.action = EditorAction::NONE;
	m_editorContext.targetInstance = nullptr;
	m_editorContext.instanceTypeToAdd.clear();
}

void Editor::render(Scene& scene) {	
	imguiBegin();
	displayMainMenu();
	displayPanels();
	imguiEnd();
}

void Editor::imguiBegin() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void Editor::imguiEnd() {
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

void Editor::displayMainMenu() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("New")) {

			}
			if (ImGui::MenuItem("Open")) {

			}
			if (ImGui::MenuItem("Save")) {

			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			for (auto& panel : m_panels) {
				if (ImGui::MenuItem(panel->getName().c_str(), nullptr, panel->isVisible())) {
					if (panel->isVisible()) {
						panel->close();
					}
					else {
						panel->open();
					}
				}
			}
			ImGui::EndMenu();
		}

		static bool m_playMode = false;
		if (ImGui::Button(m_playMode ? "Stop" : "Play")) {
			
		}

		ImGui::EndMainMenuBar();
	}
}

void Editor::displayPanels() {
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	ImGui::ShowDemoWindow();

	for (auto& panel : m_panels) {
		if (!panel->isVisible()) { continue; }
		panel->render();
	}
}

void Editor::openScriptEditor(Script& script) {
	// Check if the script is already open
	auto it = std::find_if(m_panels.begin(), m_panels.end(),
		[&script](const std::unique_ptr<EditorPanel>& panel) {
			auto* editor = dynamic_cast<ScriptEditor*>(panel.get());
			return editor && &editor->getScript() == &script;
		});

	if (it == m_panels.end()) {
		// If not open, create a new ScriptEditor
		m_panels.push_back(std::make_unique<ScriptEditor>(script));
	}
}

void Editor::onFileDrop(SDL_Event& event) {
	char* filepath = event.drop.file;
	std::cout << "file dropped: " << filepath << std::endl;

	std::string filePath(filepath);
	std::string extension = filePath.substr(filePath.find_last_of(".") + 1);

	if (extension == "png" || extension == "jpg" || extension == "jpeg") {
		std::cout << "Creating texture object..." << std::endl;
	}
	else if (extension == "obj") {
		std::cout << "Creating mesh object..." << std::endl;
	}
	else {
		std::cout << "Unsupported file type: " << extension << std::endl;
	}

	SDL_free(filepath);
}
