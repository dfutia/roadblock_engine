#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "Graphic/Rendering/framebuffer.h"

#include <glad/glad.h>

#include <memory>

class GraphicsContext;
class Scene;

class Renderer {
public:
	Renderer(int width, int height, GraphicsContext& graphics);

	void beginFrame();
	void endFrame();
	void render(Scene& scene);

	GLuint getRenderedTextureID() const;
	int getRenderWidth() const { return m_renderWidth; }
	int getRenderHeight() const { return m_renderHeight; }
private:
	GraphicsContext&  m_graphics;
	int m_renderWidth, m_renderHeight;
	std::unique_ptr<Framebuffer> m_currentFramebuffer;
};

#endif 