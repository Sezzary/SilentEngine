#version 460 core

// Inputs
in vec2 uv;

// Outputs
out vec4 fragColor;

// Uniforms
uniform float     blendAlpha;
uniform sampler2D tex;

float Remap(float val, float fromMin, float fromMax, float toMin, float toMax)
{
    return toMin + (((val - fromMin) * (toMax - toMin)) / (fromMax - fromMin));
}

void main()
{
    vec4 texColor = texture(tex, uv);

    // Compute alpha based on luminance.
    float luminance     = dot(texColor.rgb, vec3(0.299, 0.587, 0.114));
    float remappedAlpha = Remap(blendAlpha, 1.0 - luminance, 1.0, 0.0, 1.0);
    float clampedAlpha  = clamp(remappedAlpha, 0.0, 1.0);

    fragColor = texColor * clampedAlpha;
}
