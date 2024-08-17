#pragma once
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glad.h>

class Framebuffer {
public:
	GLuint fbo;
	GLuint textureColorBuffer;
	GLuint rbo;

	Framebuffer(int width, int height);
	~Framebuffer();

	void bind();
	void unbind();

	GLuint getTextureID() const;
private:
	//GLuint fbo;
	//GLuint textureColorBuffer;
	//GLuint rbo;
};

#endif 