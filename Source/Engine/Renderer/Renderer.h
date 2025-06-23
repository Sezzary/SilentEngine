#pragma once

#include "Engine/Renderer/Base.h"

namespace Silent::Renderer
{
    enum class RendererType
    {
        OpenGl,
        SdlGpu,
        Vulkan
    };

    std::unique_ptr<RendererBase> CreateRenderer(RendererType type);
}
