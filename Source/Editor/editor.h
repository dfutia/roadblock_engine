#pragma once
#ifndef EDITOR_H
#define EDITOR_H

#include <vector>
#include <memory>

class EditorPanel;
class GraphicsContext;

class Editor {
public:
	Editor(GraphicsContext& graphics);
	~Editor();

	void render();
private:
	std::vector<std::unique_ptr<EditorPanel>> m_panels;
	GraphicsContext& m_graphics;
};

#endif 