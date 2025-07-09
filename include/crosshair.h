#pragma once

#include "shader.h"

class Crosshair {
    GLuint vbo, vao;
    float vertices[12] = {
        -0.01f, 0.0f, 0.0f,
        0.01f, 0.0f, 0.0f,
        0.0f, -0.02f, 0.0f,
        0.0f,  0.02f, 0.0f
    };
    public:
        Crosshair();
        void draw(Shader &shader);
        ~Crosshair();
};
