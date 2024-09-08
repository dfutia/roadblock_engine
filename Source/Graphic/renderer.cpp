#include "Graphic/renderer.h"
#include "Graphic/graphicscontext.h"
#include "Graphic/texture.h"
#include "Graphic/shader.h"
#include "Graphic/mesh.h"
#include "Graphic/camera.h"
#include "Scene/scene.h"
#include "Scene/SceneGraph/part.h"

Renderer::Renderer(int width, int height, GraphicsContext& graphics) : 
	m_width(width), m_height(height),
	m_graphics(graphics) 
{
	glEnable(GL_DEPTH_TEST);
	m_currentFramebuffer = std::make_unique<Framebuffer>(m_width, m_height);
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

void Renderer::render(Scene& scene) {
	Shader* shader = scene.getShader();
	Camera* camera = scene.getCamera();

	shader->use();

	glm::mat4 model = glm::mat4(1.0f);
	shader->setMat4("model", model);
	shader->setMat4("view", camera->getViewMatrix());
	shader->setMat4("projection", camera->getProjectionMatrix());

	for (auto& instance : scene.getInstances()) {
		Part* part = dynamic_cast<Part*>(instance.get());
		if (part) {
			Mesh& mesh = part->getMesh();
			glBindVertexArray(mesh.vao);
			glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	}
}

GLuint Renderer::getRenderedTextureID() const {
	return m_currentFramebuffer->getTextureID();
}

//static auto texture = loadTextureFromFile("brickwall", "Asset/Textures/brickwall.jpg");
//static Mesh cube = createCubeMesh(1.0f);
//static Shader shader("Asset/Shaders/texture.vert", "Asset/Shaders/texture.frag"); //static Shader shader(vertexShaderSource, fragmentShaderSource);

// bind texture
//glActiveTexture(GL_TEXTURE0);
//glBindTexture(GL_TEXTURE_2D, texture->id);
//shader.setInt("texture1", 0);

// bind mesh
//glBindVertexArray(cube.vao);
//glDrawElements(GL_TRIANGLES, cube.indices.size(), GL_UNSIGNED_INT, 0);
//glBindVertexArray(0);

//const char* vertexShaderSource = R"(
//    #version 460 core
//    layout (location = 0) in vec3 aPos;
//    uniform mat4 model;
//    uniform mat4 view;
//    uniform mat4 projection;
//    void main()
//    {
//        gl_Position = projection * view * model * vec4(aPos, 1.0);
//    }
//)";

//const char* fragmentShaderSource = R"(
//    #version 460 core
//    out vec4 FragColor;
//    uniform vec3 color;
//    void main()
//    {
//        FragColor = vec4(color, 1.0);
//    }
//)";

//glm::mat4 view = glm::lookAt(
//	glm::vec3(0.0f, 0.0f, 5.0f),  // Camera position
//	glm::vec3(0.0f, 0.0f, 0.0f),  // Look at point
//	glm::vec3(0.0f, 1.0f, 0.0f)   // Up vector
//);

//glm::mat4 projection = glm::perspective(
//	glm::radians(70.0f),  // FOV
//	1280.0f / 720.0f,      // Aspect ratio
//	0.1f,                 // Near plane
//	100.0f                // Far plane
//);
