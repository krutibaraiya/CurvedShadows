#include <fstream>
#include <sstream>
#include "Shader.h"

/**
 * Constructor to the class Shader
 * @param vertPath path to the vertex shader file
 * @param fragPath path to the fragment shader file
 */
Shader::Shader(const std::string &vertPath, const std::string &fragPath) {
    GLuint vertShader = compileShader(vertPath, GL_VERTEX_SHADER), fragShader = compileShader(fragPath, GL_FRAGMENT_SHADER);
    id = glCreateProgram();
    glAttachShader(id, vertShader);
    glAttachShader(id, fragShader);
    glLinkProgram(id);
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (not success) {
        glGetProgramInfoLog(id, 512, nullptr, infoLog);
        std::string msg = std::string("Program linking failed: ") + infoLog;
        throw std::runtime_error(msg);

    }
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
}

/**
 * Method implemented to read the shader file
 * @param path path to the shader file
 */
std::string Shader::readShaderFile(const std::string &path) {
    std::ifstream shaderFile(path);
    if (not shaderFile) {
        throw std::runtime_error("Error while opening shader file at " + path);
    }
    std::stringstream stringstream;
    stringstream << shaderFile.rdbuf();
    shaderFile.close();
    return stringstream.str();
}

/**
 * @brief method implemented to compile the shader
 * @param path path to the shader file
 * @param type type of the shader
 */
GLuint Shader::compileShader(const std::string &path, GLenum type) {
    std::string code = readShaderFile(path);
    GLuint shader = glCreateShader(type);
    const char* cCode = code.c_str();
    glShaderSource(shader, 1, &cCode, nullptr);
    glCompileShader(shader);
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (not success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::string msg = std::string("Shader compilation failed: ") + path + "\n" + infoLog;
        throw std::runtime_error(msg);
    }
    return shader;
}

/**
 * @brief Method implemented to get the location of the uniform
 * @param name name of the uniform
 */
GLuint Shader::getLoc(const std::string &name) const {
    return glGetUniformLocation(id, name.c_str());
}

/**
 * @brief method implemented to use the shader
 */
void Shader::use() {
    glUseProgram(id);
}

/**
 * @brief Method implemented to set the input to the shaders
 * @param name
 * @param value
 */
void Shader::set(const std::string &name, GLboolean value) const {
    glProgramUniform1i(id, getLoc(name), (int) value);
}
void Shader::set(const std::string &name, GLint value) const {
    glProgramUniform1i(id, getLoc(name), value);
}
void Shader::set(const std::string &name, GLfloat value) const {
    glProgramUniform1f(id, getLoc(name), value);
}
void Shader::set(const std::string &name, const std::vector<GLint> &values) const {
    glProgramUniform4iv(id, getLoc(name), 1, values.data());
}
void Shader::set(const std::string &name, const std::vector<GLfloat> &values) const {
    glProgramUniform4fv(id, getLoc(name), 1, values.data());
}
void Shader::set(const std::string &name, const glm::mat4x4 &value) const {
    glProgramUniformMatrix4fv(id, getLoc(name), 1, GL_FALSE, &value[0][0]);
}
void Shader::setf(const std::string &name, const std::vector<GLfloat> &values) const {
    glProgramUniform4fv(id, getLoc(name), 1, values.data());
}
void Shader::set(const std::string &name, const glm::vec3 &value) const {
    glProgramUniform3fv(id, getLoc(name), 1, &value.r);
}
