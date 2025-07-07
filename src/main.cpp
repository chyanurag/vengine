#include "chunk.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.h"
#include "stb_image.h"

#define SHADER_DIR "shaders/"

Camera camera(glm::vec3(0.f, 40.f, 20.f));
float lastX = 400, lastY = 400;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

double fpsTimer = 0.0;
int frameCount = 0;

void processKeyboard(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(GLFW_KEY_W, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(GLFW_KEY_S, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(GLFW_KEY_A, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(GLFW_KEY_D, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.processKeyboard(GLFW_KEY_SPACE, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        camera.processKeyboard(GLFW_KEY_C, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static float lastX = 400.0f;
    static float lastY = 400.0f;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float dx = xpos - lastX;
    float dy = ypos - lastY;

    lastX = xpos;
    lastY = ypos;

    camera.processMouse(dx, dy);
}


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800, 800, "Minecraft", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSwapInterval(0);



    glewInit();
    glViewport(0, 0, 800, 800);
    glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);

    Chunk chunk;
    Shader shader(SHADER_DIR "main.vert", SHADER_DIR "main.frag");

    glm::mat4 projection = glm::perspective(glm::radians(90.f), 800.f/800.f, .1f, 1000.f);
    glm::mat4 model = glm::mat4(1.f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.f, 0.f, 20.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    int width, height, ch;
    unsigned char* data = stbi_load("assets/atlas.jpg", &width, &height, &ch, 3);
    if (!data) {
        std::cerr << "failed to read atlas\n";
        throw "error reading atlas";
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        frameCount++;
        fpsTimer += deltaTime;

        if (fpsTimer >= 1.0) {
            double fps = frameCount / fpsTimer;

            std::string title = "Minecraft - FPS: " + std::to_string((int)fps);
            glfwSetWindowTitle(window, title.c_str());

            fpsTimer = 0.0;
            frameCount = 0;
        }

        glfwPollEvents();
        processKeyboard(window);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        shader.set_int("atlas", 0);
        shader.set_mat4("model", model);
        shader.set_mat4("projection", projection);
        shader.set_mat4("view", camera.getViewMatrix());
        chunk.draw(shader);

        glfwSwapBuffers(window);
    }



    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
