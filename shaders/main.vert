#version 330 core

layout (location = 0) in vec3 iPos;
layout (location = 1) in vec2 iTex;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec2 Tex;

void main() {

    gl_Position = projection * view * model * vec4(iPos, 1.0);
    Tex = iTex;

}
