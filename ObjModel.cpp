#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include "ObjModel.h"

/**
 * brief Constructor of the class ObjModel
 * @param path path of the obj file of the object
 */
ObjModel::ObjModel(const char* path) {
    load(path);
}

/**
 * @brief Method implemented to parse the obj file of the object
 * @param path path path of the obj file of the object
 */
void ObjModel::load(const char* path) {
    Assimp::Importer importer;
    auto scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw std::runtime_error(importer.GetErrorString());
    }
    processNode(scene->mRootNode, scene);
}

/**
 * @brief Method defined to triangulate the vertices
 * @param node pointer to the vertex of the object
 * @param scene root node, meshes, materials
 */
void ObjModel::processNode(aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        auto mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

/**
 * @brief Method implemented to process the mesh
 * @param mesh pointer to mesh
 * @param scene pointer to scene
 */
Mesh ObjModel::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex{};
        vertex.position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
        vertex.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
        if (mesh->mTextureCoords[0]) {
            vertex.uv = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
        } else {
            vertex.uv = {0, 0};
        }
        vertices.push_back(vertex);
    }
    for (int i = 0; i < mesh->mNumFaces; ++i) {
        auto face = mesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }
    return Mesh(vertices, indices);
}

/**
 * brief Method to render the object
 */
void ObjModel::draw() {
    for (auto &mesh : meshes) {
        mesh.draw();
    }
}

/**
 * Constructor for class Mesh
 * @param vertices vector of vertices of the object
 * @param indices vector of indices of the vertices
 */
Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices) : vertices(vertices),
                                                                                            indices(indices) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, uv));
    glBindVertexArray(0);
}
/**
 * @brief Method to render the mesh
 */
void Mesh::draw() {
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
