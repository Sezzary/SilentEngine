#version 460 core

// Inputs
in vec3 vertColor;
in vec2 texCoord;

// Outputs
out vec4 FragColor;

// Uniforms
uniform float     blendAlpha;
uniform sampler2D tex0;
uniform sampler2D tex1;

void main()
{
    //FragColor = vertColor;
    //FragColor = texture(tex, texCoord) * vec4(vertColor, 1.0);
    FragColor = mix(texture(tex0, texCoord), texture(tex1, texCoord), blendAlpha) * vec4(vertColor, 1.0);
}
