#version 460 core

// Inputs
in vec2 TexCoord;

// Outputs
out vec4 FragColor;

// Uniforms
uniform sampler2D screenTex;

void main()
{
    vec4 color = texture(screenTex, TexCoord);
    FragColor  = color;
}
