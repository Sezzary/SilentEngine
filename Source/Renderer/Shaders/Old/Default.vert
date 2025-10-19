#version 460 core

// Layout
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;
layout (location = 1) in vec2 aTexCoord;

// Outputs
out vec3 vertColor;
out vec2 texCoord;

// Uniforms
uniform mat4 modelMat;
uniform mat4 viewMat;

// Constants
const float STEP_SIZE = 1.0 / 32.0;

void main()
{
    gl_Position = (viewMat * modelMat) * vec4(aPos, 1.0);
    //gl_Position.xyz = floor(gl_Position.xyz / STEP_SIZE) * STEP_SIZE;
    vertColor   = vec3(1.0f, 1.0f, 1.0f);//aColor;
    texCoord    = aTexCoord;
}
