#include "outline.h"

Outline::Outline(World* world) : worldRef(world) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Outline::draw(glm::mat4 view, glm::mat4 projection, Shader& shader) {
    shader.use();
    shader.set_mat4("projection", projection);
    shader.set_mat4("view", view);
    model = glm::translate(glm::mat4(1.0f), glm::vec3(hitBlock));
    model = glm::scale(model, glm::vec3(1.02f));
    shader.set_mat4("model", model);

    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, 24);
}

void Outline::update(glm::vec3 position, glm::vec3 front) {
    worldRef->raycastBlock(position, front, 100, hitBlock, prevBlock);
}

Outline::~Outline() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}
