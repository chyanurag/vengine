#version 330 core

out vec4 FragColor;

in vec2 Tex;
in vec3 FragPos;
in float Light;

uniform sampler2D atlas;

uniform vec3 cameraPos;
uniform vec3 fogColor;
uniform float fogStart;
uniform float fogEnd;

void main() {

    float distance = length(FragPos - cameraPos);
    float fogFactor = exp(-distance * 0.02);
    fogFactor = clamp(fogFactor, 0.4, 0.8);
    vec4 baseColor = texture(atlas, Tex);
    float light = clamp(Light, 0.3, .7);
    baseColor *= light;

    // FragColor = mix(vec4(fogColor, 1.0), baseColor, fogFactor);
    FragColor = baseColor;

}
