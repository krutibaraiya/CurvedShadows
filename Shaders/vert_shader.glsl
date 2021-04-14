#version 330 core

layout (location = 0) in vec3 vPos_modelspace;
layout (location = 1) in vec3 vNormal;

out vec3 Normal;
out vec4 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main() {
    gl_Position = projection * view * model * vec4(vPos_modelspace, 1.0);
    Normal = transpose(inverse(mat3(model))) * vNormal;
    FragPos = model * vec4(vPos_modelspace, 1.0);
}