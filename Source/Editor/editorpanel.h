#pragma once
#ifndef EDITOR_PANEL_H
#define EDITOR_PANEL_H

#include <string>

class EditorPanel {
public:
	EditorPanel(bool open = false, std::string name = "Untitled") : m_open(open), m_name(name) {}
	virtual ~EditorPanel() = default;
	virtual void render() = 0;

	std::string getName() { return m_name; }

	void open() { m_open = true; }
	void close() { m_open = false; }
	bool isVisible() { return m_open; }
protected:
	std::string m_name;
	bool m_open;
};

#endif 