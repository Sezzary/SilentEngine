#version 460 core

// Inputs
in vec2 uv;

// Outputs
out vec4 fragColor;

// Uniforms
uniform float     blendAlpha;
uniform sampler2D tex;

void main()
{
    fragColor = mix(texture(tex, uv), vec4(0.0f, 0.0f, 0.0f, 1.0), blendAlpha);
}
