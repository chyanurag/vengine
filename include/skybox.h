#pragma once
#include <GL/glew.h>
#include <GL/gl.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"

class Skybox {
    GLuint vbo, vao;
    GLuint texture;
    GLuint loadCubemap(std::vector<std::string> faces);
    public:
        void setup();
        Skybox(std::vector<std::string> faces);
        ~Skybox();
        void draw(glm::mat4 view, glm::mat4 projection, Shader& shader);
};
