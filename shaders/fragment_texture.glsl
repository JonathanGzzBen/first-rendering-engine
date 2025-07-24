#version 450 core

layout (location = 0) out vec4 fColor;

uniform sampler2D texture_diffuse_1;
uniform sampler2D texture_diffuse_2;
uniform sampler2D texture_diffuse_3;

in vec2 texPos;

void main() {
    fColor = texture(texture_diffuse_1, texPos);
}