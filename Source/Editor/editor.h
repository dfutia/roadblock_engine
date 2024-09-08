#pragma once
#ifndef EDITOR_H
#define EDITOR_H

#include "Event/event.h"
#include "Editor/editorcontext.h"

#include <SDL2/SDL.h>

#include <vector>
#include <memory>

class EditorPanel;
class GraphicsContext;
class Renderer;
class ScriptEditor;
class Script;
class Scene;

class Editor {
public:
	Editor(GraphicsContext& graphics, Renderer& renderer);
	~Editor();

	void update(Scene& scene);
	void render();
	void openScriptEditor(Script& script);
	void onFileDrop(SDL_Event& event);
private:
	EditorContext m_editorContext;
	GraphicsContext& m_graphics;
	Event<Script&>::ConnectionHandle scriptOpenConnection;

	std::vector<std::unique_ptr<EditorPanel>> m_panels;
	std::vector<std::unique_ptr<ScriptEditor>> m_scriptEditors;
};

#endif 