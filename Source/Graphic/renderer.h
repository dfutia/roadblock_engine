#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "Graphic/framebuffer.h"

#include <memory>

class GraphicsContext;

class Renderer {
public:
	Renderer(int width, int height, GraphicsContext& graphics);

	void initialize();
	void beginFrame();
	void endFrame();
	void render();
	void renderToScreen();

	void setFramebuffer(Framebuffer* framebuffer);
	Framebuffer* getDefaultFramebuffer();
private:
	GraphicsContext&  m_graphics;
	int m_width, m_height;
	std::unique_ptr<Framebuffer> m_defaultFramebuffer;
	Framebuffer* m_currentFramebuffer;
};

#endif 