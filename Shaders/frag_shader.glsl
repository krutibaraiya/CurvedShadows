#version 330 core
out vec4 FragColor; // Output fragment color

in vec3 Normal; // Normal vector
in vec4 FragPos; // Input fragment position

// Light class
struct Light {
    vec3 lightPos;
    vec3 lightColor;
    mat4 lightSpace;
    sampler2D shadowMap;
    bool enabled;
};

//uniform int numLights; // number of lights
//uniform Light whitelight; // Array of the light sources

void main() {
    vec3 norm = normalize(Normal);
    vec3 color = vec3(0);
    int total_enabled = 1;
  
        total_enabled++;
        vec3 ambient = 0.3 * whitelight.lightColor;
        vec3 lightDir = normalize(whitelight.lightPos - FragPos.xyz);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = 0.8 * diff * whitelight.lightColor;
        vec4 fragPosLightSpace = whitelight.lightSpace * FragPos;
        color += (ambient  * diffuse);
    
    color /= 1;
    FragColor = vec4(color, 1);
}
