#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera {
public:
    glm::vec3 position;
    float yaw, pitch;
    float speed = 5.0f;
    float sensitivity = 0.1f;
    Camera(glm::vec3 startPos) : position(startPos), yaw(-90.f), pitch(0.f) {}
    glm::vec3 getFront() const;
    glm::mat4 getViewMatrix() const;
    void processKeyboard(GLFWwindow* window, int key, float deltaTime);
    void processMouse(float dx, float dy);
};

