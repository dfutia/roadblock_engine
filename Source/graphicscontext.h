#pragma once
#ifndef GRAPHICS_CONTEXT_H
#define GRAPHICS_CONTEXT_H

#include <SDL2/SDL.h>
#include <glad/glad.h>

#include <string>

class GraphicsContext {
public:
	GraphicsContext();
	~GraphicsContext();

	bool initialize(const std::string& windowTitle, int width, int height);
	void cleanup();
	void swapBuffers();

	SDL_Window* getWindow() const { return m_window; }
	SDL_GLContext getGLContext() const { return m_glContext; }
private:
	bool initializeSDL();
	bool createWindow(const std::string& windowTitle, int width, int height);
	bool createGLContext();
	bool initializeGLAD();

	SDL_Window* m_window;
	SDL_GLContext m_glContext;
};

#endif 
