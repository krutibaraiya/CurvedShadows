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

uniform int numLights; // number of lights
uniform Light whitelight; // Array of the light sources

void main() {
    vec3 norm = normalize(Normal);
    vec3 color = vec3(0);

    
    //ambient
        vec3 ambient = 0.1 * whitelight.lightColor;

    //diffuse
        vec3 lightDir = normalize(whitelight.lightPos - FragPos.xyz);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * whitelight.lightColor;



        
    vec3 result = (ambient + diffuse ) * color;
 
    FragColor = vec4(result, 1);
}
