#include "Graphic/Rendering/renderer.h"
#include "Graphic/Rendering/camera.h"
#include "Graphic/graphicscontext.h"
#include "Graphic/Resources/texture.h"
#include "Graphic/Resources/shader.h"
#include "Graphic/Resources/mesh.h"
#include "Scene/scene.h"
#include "Scene/Nodes/part.h"
#include "Scene/Nodes/meshpart.h"
#include "Asset/assetmanager.h"
#include "Asset/textureloader.h"

Renderer::Renderer(int width, int height, GraphicsContext& graphics) : 
	m_renderWidth(width), m_renderHeight(height),
	m_graphics(graphics) 
{
	glEnable(GL_DEPTH_TEST);
	m_currentFramebuffer = std::make_unique<Framebuffer>(m_renderWidth, m_renderHeight);
}

void Renderer::beginFrame() {
	m_currentFramebuffer->bind();
	glViewport(0, 0, m_renderWidth, m_renderHeight);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::endFrame() {
	m_currentFramebuffer->unbind();
}

void Renderer::render(Scene& scene) {
	Shader* shader = scene.getSceneShader();
	Camera* camera = scene.getCamera();
	glm::mat4 view = camera->getViewMatrix();
	glm::mat4 projection = camera->getProjectionMatrix();

	shader->use();

	//glm::mat4 model = glm::mat4(1.0f);
	//shader->setMat4("model", model);
	shader->setMat4("view", view);
	shader->setMat4("projection", projection);
	//shader->setVec3("lightPos", glm::vec3(5.0f, 5.0f, 5.0f));
	//shader->setVec3("viewPos", camera->getPosition());

	for (auto& instance : scene.getInstances()) {
		shader->setMat4("model", instance->transform);

		Part* part = dynamic_cast<Part*>(instance.get());
		MeshPart* meshPart = dynamic_cast<MeshPart*>(instance.get());

		if (part) {
			Mesh& mesh = part->getMesh();
			Material* material = mesh.material;

			if (!material->textures.empty()) {
				shader->setBool("useTexture", true);
			}

			shader->setVec3("objectColor", part->getColor());

			shader->setVec3("material.ambient", material->ambient);
			shader->setVec3("material.diffuse", material->diffuse);
			shader->setVec3("material.specular", material->specular);
			shader->setFloat("material.shininess", material->shininess);

			for (unsigned int i = 0; i < material->textures.size(); i++) {
				glActiveTexture(GL_TEXTURE0 + i); 
				glBindTexture(GL_TEXTURE_2D, material->textures[i]->id);
				std::string uniformName = "textures[" + std::to_string(i) + "]";
				shader->setInt(uniformName, i); 
			}
			shader->setInt("numTextures", material->textures.size());

			glBindVertexArray(mesh.vao);
			glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			for (unsigned int i = 0; i < material->textures.size(); i++) {
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			shader->setInt("numTextures", 0);
		}

		if (meshPart) {
			Mesh& mesh = meshPart->getMesh();
			Material* material = mesh.material;

			if (!material->textures.empty()) {
				shader->setBool("useTexture", true);
			}

			shader->setVec3("objectColor", meshPart->getColor());

			shader->setVec3("material.ambient", material->ambient);
			shader->setVec3("material.diffuse", material->diffuse);
			shader->setVec3("material.specular", material->specular);
			shader->setFloat("material.shininess", material->shininess);

			for (unsigned int i = 0; i < material->textures.size(); i++) {
				glActiveTexture(GL_TEXTURE0 + i); // activate the texture unit
				glBindTexture(GL_TEXTURE_2D, material->textures[i]->id); // bind the texture to the active unit
				std::string uniformName = "textures[" + std::to_string(i) + "]";
				shader->setInt(uniformName, i); // tell shader which unit the texture is at
			}
			shader->setInt("numTextures", material->textures.size());

			glBindVertexArray(mesh.vao);
			glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			for (unsigned int i = 0; i < material->textures.size(); i++) {
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			shader->setInt("numTextures", 0);
		}
	}
}

GLuint Renderer::getRenderedTextureID() const {
	return m_currentFramebuffer->getTextureID();
}

// SKYBOX CODE START
// 	//static Shader skyboxShader("Asset/Shaders/skybox.vert", "Asset/Shaders/skybox.frag");
	//static std::array<std::string, 6> texturePaths = {
	//	"Asset/Textures/skybox/right.jpg",
	//	"Asset/Textures/skybox/left.jpg",
	//	"Asset/Textures/skybox/top.jpg",
	//	"Asset/Textures/skybox/bottom.jpg",
	//	"Asset/Textures/skybox/front.jpg",
	//	"Asset/Textures/skybox/back.jpg"
	//};
	//static auto texture = loadCubemap("skybox", texturePaths);
	//static auto cube = createCubeMesh(1.0f);

	//glDepthFunc(GL_LEQUAL);
	//skyboxShader.use();

	//glm::mat4 skyboxView = glm::mat4(glm::mat3(view));

	//skyboxShader.setMat4("view", skyboxView);
	//skyboxShader.setMat4("projection", projection);

	//// texture
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, gTextureStore.textures["skybox"]->id);
	//skyboxShader.setInt("skybox", 0);

	//// mesh
	//glBindVertexArray(cube.vao);
	//glDrawElements(GL_TRIANGLES, cube.indices.size(), GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0);

	//glDepthFunc(GL_LESS);
// SKYBOX CODE END

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
