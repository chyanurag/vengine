#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include "shader.h"
#include "world.h"

class Outline {
    glm::ivec3 hitBlock;
    glm::ivec3 prevBlock;
    World* worldRef;
    glm::mat4 model;
    GLuint vbo, vao;
    float vertices[72] = {
        0, 0, 0, 1, 0, 0,
        1, 0, 0, 1, 0, 1,
        1, 0, 1, 0, 0, 1,
        0, 0, 1, 0, 0, 0,

        0, 1, 0, 1, 1, 0,
        1, 1, 0, 1, 1, 1,
        1, 1, 1, 0, 1, 1,
        0, 1, 1, 0, 1, 0,

        0, 0, 0, 0, 1, 0,
        1, 0, 0, 1, 1, 0,
        1, 0, 1, 1, 1, 1,
        0, 0, 1, 0, 1, 1,
    };
    public:
        Outline(World* world);
        void draw(glm::mat4 view, glm::mat4 projection, Shader& shader);
        void update(glm::vec3 position, glm::vec3 front);
        ~Outline();
};
