#pragma once

#include "Renderer/Backends/SdlGpu/Pipeline.h"

namespace Silent::Renderer
{
    /** @brief Pipeline configuration data. */
    struct PipelineConfig
    {
        PipelineType Type = PipelineType::Primitive2d;

        std::string VertexShaderName             = {};
        uint        VertShaderSamplerCount       = 0;
        uint        VertShaderUniBufferCount     = 0;
        uint        VertShaderStorageBufferCount = 0;
        uint        VertShaderStorageTexCount    = 0;

        std::string FragmentShaderName           = {};
        uint        FragShaderSamplerCount       = 0;
        uint        FragShaderUniBufferCount     = 0;
        uint        FragShaderStorageBufferCount = 0;
        uint        FragShaderStorageTexCount    = 0;

        std::vector<SDL_GPUVertexBufferDescription> VertBufferDescs   = {};
        std::vector<SDL_GPUVertexAttribute>         VertBufferAttribs = {};
        std::vector<SDL_GPUColorTargetDescription>  ColorTargetDescs  = {}; // @todo Maybe not needed except for special cases. Filled automatically.
    };

    extern const std::vector<SDL_GPUColorTargetBlendState> PIPELINE_BLEND_MODE_COLOR_TARGETS;
    extern const std::vector<PipelineConfig>               PIPELINE_CONFIGS;
}
