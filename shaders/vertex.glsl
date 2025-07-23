#version 450 core

layout (location = 0) in vec3 vPos;
layout (location = 0) in vec3 vNormal;
layout (location = 0) in vec3 vTexCoords;


void main() {
    gl_Position = vec4(vPos, 1.0);
}