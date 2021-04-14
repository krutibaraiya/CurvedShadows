#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec4 FragPos;

void main(){
    vec3 norm = normalize(Normal);
    vec3 color = vec3(0);
    FragColor = vec4(color, 1);
}