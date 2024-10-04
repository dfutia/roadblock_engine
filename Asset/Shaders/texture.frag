#version 460 core
#define MAX_TEXTURES 4

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D textures[MAX_TEXTURES]; 
uniform int numTextures;

void main() {
    vec4 texColor = vec4(0.0);
    for (int i = 0; i < numTextures && i < MAX_TEXTURES; i++) {
        texColor += texture(textures[i], texCoord) * (1.0 / numTextures);
    }

    FragColor = texColor;
}
