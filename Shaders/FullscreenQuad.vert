#version 460 core

// Layout
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUv;

// Outputs
out vec2 uv;

void main()
{
    gl_Position = vec4(aPos, 0.0f, 1.0f);
    uv          = aUv;
}
