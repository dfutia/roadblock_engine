#version 460 core
#define MAX_TEXTURES 4

in vec3 fragPosition;
in vec3 normal;
in vec2 texCoords;

out vec4 fragColor;

// Lighting 
uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform vec3 lightColor;
uniform vec3 objectColor;

// Material 
uniform float shininess;

// Texture
uniform sampler2D textures[MAX_TEXTURES]; 
uniform int numTextures;
uniform bool useTexture;

void main() {
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 ambient = 0.1 * lightColor;
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = spec * lightColor;
    vec3 phongResult = ambient + diffuse + specular;
    vec3 baseColor = objectColor;
        if (useTexture && numTextures > 0) {
        baseColor = vec3(0.0);
        float weight = 1.0 / float(numTextures);
        for (int i = 0; i < numTextures; ++i) {
            baseColor += texture(textures[i], TexCoords).rgb * weight;
        }
    }
    fragColor = vec4(phongResult * baseColor, 1.0);
}