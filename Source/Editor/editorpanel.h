#pragma once
#ifndef EDITOR_PANEL_H
#define EDITOR_PANEL_H

class EditorPanel {
public:
	virtual ~EditorPanel() = default;
	virtual void render() = 0;
};

#endif 