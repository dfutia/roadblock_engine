#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTexCoords;

uniform mat4 model;          // Model matrix
uniform mat4 view;           // View matrix
uniform mat4 projection;     // Projection matrix

out vec3 FragPos;            // Position in world space
out vec2 TexCoords;          // Texture coordinates

void main()
{
    FragPos = vec3(model * vec4(aPosition, 1.0));  // World position
    TexCoords = aTexCoords;                   // Pass texture coordinates to fragment shader
    gl_Position = projection * view * vec4(FragPos, 1.0);  // Final vertex position
}
