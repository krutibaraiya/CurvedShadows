#include "Camera.h"
#include <glm/gtx/rotate_vector.hpp>

/**
 * @brief Method to translate the camera
 */
void Camera::translate(Camera::Direction direction) {
    switch (direction) {
        case Direction::FORWARD:
            position += front * translationSensitivity;
            break;
        case Direction::BACKWARD:
            position -= front * translationSensitivity;
            break;
        case Direction::RIGHT:
            position += right * translationSensitivity;
            break;
        case Direction::LEFT:
            position -= right * translationSensitivity;
            break;
        case Direction::UP:
            position += up * translationSensitivity;
            break;
        case Direction::DOWN:
            position -= up * translationSensitivity;
            break;
    }
    updateVectors();
}
/**
 * Method to update the vectors of the camera view
 */
void Camera::updateVectors() {
    front = glm::normalize(glm::vec3(
            cos(angles.yaw) * cos(angles.pitch),
            sin(angles.pitch),
            sin(angles.yaw) * cos(angles.pitch)
    ));
    right = glm::normalize(glm::cross(front, worldUp));
    auto rolledRight = glm::rotate(right, angles.roll, front);
    up = glm::normalize(glm::cross(rolledRight, front));
}

/**
 * @brief Method to set the position and axis of camera
 */
void Camera::lookAt() {
    auto center = position + front;
    gluLookAt(position.x, position.y, position.z, center.x, center.y, center.z, up.x, up.y, up.z);
}

/**
 * @brief Parametrized constructor to initialise Camera class
 */
Camera::Camera(const glm::vec3 &position, const glm::vec3 &target, const glm::vec3 &worldUp) : position(position),
                                                                                               initPosition(position),
                                                                                               worldUp(worldUp) {
    front = glm::normalize(target - position);
    initAngles.yaw = angles.yaw = atan2f(front.z, front.x);
    initAngles.pitch = angles.pitch = asinf(front.y);
    initAngles.roll = angles.roll = 0;
    updateVectors();
}
/**
 * @brief Method to reset the camera position and axis to initial
 */
void Camera::reset() {
    position = initPosition;
    angles.yaw = initAngles.yaw;
    angles.pitch = initAngles.pitch;
    angles.roll = initAngles.roll;
    zoomFactor = 1;
    updateVectors();
}
/**
 * @brief Method to pitch and yaw the camera
 */
void Camera::rotate(int x, int y) {
    angles.yaw -= rotationSensitivity * (float) x;
    angles.pitch += rotationSensitivity * (float) y;
    angles.pitch = glm::clamp(angles.pitch, -HALF_PI + 0.001f, HALF_PI);
    updateVectors();
}
/**
 * @brief Method to roll the camera in xy-plane
 */
void Camera::rotate(int z) {
    angles.roll += rotationSensitivity * (float) z;
    if (angles.roll >= TWO_PI)
        angles.roll -= TWO_PI;
    else if (angles.roll <= -TWO_PI)
        angles.roll += TWO_PI;
    updateVectors();
}
/**
 * @brief Method to zoom the camera
 */
void Camera::zoom(int y) {
    zoomFactor -= (float) y * zoomSensitivity;
    zoomFactor = std::fmax(zoomFactor, 0.05f);
    zoomFactor = std::fmin(zoomFactor, 20.f);
}
/**
 * @brief Method to get the zoom factor
 */
GLfloat Camera::getZoom() const {
    return zoomFactor;
}

