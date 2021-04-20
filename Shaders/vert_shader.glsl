#version 330 core
layout (location = 0) in vec3 aPos; // Input vertex
layout (location = 1) in vec3 aNormal; // Input normal
layout (location = 2) in vec2 aTexCoords;
 
out vec3 Normal; // output normal
out vec4 FragPos; // fragment position 
out vec2 TexCoords;

uniform mat4 model; // Model matrix
uniform mat4 view; // View Matrix
uniform mat4 projection; // Projection matrix


void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    Normal = transpose(inverse(mat3(model))) * aNormal;
    FragPos = model * vec4(aPos, 1.0);
    TexCoords= aTexCoords;
}
