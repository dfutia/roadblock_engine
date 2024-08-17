#include "Graphic/renderer.h"
#include "Graphic/graphicscontext.h"
#include "Graphic/mesh.h"
#include "Graphic/shader.h"

Renderer::Renderer(int width, int height, GraphicsContext& graphics) : 
	m_width(width), m_height(height), 
	m_graphics(graphics) 
{
	m_defaultFramebuffer = std::make_unique<Framebuffer>(width, height);
	m_currentFramebuffer = m_defaultFramebuffer.get();
	glEnable(GL_DEPTH_TEST);
}

void Renderer::initialize() {
}

void Renderer::beginFrame() {
	m_currentFramebuffer->bind();
	glViewport(0, 0, m_width, m_height);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::endFrame() {
	m_currentFramebuffer->unbind();
}

void Renderer::render() {
	static Mesh cube = createCubeMesh(1.0f);
	static Shader shader(vertexShaderSource, fragmentShaderSource);

	shader.use();

	glm::mat4 model = glm::mat4(1.0f);
	shader.setMat4("model", model);

	glm::mat4 view = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 5.0f),  // Camera position
		glm::vec3(0.0f, 0.0f, 0.0f),  // Look at point
		glm::vec3(0.0f, 1.0f, 0.0f)   // Up vector
	);
	shader.setMat4("view", view);

	glm::mat4 projection = glm::perspective(
		glm::radians(70.0f),  // FOV
		1280.0f / 720.0f,      // Aspect ratio
		0.1f,                 // Near plane
		100.0f                // Far plane
	);
	shader.setMat4("projection", projection);

	shader.setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));  // Red color

	glBindVertexArray(cube.vao);
	glDrawElements(GL_TRIANGLES, cube.indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

//void Renderer::renderToScreen() {
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	glViewport(0, 0, m_width, m_height);
//	glClear(GL_COLOR_BUFFER_BIT);
//}

void Renderer::renderToScreen() {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_currentFramebuffer->fbo);  // Bind the custom framebuffer for reading
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);  // Bind the default framebuffer for drawing

	glBlitFramebuffer(
		0, 0, m_width, m_height,  // Source coordinates
		0, 0, m_width, m_height,  // Destination coordinates
		GL_COLOR_BUFFER_BIT,       // What to blit
		GL_NEAREST                 // Filtering method
	);

	// Now the content should be transferred to the screen's framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);  // Ensure the default framebuffer is bound
}

void Renderer::setFramebuffer(Framebuffer* framebuffer) {
	if (framebuffer) {
		m_currentFramebuffer = framebuffer;
	}
	else {
		m_currentFramebuffer = m_defaultFramebuffer.get();
	}
}

Framebuffer* Renderer::getDefaultFramebuffer() {
	return m_defaultFramebuffer.get();
}