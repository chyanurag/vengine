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

void Camera::processKeyboard(GLFWwindow* window, int key, float deltaTime) {
    float velocity = speed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        velocity *= 5.0f;

    glm::vec3 front = getFront();
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.f, 1.f, 0.f)));

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position += front * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position -= front * velocity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position -= right * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position += right * velocity;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        position += glm::vec3(0.f, 1.f, 0.f) * velocity;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        position += glm::vec3(0.f, -1.f, 0.f) * velocity;
}


void Camera::processMouse(float dx, float dy) {
    yaw += dx * sensitivity;
    pitch -= dy * sensitivity;
    if (pitch > 89.f) pitch = 89.f;
    if (pitch < -89.f) pitch = -89.f;
}
