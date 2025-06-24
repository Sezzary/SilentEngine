#pragma once

#include "Engine/Renderer/Base.h"

namespace Silent::Renderer
{
    std::unique_ptr<RendererBase> CreateRenderer(RendererType type);
}
