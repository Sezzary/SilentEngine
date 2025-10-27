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
        Vector3 Position = Vector3::Zero;
        Vector2 Uv       = Vector2::Zero;
    };

    const std::vector<SDL_GPUColorTargetBlendState> PIPELINE_BLEND_MODE_COLOR_TARGETS = 
    {
        // Opaque.
        SDL_GPUColorTargetBlendState
        {
            .src_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
            .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ZERO,
            .color_blend_op        = SDL_GPU_BLENDOP_ADD,
            .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
            .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ZERO,
            .alpha_blend_op        = SDL_GPU_BLENDOP_ADD,
            .enable_blend          = true
        },
        // Alpha.
        SDL_GPUColorTargetBlendState
        {
            .src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
            .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
            .color_blend_op        = SDL_GPU_BLENDOP_ADD,
            .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
            .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
            .alpha_blend_op        = SDL_GPU_BLENDOP_ADD,
            .enable_blend          = true
        },
        // Fast alpha. Requires `IsFastAlpha` shader uniform set to `true`.
        SDL_GPUColorTargetBlendState
        {
            .src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
            .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
            .color_blend_op        = SDL_GPU_BLENDOP_ADD,
            .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
            .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
            .alpha_blend_op        = SDL_GPU_BLENDOP_ADD,
            .enable_blend          = true
        },
        // Multiply.
        SDL_GPUColorTargetBlendState
        {
            .src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_COLOR,
            .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
            .color_blend_op        = SDL_GPU_BLENDOP_ADD,
            .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
            .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
            .alpha_blend_op        = SDL_GPU_BLENDOP_ADD,
            .enable_blend          = true
        },
        // Add.
        SDL_GPUColorTargetBlendState
        {
            .src_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
            .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
            .color_blend_op        = SDL_GPU_BLENDOP_ADD,
            .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
            .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
            .alpha_blend_op        = SDL_GPU_BLENDOP_ADD,
            .enable_blend          = true
        },
        // Subtract.
        SDL_GPUColorTargetBlendState
        {
            .src_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
            .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
            .color_blend_op        = SDL_GPU_BLENDOP_SUBTRACT,
            .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
            .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
            .alpha_blend_op        = SDL_GPU_BLENDOP_SUBTRACT,
            .enable_blend          = true
        },
        // Wireframe.
        SDL_GPUColorTargetBlendState
        {
            .src_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
            .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ZERO,
            .color_blend_op        = SDL_GPU_BLENDOP_ADD,
            .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
            .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ZERO,
            .alpha_blend_op        = SDL_GPU_BLENDOP_ADD,
            .enable_blend          = true
        }
    };

    const std::vector<PipelineConfig> PIPELINE_CONFIGS =
    {
        // 2D primitive.
        PipelineConfig
        {
            .Stage                    = RenderStage::Primitive2d,
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
        // 2D textured primitive.
        PipelineConfig
        {
            .Stage                  = RenderStage::Primitive2dTextured,
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
