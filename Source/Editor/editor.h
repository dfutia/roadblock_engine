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


#include <SDL2/SDL.h>

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
	void openScriptEditor(Script& script);
	void onFileDrop(SDL_Event& event);
private:
	EditorContext m_editorContext;
	GraphicsContext& m_graphics;
	Scene& m_scene;
	Event<Script&>::ConnectionHandle scriptOpenConnection;

	std::vector<std::unique_ptr<EditorPanel>> m_panels;

	void imguiBegin();
	void imguiEnd();
	void displayMainMenu();
	void displayPanels();

	std::unique_ptr<Instance> create(const std::string& typeName) {
		if (typeName == "Script") return std::make_unique<Script>();
		if (typeName == "Part") return std::make_unique<Part>();
		if (typeName == "Model") return std::make_unique<Model>();
		return nullptr;
	}
};

#endif 