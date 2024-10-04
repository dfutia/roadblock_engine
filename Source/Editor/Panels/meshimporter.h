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
            std::cout << "Before adding to scene, mesh has " << mesh->material->textures.size() << " textures" << std::endl;
            addMeshToScene(mesh, filepath);
        }

        //if (scene == nullptr) {
        //    spdlog::error("Assimp Import Error (null scene): {}", importer.GetErrorString());
        //    return nullptr;
        //}

        //if (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
        //    std::string const errorString = importer.GetErrorString();
        //    if (!errorString.empty()) {
        //        spdlog::error("Assimp Import Error (incomplete scene): {}", importer.GetErrorString());
        //        return nullptr;
        //    }
        //    spdlog::warn("Assimp flagged incomplete scene");
        //}

        //if (scene->mRootNode == nullptr) {
        //    spdlog::error("No root node for mesh, cannot process");
        //    return nullptr;
        //}
    }

    Mesh* processNode(aiNode* node, const aiScene* scene) {
        Mesh* combinedMesh = new Mesh();

        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            processMesh(mesh, scene, combinedMesh);
            std::cout << "After processing mesh " << i << ", combined mesh has " << combinedMesh->material->textures.size() << " textures" << std::endl;
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


        std::cout << "processNode: Before returning, combinedMesh has " << combinedMesh->material->textures.size() << " textures" << std::endl;
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
            std::cout << "loading materials" << std::endl;
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


            std::cout << "After loading textures, mesh has " << mesh->material->textures.size() << " textures" << std::endl;
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
            //std::cout << "texture key: " << textureKey << std::endl;
            std::shared_ptr<Texture> texture = gAssetManager.getTexture(textureKey);

            if (!texture) {
                const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(str.C_Str());
                if (embeddedTexture) {
                    //texture = TextureLoader::loadFromMemory(embeddedTexture);
                    std::cout << "Loaded texture from memory" << std::endl;
                }
                else {
                    std::string fullPath = m_dir + '/' + textureKey;
                    texture = TextureLoader::loadFromFile(fullPath);
                    std::cout << "Loaded texture from file: " << fullPath << std::endl;
                }

                if (texture) {
                    gAssetManager.addTexture(textureKey, texture);
                }
            }

            if (texture) {
                std::cout << "adding texture to material" << std::endl;
                mesh->material->textures.push_back(texture);
                std::cout << "texutres in material: " << std::to_string(mesh->material->textures.size()) << std::endl;
            }

            //// Check if texture was loaded before
            //bool skip = false;
            ////for (unsigned int j = 0; j < mesh->material.textures.size(); j++) {
            ////    if (std::strcmp(mesh->material.textures[j]->path.data(), str.C_Str()) == 0) {
            ////        mesh->material.textures.push_back(mesh->material.textures[j]);
            ////        skip = true;
            ////        break;
            ////    }
            ////}
            //if (!skip) {
            //    std::shared_ptr<Texture> texture;

            //    // Check if it's an embedded texture
            //    const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(str.C_Str());
            //    if (embeddedTexture) {
            //        //texture = loadEmbeddedTexture(embeddedTexture, typeName);
            //        std::cout << "load texture from memory" << std::endl;
            //    }
            //    else {
            //        // It's an external texture
            //        //std::string filename = std::string(str.C_Str());
            //        //std::string fullPath = directory + '/' + filename;
            //        //texture = loadTextureFromFile(fullPath, typeName);
            //        std::cout << "load texture from file" << std::endl;
            //    }

            //    if (texture) {
            //        mesh->material.textures.push_back(texture);
            //    }
            //}
        }
    }

    void addMeshToScene(Mesh* mesh, const std::string& filepath) {
        std::string filename = filepath.substr(filepath.find_last_of("/\\") + 1);
        filename = filename.substr(0, filename.find_last_of('.'));

        std::cout << "Textures in mesh before adding to scene: " << mesh->material->textures.size() << std::endl;

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
