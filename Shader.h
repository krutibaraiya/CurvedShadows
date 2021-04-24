#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @brief Class defined for shader
 */
class Shader {
    /**
     * @brief Method defined to read the shader file
     * @param path path to the shader file
     */
    static std::string readShaderFile(const std::string &path);

    /**
     * @brief method defined to compile the shader file
     * @param path path of the shader file
     * @param type type of the shader
     */
    static GLuint compileShader(const std::string &path, GLenum type);

    /**
    * @brief Method implemented to get the location of the uniform
    * @param name name of the uniform
     */
    inline GLuint getLoc(const std::string &name) const;

public:
    GLuint id;

    /**
     * @brief constructor for Shader class
     */
    Shader(const std::string &vertPath, const std::string &fragPath);

    void use();
    /**
     * Methods to set the input to the shaders
     */
    void set(const std::string &name, GLboolean value) const;
    void set(const std::string &name, GLint value) const;
    void set(const std::string &name, GLfloat value) const;
    void set(const std::string &name, const std::vector<GLint> &values) const;
    void set(const std::string &name, const std::vector<GLfloat> &values) const;
    void set(const std::string &name, const glm::vec3 &value) const;
    void set(const std::string &name, const glm::mat4x4 &value) const;
    void setf(const std::string &name, const std::vector<GLfloat> &values) const;
};

#endif
