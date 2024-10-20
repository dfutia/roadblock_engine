#pragma once
#ifndef MESH_LOADER_H
#define MESH_LOADER_H

#include "Asset/assetloader.h"
#include "Asset/assetmanager.h"
#include "Asset/sources.h"
#include "Graphic/Resources/mesh.h"
#include "filesystem.h"

#include <spdlog/spdlog.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <string>
#include <memory>
#include <vector>

class AssetManager;

class MeshLoader : public AssetLoader<Mesh> {
public:
    std::shared_ptr<MeshHandle> Load(const AssetSource& source, AssetManager& assetManager) override {
        if (std::holds_alternative<FileSource>(source)) {
            return LoadFromFile(std::get<FileSource>(source).filepath);
        }
        else if (std::holds_alternative<MemorySource>(source)) {
            //const auto& memorySource = std::get<MemorySource>(source);
            //if (memorySource.data == nullptr && memorySource.size == 0) {
            //    return std::make_shared<MeshHandle>(new Mesh(createCubeMesh(1.0f)));
            //}
        }
        return nullptr;
    }

private:
    std::string m_dir;
    std::string m_currentModelName;

    std::shared_ptr<MeshHandle> LoadFromFile(const std::string& filepath) {
        std::cout << "loading mesh from file" << std::endl;
        m_dir = gFilesystem.getParentDirectory(filepath);

        Assimp::Importer importer;
        importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

        unsigned flags = aiProcess_Triangulate | aiProcess_SortByPType | aiProcess_JoinIdenticalVertices |
            aiProcess_FixInfacingNormals | aiProcess_GenUVCoords | aiProcess_OptimizeMeshes;
        const aiScene* scene = importer.ReadFile(filepath, flags);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            spdlog::error("Assimp error: {}", importer.GetErrorString());
            return nullptr;
        }

        std::shared_ptr<Mesh> mesh = processNode(scene->mRootNode, scene);
        return std::make_shared<MeshHandle>(mesh, gFilesystem.getFilename(filepath));
    }

    std::shared_ptr<Mesh> processNode(aiNode* node, const aiScene* scene) {
        std::shared_ptr<Mesh> combinedMesh = std::make_shared<Mesh>();

        // Process meshes associated with the current node
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            processMesh(mesh, scene, combinedMesh);
        }

        // Process child nodes
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            std::shared_ptr<Mesh> childMesh = processNode(node->mChildren[i], scene);
            if (childMesh) {
                // Combine child mesh with current mesh
                combinedMesh->vertices.insert(combinedMesh->vertices.end(),
                    childMesh->vertices.begin(), childMesh->vertices.end());
                combinedMesh->indices.insert(combinedMesh->indices.end(),
                    childMesh->indices.begin(), childMesh->indices.end());

                // Merge materials if both meshes have valid materials
                if (childMesh->materialHandle && combinedMesh->materialHandle) {
                    auto childTextures = childMesh->materialHandle->Get()->textureHandles;
                    if (!childTextures.empty()) {
                        combinedMesh->materialHandle->Get()->textureHandles.insert(
                            combinedMesh->materialHandle->Get()->textureHandles.end(),
                            childTextures.begin(),
                            childTextures.end()
                        );
                    }
                }
                else if (childMesh->materialHandle) {
                    // If combined mesh doesn't have a material but child does, use child's material
                    combinedMesh->materialHandle = childMesh->materialHandle;
                }
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

    void processMesh(aiMesh* assimpMesh, const aiScene* scene, std::shared_ptr<Mesh> mesh) {
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
            aiMaterial* aiMaterial = scene->mMaterials[assimpMesh->mMaterialIndex];
            std::string materialName = m_currentModelName + "_Material_" + std::to_string(assimpMesh->mMaterialIndex);

            mesh->materialHandle = g_assetManager.GetAsset<Material>(AiMaterialSource{
                aiMaterial,
                scene,
                m_dir
                });
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
};

#endif
