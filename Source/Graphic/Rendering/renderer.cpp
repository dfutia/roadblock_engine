#include "Graphic/Rendering/renderer.h"
#include "Graphic/Rendering/camera.h"
#include "Graphic/graphicscontext.h"
#include "Graphic/Resources/texture.h"
#include "Graphic/Resources/shader.h"
#include "Graphic/Resources/mesh.h"
#include "Scene/scene.h"
#include "Scene/Nodes/part.h"
#include "Scene/Nodes/meshpart.h"
#include "Scene/Nodes/skybox.h"
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
	Camera* camera = scene.getCamera();
	glm::mat4 view = camera->getViewMatrix();
	glm::mat4 projection = camera->getProjectionMatrix();

	renderSkybox(scene, view, projection);
	renderScene(scene, view, projection);
}

void Renderer::renderSkybox(Scene& scene, const glm::mat4& view, const glm::mat4& projection) {
	for (auto& instance : scene.getInstances()) {
		Skybox* skybox = dynamic_cast<Skybox*>(instance.get());
		if (skybox) {
			glDepthFunc(GL_LEQUAL);
			Mesh& mesh = skybox->getMesh();
			Shader* shader = skybox->getShader();
			shader->use();
			glm::mat4 skyboxView = glm::mat4(glm::mat3(view)); // Remove translation from the view matrix
			shader->setMat4("view", skyboxView);
			shader->setMat4("projection", projection);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getTexture()->getId());
			shader->setInt("skybox", 0);
			glBindVertexArray(mesh.vao);
			glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			glDepthFunc(GL_LESS);
			break; // Assuming only one skybox
		}
	}
}

void Renderer::renderScene(Scene& scene, const glm::mat4& view, const glm::mat4& projection) {
	Shader* shader = scene.getSceneShader();
	shader->use();
	shader->setMat4("view", view);
	shader->setMat4("projection", projection);

	for (auto& instance : scene.getInstances()) {
		BasePart* basePart = dynamic_cast<BasePart*>(instance.get());
		if (basePart) {
			Mesh mesh = basePart->getMesh();
			Material* material = mesh.material;
			shader->setMat4("model", basePart->getTransform());
			if (!material->textures.empty()) {
				shader->setBool("useTexture", true);
			}
			shader->setVec3("objectColor", basePart->getColor());
			shader->setVec3("material.ambient", material->ambient);
			shader->setVec3("material.diffuse", material->diffuse);
			shader->setVec3("material.specular", material->specular);
			shader->setFloat("material.shininess", material->shininess);
			for (unsigned int i = 0; i < material->textures.size(); i++) {
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, material->textures[i]->getId());
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
	}
}

GLuint Renderer::getRenderedTextureID() const {
	return m_currentFramebuffer->getTextureID();
}