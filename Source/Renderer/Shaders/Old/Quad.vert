#version 460 core

layout (location = 0) in vec2 aPos;

void main()
{
    gl_Position = vec4(aPos.xy / vec2(320, 240) * 2.0 - 1.0, 0.0, 1.0);
}
