#include "Framework.h"
#include "Engine/Renderer/Renderer.h"

#include "Engine/Renderer/Backends/SdlGpu/SdlGpu.h"
#include "Engine/Renderer/Backends/OpenGl/OpenGl.h"
#include "Engine/Renderer/Backends/Vulkan/Vulkan.h"
#include "Engine/Renderer/Base.h"

namespace Silent::Renderer
{
    std::unique_ptr<RendererBase> CreateRenderer(RendererType type)
    {
        switch (type)
        {
            case RendererType::OpenGl:
            {
                return std::make_unique<OpenGlRenderer>();
            }

            /*case RendererType::SdlGpu:
            {
                return std::make_unique<SdlGpuRenderer>();
            }*/

            case RendererType::Vulkan:
            {
                return std::make_unique<VulkanRenderer>();
            }

            default:
            {
                break;
            }
        }

        return nullptr;
    }
}
