#pragma once
#ifndef EDITOR_H
#define EDITOR_H

#include "Event/event.h"
#include "Editor/editorcontext.h"
#include "Scene/Nodes/instance.h"
#include "Scene/Nodes/script.h"
#include "Scene/Nodes/model.h"
#include "Scene/Nodes/part.h"
#include "Scene/Nodes/meshpart.h"
#include "Scene/Nodes/skybox.h"

#include <SDL2/SDL.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_opengl3.h>

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <functional>

class EditorPanel;
class GraphicsContext;
class Renderer;
class ScriptEditor;
class Script;
class Model;
class Part;
class Scene;
class Audio;

class Editor {
public:
	using CreatorFunction = std::function<std::unique_ptr<Instance>()>;

	Editor(GraphicsContext& graphics, Renderer& renderer, Scene& scene, Audio& audio);
	~Editor();

	void update(Scene& scene);
	void render(Scene& scene);
	void onFileDrop(SDL_Event& event);
private:
	void imguiBegin();
	void imguiEnd();
	void displayMainMenu();
	void displayPanels();
	std::unique_ptr<Instance> create(const std::string& typeName);

	void onOpenScriptEditor(Script& script);

	ScriptEditor* findScriptEditor(const Script& script);

	EditorContext m_editorContext;
	GraphicsContext& m_graphics;
	Scene& m_scene;

	Event<Script&>::ConnectionHandle openScriptConnection;
	Event<int, int>::ConnectionHandle viewportResizeConnection;

	std::list<std::unique_ptr<EditorPanel>> m_panels;

	ImFont* m_largeFont;
};

#endif 