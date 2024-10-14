#version 460 core
#define MAX_TEXTURES 4

in vec3 FragPos;         // World position from the vertex shader
in vec2 TexCoords;       // Texture coordinates from the vertex shader

out vec4 FragColor;      // Output color of the fragment

// Uniforms for texture and color
uniform sampler2D textures[MAX_TEXTURES];  // Texture array
uniform int numTextures;                   // Number of active textures (up to MAX_TEXTURES)
uniform vec3 objectColor;                  // Base object color
uniform bool useTexture;                   // Whether to use textures

void main()
{
    // Base color setup (without textures or with textures)
    vec3 baseColor = objectColor;
    
    if (useTexture && numTextures > 0) {
        // Blend textures if more than one texture is active
        baseColor = vec3(0.0);  // Initialize base color

        // Loop through active textures and blend them based on their weights
        float weight = 1.0 / float(numTextures);  // Even weight for each texture
        for (int i = 0; i < numTextures; ++i) {
            baseColor += texture(textures[i], TexCoords).rgb * weight;
        }
    }
    
    // Final output color
    FragColor = vec4(baseColor, 1.0);  // No lighting, just texture and color
}
