#include "camera.h"

glm::vec3 Camera::getFront() const {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    return glm::normalize(front);
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + getFront(), glm::vec3(0.f, 1.f, 0.f));
}

void Camera::processKeyboard(int key, float deltaTime) {
    float velocity = speed * deltaTime;
    glm::vec3 front = getFront();
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.f, 1.f, 0.f)));

    if (key == GLFW_KEY_W)
        position += front * velocity;
    if (key == GLFW_KEY_S)
        position -= front * velocity;
    if (key == GLFW_KEY_A)
        position -= right * velocity;
    if (key == GLFW_KEY_D)
        position += right * velocity;
    if (key == GLFW_KEY_SPACE)
        position += glm::vec3(0.f, 1.f, 0.f) * velocity;
    if (key == GLFW_KEY_C)
        position += glm::vec3(0.f, -1.f, 0.f) * velocity;
}

void Camera::processMouse(float dx, float dy) {
    yaw += dx * sensitivity;
    pitch -= dy * sensitivity;
    if (pitch > 89.f) pitch = 89.f;
    if (pitch < -89.f) pitch = -89.f;
}
