#version 330 core
out vec4 FragColor; /// Output fragment color

in vec3 Normal; /// Normal vector
in vec4 FragPos; /// Input fragment position


/**
* Light class
*/
struct Light {
    vec3 lightPos;
    vec3 lightColor;
    mat4 lightSpace;
    sampler2D shadowMap;
    bool enabled;
};


uniform Light whitelight; /// Array of the light sources

/// Poisson Disk
vec2 poissonDisk[16] = vec2[](
vec2(-0.94201624, -0.39906216),
vec2(0.94558609, -0.76890725),
vec2(-0.094184101, -0.92938870),
vec2(0.34495938, 0.29387760),
vec2(-0.91588581, 0.45771432),
vec2(-0.81544232, -0.87912464),
vec2(-0.38277543, 0.27676845),
vec2(0.97484398, 0.75648379),
vec2(0.44323325, -0.97511554),
vec2(0.53742981, -0.47373420),
vec2(-0.26496911, -0.41893023),
vec2(0.79197514, 0.19090188),
vec2(-0.24188840, 0.99706507),
vec2(-0.81409955, 0.91437590),
vec2(0.19984126, 0.78641367),
vec2(0.14383161, -0.14100790)
);

/**
 * Method to get the degree of visibility of a fragment
 */
float getVisibility(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir, sampler2D shadowMap) {
    /// perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    FragColor = vec4(vec3(fragPosLightSpace.z), 1);

    /// normalize to [0,1] range
    /// full formula: (((far-near) * coord) + near + far) / 2.0
    /// we have far = 1, near = 0
    projCoords = projCoords * 0.5 + 0.5;

    /// declare a bias to deal with shadow acne
    float cosTheta = clamp(dot(normal, lightDir), 0.0, 1.0);
    float bias = clamp(0.005 * tan(acos(cosTheta)), 0, 0.01);
    projCoords.z -= bias;
    float visibility = 1.0;
    float spreadParam = 500.0;
    for (int i = 0; i < 16; i++) {
        if (texture(shadowMap, projCoords.xy + poissonDisk[i] / spreadParam).r < projCoords.z){
            visibility -= 0.05;
        }
    }
    return visibility;
}
/**
*@brief method to define light properties
*/
void main() {
    vec3 norm = normalize(Normal);
    vec3 color = vec3(0);

        vec3 ambient = 0.3 * whitelight.lightColor;
        vec3 lightDir = normalize(whitelight.lightPos - FragPos.xyz);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = 0.8 * diff * whitelight.lightColor;
        vec4 fragPosLightSpace = whitelight.lightSpace * FragPos;
        float visibility = getVisibility(fragPosLightSpace, norm, lightDir, whitelight.shadowMap);
        color += (ambient + visibility * diffuse);
 
    FragColor=vec4(color, 1);
}
