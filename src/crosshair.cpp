#include "crosshair.h"

Crosshair::Crosshair() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Crosshair::draw(Shader& shader) {
    shader.use();
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, 4);
}

Crosshair::~Crosshair() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}
