#version 330 core

out vec4 FragColor;

in vec3 color;
in vec2 texCoord;

uniform sampler2D tex0; // For the first texture
uniform sampler2D tex1; // For the second texture
uniform bool useTexture;
uniform int numTextures; // Number of textures to use

void main()
{
    if (useTexture) {
        if (numTextures > 1) {
            vec4 color0 = texture(tex0, texCoord);
            vec4 color1 = texture(tex1, texCoord);
            FragColor = mix(color0, color1, 0.5); // Simple blend, customize as needed
        } else {
            FragColor = texture(tex0, texCoord); // Default to the first texture if only one is present
        }
    } else {
        FragColor = vec4(color, 1.0); // Fallback to solid color
    }
}
