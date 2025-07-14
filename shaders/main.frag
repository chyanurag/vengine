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

    vec4 baseColor = texture(atlas, Tex);
    float light = clamp(Light, 0.3, 1.0);
    vec3 color = baseColor.rgb;
    color = pow(color, vec3(1.2));
    color *= light * 1.1;
    color = min(color * 1.1, vec3(1.0));
    FragColor = vec4(color, baseColor.a);


}
