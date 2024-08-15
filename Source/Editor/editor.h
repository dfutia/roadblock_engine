#pragma once
#ifndef EDITOR_H
#define EDITOR_H

#include "Event/event.h"

#include <vector>
#include <memory>

class EditorPanel;
class GraphicsContext;
class ScriptEditor;
class Script;

class Editor {
public:
	Editor(GraphicsContext& graphics);
	~Editor();

	void render();
	void openScriptEditor(Script& script);
private:
	GraphicsContext& m_graphics;
	Event<Script&>::ConnectionHandle scriptOpenConnection;

	std::vector<std::unique_ptr<EditorPanel>> m_panels;
	std::vector<std::unique_ptr<ScriptEditor>> m_scriptEditors;
};

#endif 