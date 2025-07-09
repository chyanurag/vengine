#version 330 core

layout (location = 0) in vec3 iPos;
layout (location = 1) in vec2 iTex;
layout(location = 2) in float iLight;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec2 Tex;
out vec3 FragPos;
out float Light;

void main() {

    gl_Position = projection * view * model * vec4(iPos, 1.0);
    Tex = iTex;
    FragPos = (model * vec4(iPos, 1.0)).xyz;
    Light = iLight;

}
