#include "block_selector.h"

BlockType BlockSelector::getCurrent() {
    return type;
}

void BlockSelector::update(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        type = Dirt; 
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        type = Grass; 
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        type = Stone; 
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        type = Log; 
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        type = Plank; 
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
        type = Cobblestone; 
}
