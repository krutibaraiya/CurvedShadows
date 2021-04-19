#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

/**
 * @brief Class to define camera
 * @param translationSensitivity Sensitivity for translating the camera
 * @param rotationSensitivity Sensitivity for rotating the camera
 * @param zoomSensitivity Sensitivity for zoom in/out the camera
 */
class Camera {
    GLfloat translationSensitivity = 1;
    GLfloat rotationSensitivity = 0.0025;
    GLfloat zoomSensitivity = 0.1;
    GLfloat zoomFactor = 1;
    glm::vec3 position;
    glm::vec3 initPosition;
    glm::vec3 up;
    glm::vec3 worldUp{0, 1, 0};
    glm::vec3 front;
    glm::vec3 right;

    void updateVectors();

public:

    /**
     * @brief Simple enum to define a direction of movement
     */
    enum class Direction {
        FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN
    };

    constexpr static auto HALF_PI = glm::half_pi<GLfloat>();
    constexpr static auto TWO_PI = glm::two_pi<GLfloat>();

    struct Angles {
        GLfloat yaw;
        GLfloat pitch;
        GLfloat roll;
    } angles, initAngles;

    /**
     * @brief Camera class
     * @param position Initial location of the camera
     * @param target Co-ordinates of where the camera is pointing
     * @param worldUp Up vector for world view
     */
    Camera(const glm::vec3 &position, const glm::vec3 &target, const glm::vec3 &worldUp = {0, 1, 0});

    /**
     * @brief Move the camera along either up, front or right axis.
     * @param direction
     */
    void translate(Camera::Direction direction);

    /**
     * @brief Calculate the final MVP matrix and call gluLookAt. Call this in render loop.
     */
    glm::mat4 lookAt();

    /**
     * @brief Reset the camera view back to initial parameters.
     */
    void reset();

    /**
     * @brief Modify the yaw and pitch of the camera.
     * @param x Horizontal offset of the movement
     * @param y Vertical offset of the movement
     */
    void rotate(int x, int y);

    /**
     * @brief Modify the roll of the camera
     * @param z Rotation about front axis
     */
    void rotate(int z);

    /**
     * @brief Change the perspective angle.
     * @param y Vertical offset of the movement
     */
    void zoom(int y);

    /**
     * @brief Get the zoom factor of the camera, to be used for gluPerspective
     */
    [[nodiscard]] GLfloat getZoom() const;
};
#endif //CAMERA_H