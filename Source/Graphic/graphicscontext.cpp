#include "Graphic/graphicscontext.h"

#include <iostream>

GraphicsContext::GraphicsContext() : m_window(nullptr), m_glContext(nullptr) {}

GraphicsContext::~GraphicsContext() {
    cleanup();
}

bool GraphicsContext::initialize(const std::string& windowTitle, int width, int height) {
    if (!initializeSDL()) return false;
    if (!createWindow(windowTitle, width, height)) return false;
    if (!createGLContext()) return false;
    if (!initializeGLAD()) return false;

    return true;
}

void GraphicsContext::cleanup() {
    if (m_glContext) {
        SDL_GL_DeleteContext(m_glContext);
        m_glContext = nullptr;
    }
    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
    SDL_Quit();
}

void GraphicsContext::swapBuffers() {
    SDL_GL_SwapWindow(m_window);
}

bool GraphicsContext::initializeSDL() {
    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

bool GraphicsContext::createWindow(const std::string& windowTitle, int width, int height) {
    m_window = SDL_CreateWindow(windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!m_window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

bool GraphicsContext::createGLContext() {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    m_glContext = SDL_GL_CreateContext(m_window);
    if (!m_glContext) {
        std::cerr << "OpenGL context creation failed: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

bool GraphicsContext::initializeGLAD() {
    if (!gladLoadGL()) {
        std::cerr << "GLAD initialization failed" << std::endl;
        return false;
    }
    return true;
}
