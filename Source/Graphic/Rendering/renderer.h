#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include <Graphic/framebuffer.h>

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
private:
	GraphicsContext&  m_graphics;
	int m_width, m_height;
	std::unique_ptr<Framebuffer> m_currentFramebuffer;
};

#endif 