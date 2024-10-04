#pragma once
#ifndef MESH_IMPORTER_H
#define MESH_IMPORTER_H

#include "Editor/editorpanel.h"
#include "Editor/editorcontext.h"
#include "Scene/scene.h"
#include "Asset/assetmanager.h"
#include "Asset/textureloader.h"

#include <spdlog/spdlog.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

class MeshImporter : public EditorPanel {
public:
    MeshImporter(Scene& scene, EditorContext& editorContext, bool open = false, std::string name = "Mesh Importer") : EditorPanel(open, name), m_scene(scene), m_editorContext(editorContext) {}

    void render() override {
        ImGui::Begin(m_name.c_str(), &m_open);

        if (ImGui::Button("Import Mesh")) {
            std::wstring filepath = gFilesystem.openFileDialog();
            if (!filepath.empty()) {
                std::string path(filepath.begin(), filepath.end());
                m_dir = gFilesystem.getParentDirectory(path);
                importMesh(path);
            }
        }

        ImGui::End();
    }
private:
    EditorContext& m_editorContext;
    Scene& m_scene;

    std::string m_dir;

    void importMesh(const std::string& filepath) {
        Assimp::Importer importer;
        importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

        unsigned flags = aiProcess_Triangulate | aiProcess_SortByPType | aiProcess_JoinIdenticalVertices |
            aiProcess_FixInfacingNormals | aiProcess_GenUVCoords | aiProcess_OptimizeMeshes;
        const aiScene* scene = importer.ReadFile(filepath, flags);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            spdlog::error("Assimp error: {}", importer.GetErrorString());
            return;
        }

        Mesh* mesh = processNode(scene->mRootNode, scene);
        if (mesh) {
            addMeshToScene(mesh, filepath);
        }
    }

    Mesh* processNode(aiNode* node, const aiScene* scene) {
        Mesh* combinedMesh = new Mesh();

        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            processMesh(mesh, scene, combinedMesh);
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            Mesh* childMesh = processNode(node->mChildren[i], scene);
            if (childMesh) {
                // Combine child mesh with current mesh
                combinedMesh->vertices.insert(combinedMesh->vertices.end(),
                    childMesh->vertices.begin(), childMesh->vertices.end());
                combinedMesh->indices.insert(combinedMesh->indices.end(),
                    childMesh->indices.begin(), childMesh->indices.end());

                // Merge materials
                if (!childMesh->material->textures.empty()) {
                    combinedMesh->material->textures.insert(
                        combinedMesh->material->textures.end(),
                        childMesh->material->textures.begin(),
                        childMesh->material->textures.end()
                    );
                }

                delete childMesh;
            }
        }

        glGenVertexArrays(1, &combinedMesh->vao);
        glGenBuffers(1, &combinedMesh->vbo);
        glGenBuffers(1, &combinedMesh->ebo);

        glBindVertexArray(combinedMesh->vao);

        glBindBuffer(GL_ARRAY_BUFFER, combinedMesh->vbo);
        glBufferData(GL_ARRAY_BUFFER, combinedMesh->vertices.size() * sizeof(Vertex), combinedMesh->vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, combinedMesh->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, combinedMesh->indices.size() * sizeof(unsigned int), combinedMesh->indices.data(), GL_STATIC_DRAW);

        // Vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // Vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        // Vertex colors
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

        // Vertex texture coords
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

        glBindVertexArray(0);

        return combinedMesh;
    }

    void processMesh(aiMesh* assimpMesh, const aiScene* scene, Mesh* mesh) {
        for (unsigned int i = 0; i < assimpMesh->mNumVertices; i++) {
            Vertex vertex;

            vertex.position.x = assimpMesh->mVertices[i].x;
            vertex.position.y = assimpMesh->mVertices[i].y;
            vertex.position.z = assimpMesh->mVertices[i].z;

            if (assimpMesh->HasNormals()) {
                vertex.normal.x = assimpMesh->mNormals[i].x;
                vertex.normal.y = assimpMesh->mNormals[i].y;
                vertex.normal.z = assimpMesh->mNormals[i].z;
            }

            if (assimpMesh->mTextureCoords[0]) {
                vertex.texCoords.x = assimpMesh->mTextureCoords[0][i].x;
                vertex.texCoords.y = assimpMesh->mTextureCoords[0][i].y;
            }
            else {
                vertex.texCoords = glm::vec2(0.0f, 0.0f);
            }

            mesh->vertices.push_back(vertex);
        }

        for (unsigned int i = 0; i < assimpMesh->mNumFaces; i++) {
            aiFace face = assimpMesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                mesh->indices.push_back(face.mIndices[j]);
            }
        }

        if (assimpMesh->mMaterialIndex >= 0) {
            aiMaterial* material = scene->mMaterials[assimpMesh->mMaterialIndex];

            aiColor3D color(0.0f, 0.0f, 0.0f);
            float shininess;

            material->Get(AI_MATKEY_COLOR_AMBIENT, color);
            mesh->material->ambient = glm::vec3(color.r, color.g, color.b);

            material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
            mesh->material->diffuse = glm::vec3(color.r, color.g, color.b);

            material->Get(AI_MATKEY_COLOR_SPECULAR, color);
            mesh->material->specular = glm::vec3(color.r, color.g, color.b);

            material->Get(AI_MATKEY_SHININESS, shininess);
            mesh->material->shininess = shininess;

            loadMaterialTextures(mesh, material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
            loadMaterialTextures(mesh, material, aiTextureType_SPECULAR, "texture_specular", scene);
            loadMaterialTextures(mesh, material, aiTextureType_HEIGHT, "texture_normal", scene);
        }

        glGenVertexArrays(1, &mesh->vao);
        glGenBuffers(1, &mesh->vbo);
        glGenBuffers(1, &mesh->ebo);

        glBindVertexArray(mesh->vao);

        glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
        glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Vertex), mesh->vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(unsigned int), mesh->indices.data(), GL_STATIC_DRAW);

        // Vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // Vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        // Vertex colors
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

        // Vertex texture coords
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

        glBindVertexArray(0);
    }

    void loadMaterialTextures(Mesh* mesh, aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene) {
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);

            std::string textureKey = std::string(str.C_Str());
            std::shared_ptr<Texture> texture = gAssetManager.getTexture(textureKey);

            if (!texture) {
                const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(str.C_Str());
                if (embeddedTexture) {
                    //texture = TextureLoader::loadFromMemory(embeddedTexture);
                }
                else {
                    std::string fullPath = m_dir + '/' + textureKey;
                    texture = TextureLoader::loadFromFile(fullPath);
                }

                if (texture) {
                    gAssetManager.addTexture(textureKey, texture);
                }
            }

            if (texture) {
                mesh->material->textures.push_back(texture);
            }
        }
    }

    void addMeshToScene(Mesh* mesh, const std::string& filepath) {
        std::string filename = filepath.substr(filepath.find_last_of("/\\") + 1);
        filename = filename.substr(0, filename.find_last_of('.'));

        auto part = std::make_unique<Part>(mesh);
        part->name = filename;
        part->setParent(m_editorContext.test);

        // Add the part to the scene
        m_scene.addInstance(std::move(part));

        // Clean up the mesh (assuming the Part makes a copy)
        //delete mesh;
    }
};

#endif 
