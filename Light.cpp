#include "Light.h"
/**
 * @brief Constructor to class Light
 * @param position position of the light source
 * @param color colour of the light source
 * @param target target of the light source
 */
Light::Light(const glm::vec3 &position, const glm::vec3 &color, const glm::vec3 &target) :
        position(position),
        color(color),
        target(target),
        model(glm::translate(glm::mat4(1), position)),
        proj_view(glm::mat4(1)),
        isEnabled(true) {}
/**
 * @brief Implementing the method to initialise the camera position
 */
glm::mat4 Light::lookAt() const {
    return glm::lookAt(position, target, {0, 1, 0});
}

/**
 * @brief Implementing the method to get the depth of the scene
 * @param resolution resolution of the depth map
 */
void Light::genDepthMap(unsigned int resolution) {
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
/**
 * @brief method defined to get the position of light source
 */
const glm::vec3 &Light::getPosition() const {
    return position;
}

/**
 * @brief Method defined to get the colour of the light source
 */
const glm::vec3 &Light::getColor() const {
    return color;
}

/**
 * @brief Method defined to get the depth map
 */
GLuint Light::getDepthMap() const {
    return depthMap;
}

/**
 * @brief Method defined to get the model matrix
 */
const glm::mat4 &Light::getModel() const {
    return model;
}
