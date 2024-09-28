#pragma once
#ifndef EDITOR_H
#define EDITOR_H

#include "Event/event.h"
#include "Editor/editorcontext.h"
#include "Scene/Nodes/instance.h"

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
class Scene;

class Editor {
public:
	using CreatorFunction = std::function<std::unique_ptr<Instance>()>;

	Editor(GraphicsContext& graphics, Renderer& renderer, Scene& scene);
	~Editor();

	void update(Scene& scene);
	void render(Scene& scene);
	void openScriptEditor(Script& script);
	void onFileDrop(SDL_Event& event);

	const std::unordered_map<std::string, CreatorFunction>& getCreators() {
		return creators;
	}
private:
	EditorContext m_editorContext;
	GraphicsContext& m_graphics;
	Scene& m_scene;
	Event<Script&>::ConnectionHandle scriptOpenConnection;

	std::vector<std::unique_ptr<EditorPanel>> m_panels;
	std::unordered_map<std::string, CreatorFunction> creators;

	void imguiBegin();
	void imguiEnd();
	void displayMainMenu();
	void displayPanels();

	std::unique_ptr<Instance> create(const std::string& typeName) {
		auto it = creators.find(typeName);
		if (it != creators.end()) {
			return it->second();
		}
		return nullptr;
	}

	void registerType(const std::string& typeName, CreatorFunction creator) {
		creators[typeName] = creator;
	}
};

#endif 