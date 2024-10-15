#include "Editor/editor.h"
#include "Editor/editorpanel.h"
#include "Editor/Panels/console.h"
#include "Editor/Panels/explorer.h"
#include "Editor/Panels/properties.h"
#include "Editor/Panels/viewport.h"
#include "Editor/Panels/contentbrowser.h"
#include "Editor/Panels/scripteditor.h"
#include "Editor/Panels/camerasettings.h"
#include "Editor/Panels/audioimporter.h"
#include "Editor/Panels/meshimporter.h"
#include "Editor/Panels/material.h"
#include "Graphic/graphicscontext.h"
#include "Graphic/Rendering/renderer.h"
#include "Scene/scene.h"
#include "Scene/Nodes/script.h"
#include "filesystem.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_opengl3.h>

Editor::Editor(GraphicsContext& graphics, Renderer& renderer, Scene& scene, Audio& audio) : m_graphics(graphics), m_scene(scene) {
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
	m_panels.push_back(std::make_unique<Viewport>(renderer, scene));
	m_panels.push_back(std::make_unique<Properties>(m_editorContext));
	m_panels.push_back(std::make_unique<Explorer>(*this, m_editorContext));
	m_panels.push_back(std::make_unique<CameraSettings>(m_scene.getCamera()));
	m_panels.push_back(std::make_unique<AudioImporter>(audio));
	m_panels.push_back(std::make_unique<MeshImporter>(m_scene, m_editorContext));
	m_panels.push_back(std::make_unique<MaterialPanel>());

	auto it = m_panels.begin();
	std::advance(it, 4);
	if (auto explorer = dynamic_cast<Explorer*>(it->get())) {
		openScriptConnection = explorer->openScriptEvent.Connect([this](Script& script) {
			std::cout << "received openScript event" << std::endl;
			onOpenScriptEditor(script);
			});
	}
}

Editor::~Editor() {
	openScriptConnection.Disconnect();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void Editor::update(Scene& scene) {
	if (m_editorContext.action == EditorAction::ADD_INSTANCE) {
		auto newInstance = gTypeRegistry.createInstance(m_editorContext.instanceTypeToAdd);
		if (newInstance) {
			newInstance->setParent(m_editorContext.targetInstance);
			scene.addInstance(std::move(newInstance));
		}
	}

	// Reset editor context
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
				std::wstring chosenPath = gFilesystem.openFolderDialog();
				if (!chosenPath.empty()) {
					gFilesystem.makeDirectory(chosenPath);
				}
			}
			if (ImGui::MenuItem("Open")) {
				gFilesystem.openFolderDialog();
			}
			if (ImGui::MenuItem("Save")) {
				// save project
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
		if (panel && panel->isVisible()) {
			panel->render();
		}
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

std::unique_ptr<Instance> Editor::create(const std::string& typeName) {
	if (typeName == "Script") return std::make_unique<Script>();
	if (typeName == "Part") return std::make_unique<Part>();
	if (typeName == "Model") return std::make_unique<Model>();
	return nullptr;
}

void Editor::onOpenScriptEditor(Script& script) {
	ScriptEditor* editorExisits = findScriptEditor(script);
	if (!editorExisits) {
		auto scriptEditor = std::make_unique<ScriptEditor>(script);
		m_panels.push_back(std::move(scriptEditor));
	}
}

ScriptEditor* Editor::findScriptEditor(const Script& script) {
	auto it = std::find_if(m_panels.begin(), m_panels.end(),
		[&script](const std::unique_ptr<EditorPanel>& panel) {
			auto* editor = dynamic_cast<ScriptEditor*>(panel.get());
			return editor && &editor->getScript() == &script;
		});

	return it != m_panels.end() ? dynamic_cast<ScriptEditor*>(it->get()) : nullptr;
}