#include "Framework.h"
#include "Renderer/Backends/SdlGpu/PipelineConfig.h"

#include "Renderer/Backends/SdlGpu/Pipeline.h"

namespace Silent::Renderer
{
    // @todo Temporary placement. Should keep these in a folder as common structs?
    struct BufferVertex
    {
        Vector3 Position = Vector3::Zero;
        Color   Col      = Color::Clear;
    };
    struct BufferPositionTextureVertex
    {
        float x, y, z;
        float u, v;
    };

    const std::vector<PipelineConfig> PIPELINE_CONFIGS =
    {
        PipelineConfig
        {
            .Type                     = PipelineType::Primitive2d,
            .VertexShaderName         = "2dPrimitive.vert",
            .FragmentShaderName       = "2dPrimitive.frag",
            .FragShaderUniBufferCount = 1,
            .VertBufferDescs          =
            {
                SDL_GPUVertexBufferDescription
                {
                    .slot               = 0,
                    .pitch              = sizeof(BufferVertex),
                    .input_rate         = SDL_GPU_VERTEXINPUTRATE_VERTEX,
                    .instance_step_rate = 0
                }
            },
            .VertBufferAttribs =
            {
                SDL_GPUVertexAttribute
                {
                    .location    = 0,
                    .buffer_slot = 0,
                    .format      = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
                    .offset      = 0
                },
                SDL_GPUVertexAttribute
                {
                    .location    = 1,
                    .buffer_slot = 0,
                    .format      = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4,
                    .offset      = sizeof(float) * 3
                }
            }
        },
        PipelineConfig
        {
            .Type                   = PipelineType::Primitive2dTextured,
            .VertexShaderName       = "TexturedQuad.vert",
            .FragmentShaderName     = "TexturedQuad.frag",
            .FragShaderSamplerCount = 1,
            .VertBufferDescs        =
            {
                SDL_GPUVertexBufferDescription
                {
                    .slot               = 0,
                    .pitch              = sizeof(BufferPositionTextureVertex),
                    .input_rate         = SDL_GPU_VERTEXINPUTRATE_VERTEX,
                    .instance_step_rate = 0
                }
            },
            .VertBufferAttribs =
            {
                SDL_GPUVertexAttribute
                {
                    .location    = 0,
                    .buffer_slot = 0,
                    .format      = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
                    .offset      = 0
                },
                SDL_GPUVertexAttribute
                {
                    .location    = 1,
                    .buffer_slot = 0,
                    .format      = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2,
                    .offset      = sizeof(float) * 3
                }
            }
        }
    };
}
