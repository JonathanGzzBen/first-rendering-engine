#version 450 core

layout (location = 0) in vec3 vPos;
layout (location = 0) in vec3 vNormal;
layout (location = 0) in vec3 vTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(vPos, 1.0);
}