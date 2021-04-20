#ifndef OBJMODEL_H
#define OBJMODEL_H

#include <vector>
#include<string>
#include <glm/glm.hpp>
#include <assimp/scene.h>

/**
 * @brief Vertex representation structure
 */
struct Vertex {
    glm::vec3 position; // xyz coordinates
    glm::vec3 normal;   // Normal vector
    glm::vec2 texcoords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

/**
 * @brief Class to represent Mesh information
 */
class Mesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>textures;
    unsigned int vao{0}, vbo{0}, ebo{0};

public:
    /**
     * @brief Constructor for the Mesh class
     * @param vertices List of vertices of the model
     * @param indices Indices of the faces
     */
    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices,const std::vector<Texture> &textures);

    void draw(Shader &shader);
};

class ObjModel {
    std::vector<Mesh> meshes;
    std::vector<Texture> textures_loaded;
    std::string directory;
    bool gammaCorrection;

    /**
     * @brief Parse and load the .obj file using ASSIMP.
     * @param path
     */
    void load(const char* path);

    /**
     * @brief Method to process a single node
     * @param node Pointer to the node
     * @param scene Pointer to the scene
     */
    void processNode(aiNode* node, const aiScene* scene);

    /**
     * @brief Method to process a mesh
     * @param mesh Pointer to the mesh 
     * @param scene Pointer to the scene
     */
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);

public:
    /**
     * @brief Constructor for the ObjModel class
     * @param path The path to the object to be loaded
     */
    explicit ObjModel(const char* path);

    /**
     * @brief Draws all meshes of the model
     */
    void draw(Shader &shader);
};

#endif //OBJMODEL_H
