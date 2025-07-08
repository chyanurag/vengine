#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include "camera.h"
#include "stb_image.h"
#include "world.h"

#define SHADER_DIR "shaders/"

Camera camera(glm::vec3(0.f, 70.f, 20.f));
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

void mouse_callback(GLFWwindow* _, double xpos, double ypos) {
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

World world;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    glm::ivec3 hitBlock;
    glm::ivec3 prevBlock;

    if (world.raycastBlock(camera.position, camera.getFront(), 100, hitBlock, prevBlock)) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            world.setBlock(hitBlock.x, hitBlock.y, hitBlock.z, Air);
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
            if (world.getWorldBlock(prevBlock.x, prevBlock.y, prevBlock.z) == Air &&
                glm::distance(glm::vec3(prevBlock) + glm::vec3(0.5f), camera.position) > 1.0f) {
                world.setBlock(prevBlock.x, prevBlock.y, prevBlock.z, Stone); // Or selected type
            }
        }
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800, 800, "Minecraft", nullptr, nullptr);

    if (window == NULL) {
        throw "window creation failed\n";
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSwapInterval(0);
    glfwSetMouseButtonCallback(window, mouse_button_callback);


    glewInit();
    glViewport(0, 0, 800, 800);
    glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);

    Shader shader(SHADER_DIR "main.vert", SHADER_DIR "main.frag");
    world.setRenderDistance(4);
    world.setPlayerPos(camera.position);
    world.updateChunksAroundPlayer();
    world.updateChunks();

    glm::mat4 projection = glm::perspective(glm::radians(90.f), 800.f/800.f, .1f, 1000.f);

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
    stbi_image_free(data);

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

        auto start = std::chrono::high_resolution_clock::now();
        processKeyboard(window);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        shader.set_int("atlas", 0);
        shader.set_mat4("projection", projection);
        shader.set_mat4("view", camera.getViewMatrix());

        world.setPlayerPos(camera.position);
        world.updateChunks();
        world.drawChunks(shader);

        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Frame Time: " << std::chrono::duration<float, std::milli>(end - start).count() << "ms\n";

        glfwSwapBuffers(window);
    }


    glDeleteTextures(1, &texture);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
