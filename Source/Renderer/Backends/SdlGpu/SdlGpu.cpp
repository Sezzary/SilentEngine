#include "Framework.h"
#include "Renderer/Backends/SdlGpu/SdlGpu.h"

#include "Application.h"
#include "Renderer/Backends/SdlGpu/Pipeline/Pipeline.h"
#include "Renderer/Backends/SdlGpu/Resources/MeshCache.h"
#include "Renderer/Backends/SdlGpu/Resources/PingPongTexture.h"
#include "Renderer/Backends/SdlGpu/Resources/TextureCache.h"
#include "Renderer/Common/Resources/Layouts/Buffers.h"
#include "Renderer/Common/Resources/Layouts/Uniforms.h"
#include "Renderer/Common/Utils.h"
#include "Renderer/Renderer.h"
#include "Services/Filesystem.h"
#include "Services/Options.h"
#include "Services/Platform.h"
#include "Utils/Utils.h"

using namespace Silent::Assets;
using namespace Silent::Services;
using namespace Silent::Utils;

namespace Silent::Renderer::SdlGpu
{
    void GpuBuffers::Release()
    {
        ViewportVertices.Release();
        ImmediateVertices2d.Release();
        ImmediateVertices3d.Release();
    }

    void Renderer::Initialize(SDL_Window& window)
    {
        static constexpr char NAME[] = "SDL_gpu";

        auto& assets = g_App.GetAssets();

        _type   = RendererType::SdlGpu;
        _window = &window;

        // Define format flags.
        int formatFlags = 0;
        switch (PLATFORM_TYPE)
        {
            case PlatformType::Windows:
            {
                formatFlags = SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL;
                break;
            }
            case PlatformType::MacOs:
            {
                formatFlags = SDL_GPU_SHADERFORMAT_MSL;
                break;
            }
            case PlatformType::Linux:
            {
                formatFlags = SDL_GPU_SHADERFORMAT_SPIRV;
                break;
            }
        }

        // Create GPU device.
        _device = SDL_CreateGPUDevice(formatFlags, Debug::IS_DEBUG_BUILD, nullptr);
        if (_device == nullptr)
        {
            throw std::runtime_error(Fmt("Failed to create GPU device for {} renderer: {}", NAME, SDL_GetError()));
        }
        Debug::Log(Fmt("Using {} renderer with {} backend.", NAME, SDL_GetGPUDeviceDriver(_device)));

        // Claim window.
        if (!SDL_ClaimWindowForGPUDevice(_device, _window))
        {
            throw std::runtime_error(Fmt("Failed to claim window for GPU device: {}", SDL_GetError()));
        }

        // Enable VSync.
        bool supportsMailbox = SDL_WindowSupportsGPUPresentMode(_device, _window, SDL_GPU_PRESENTMODE_MAILBOX);
        SDL_SetGPUSwapchainParameters(_device, _window, SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
                                      supportsMailbox ? SDL_GPU_PRESENTMODE_MAILBOX : SDL_GPU_PRESENTMODE_VSYNC);

        // Initialize buffers.
        InitializeDoubleBuffer();
        InitializeGpuBuffers();

        // Initialize textures.
        _textures = std::make_unique<TextureCache>(*_device);

        // Initialize render targets.
        _renderTargets.resize((int)RenderTargetType::Count);
        for (auto& renderTarget : _renderTargets)
        {
            renderTarget.Initialize(*_device);
        }

        // Initialize pipelines.
        _pipelines.Initialize(*_window, *_device);

        // Create nearest-neighbor sampler.
        auto nearestSamplerInfo = SDL_GPUSamplerCreateInfo
        {
            .min_filter     = SDL_GPU_FILTER_NEAREST,
            .mag_filter     = SDL_GPU_FILTER_NEAREST,
            .mipmap_mode    = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST,
            .address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_REPEAT,
            .address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_REPEAT,
            .address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_REPEAT
        };
        _samplers.push_back(SDL_CreateGPUSampler(_device, &nearestSamplerInfo));

        // Create linear sampler.
        auto linearSamplerInfo = SDL_GPUSamplerCreateInfo
        {
            .min_filter     = SDL_GPU_FILTER_LINEAR,
            .mag_filter     = SDL_GPU_FILTER_LINEAR,
            .mipmap_mode    = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST,
            .address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_REPEAT,
            .address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_REPEAT,
            .address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_REPEAT
        };
        _samplers.push_back(SDL_CreateGPUSampler(_device, &linearSamplerInfo));

        // Create ImGui context.
        ImGui::CreateContext();
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad;
        ImGui_ImplSDL3_InitForSDLGPU(_window);

        // Initialize ImGui backend.
        auto initInfo = ImGui_ImplSDLGPU3_InitInfo
        {
            .Device            = _device,
            .ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat(_device, _window),
            .MSAASamples       = SDL_GPU_SAMPLECOUNT_1
        };
        ImGui_ImplSDLGPU3_Init(&initInfo);

        // Upload transfer data to GPU resources.
        auto* uploadCmdBuffer = SDL_AcquireGPUCommandBuffer(_device);
        auto* copyPass        = SDL_BeginGPUCopyPass(uploadCmdBuffer);

        // @temp
        GetMeshes().Upload(*copyPass, "Psx/CHARA/HERO.ILM");
        //GetMeshes().Upload(*copyPass, "Psx/CHARA/PRSD.ILM");
        //GetTextures().Upload(*copyPass, "Psx/CHARA/PRSD.TIM");
        GetTextures().Upload(*copyPass, "Psx/CHARA/HERO.TIM");
        //GetTextures().Upload(*copyPass, "Psx/MISC/DEMO0000.DAT"); // Temp. load test.
        //GetTextures().Upload(*copyPass, "Psx/ANIM/AMUSE1.DMS"); // Temp. load test.

        //GetMeshes().Upload(*copyPass, "Psx/BG/APU0002.IPD");
        //GetMeshes().Upload(*copyPass, "Psx/ITEM/FOOK.TMD");

        GetTextures().Upload(*copyPass, ToSpan(DEFAULT_TEXTURE_PIXELS), DEFAULT_TEXTURE_RES, "");
        // @todo If atlas textures aren't updated and the texture is missing, for some reason
        // the app hangs instead of crashing like it's supposed to. Why isn't such an error
        // handled as written?
        UpdateResources(*copyPass);

        SDL_EndGPUCopyPass(copyPass);
        SDL_SubmitGPUCommandBuffer(uploadCmdBuffer);
    }

    void Renderer::Deinitialize()
    {
        SDL_WaitForGPUIdle(_device);

        // Release fences.
        SDL_WaitForGPUFences(_device, true, &_renderFence, 1);
        SDL_ReleaseGPUFence(_device, _renderFence);

        // Release ImGui context.
        ImGui_ImplSDL3_Shutdown();
        ImGui_ImplSDLGPU3_Shutdown();
        ImGui::DestroyContext();

        // Release scene resources.
        GetTextures().Release();
        GetMeshes().ReleaseAll();

        // Release samplers.
        for (auto* sampler : _samplers)
        {
            SDL_ReleaseGPUSampler(_device, sampler);
        }
        _samplers.clear();

        // Release render targets.
        for (auto& renderTarget : _renderTargets)
        {
            renderTarget.Release();
        }

        // Release depth target.
        if (_depthTarget != nullptr)
        {
            SDL_ReleaseGPUTexture(_device, _depthTarget);
            _depthTarget = nullptr;
        }

        // Release GPU setup.
        _pipelines.Release();
        _gpuBuffers.Release();

        // Release window and destroy GPU device.
        SDL_ReleaseWindowFromGPUDevice(_device, _window);
        SDL_DestroyGPUDevice(_device);
    }

    void Renderer::PrepareFrameResources()
    {
        // Acquire command buffer.
        _commandBuffer = SDL_AcquireGPUCommandBuffer(_device);
        if (_commandBuffer == nullptr)
        {
            Debug::Log(Fmt("Failed to acquire command buffer: {}", SDL_GetError()), Debug::LogLevel::Error);
            return;
        }

        // Acquire render targets.
        UpdateRenderTargets();

        // Acquire depth target.
        _depthTarget = GetDepthTexture();
        if (_depthTarget == nullptr)
        {
            return;
        }

        // Acquire swapchain target.
        _swapchainTarget = nullptr;
        if (!SDL_WaitAndAcquireGPUSwapchainTexture(_commandBuffer, _window, &_swapchainTarget, nullptr, nullptr))
        {
            return;
        }

        // Process copy pass.
        auto* copyPass = SDL_BeginGPUCopyPass(_commandBuffer);

        UpdateResources(*copyPass);

        SDL_EndGPUCopyPass(copyPass);
    }

    void Renderer::Update()
    {
        // Wait for GPU to finish previous frame.
        if (_renderFence != nullptr)
        {
            SDL_WaitForGPUFences(_device, true, &_renderFence, 1);
            SDL_ReleaseGPUFence(_device, _renderFence);
            _renderFence = nullptr;
        }

        // Frame setup.
        SortRenderBufferData();
        ClearDrawBatches();

        // Submit new frame to draw for GPU.
        if (_swapchainTarget != nullptr)
        {
            DrawFrame();
        }

        // Submit command buffer for GPU to render new frame asynchronously.
        _renderFence = SDL_SubmitGPUCommandBufferAndAcquireFence(_commandBuffer);
    }

    void Renderer::SaveScreenshot() const
    {
        constexpr int COLOR_CHANNEL_COUNT = 3; // RGB.

        // @todo Doesn't work.

        const auto& fs = g_App.GetFilesystem();

        // Get window size.
        auto res = GetViewportResolution();

        // Ensure directory exists.
        auto timestamp = GetCurrentDateString() + "_" + GetCurrentTimeString();
        auto filename  = (SCREENSHOT_FILENAME_BASE + timestamp) + PNG_FILE_EXT;
        auto path      = fs.GetScreenshotsDirectory() / filename;
        stdfs::create_directories(path.parent_path());

        // Get window surface.
        auto* surface = SDL_GetWindowSurface(_window);
        if (surface == nullptr)
        {
            Debug::Log(Fmt("Failed to save screenshot: {}", SDL_GetError()),
                       Debug::LogLevel::Warning, Debug::LogMode::All, true);
            return;
        }

        // Lock surface to access pixels.
        if (!SDL_LockSurface(surface))
        {
            Debug::Log(Fmt("Failed to save screenshot: {}", SDL_GetError()),
                       Debug::LogLevel::Warning, Debug::LogMode::All, true);
            return;
        }

        // Write screenshot file.
        if (stbi_write_png(path.string().c_str(), res.x, res.y, COLOR_CHANNEL_COUNT,
                           surface->pixels, res.x * COLOR_CHANNEL_COUNT))
        {
            Debug::Log("Saved screenshot.", Debug::LogLevel::Info, Debug::LogMode::All, true);
        }
        else
        {
            Debug::Log("Failed to save screenshot.", Debug::LogLevel::Warning, Debug::LogMode::All, true);
        }

        SDL_UnlockSurface(surface);
    }

    TextureCache& Renderer::GetTextures()
    {
        return *(TextureCache*)_textures.get();
    }

    MeshCache& Renderer::GetMeshes()
    {
        return *(MeshCache*)_meshes.get();
    }

    void Renderer::UpdateRenderTargets()
    {
        // Check if all render targets are valid.
        bool allValid = true;
        for (const auto& renderTarget : _renderTargets)
        {
            if (!renderTarget.IsValid())
            {
                allValid = false;
            }
        }

        // Check if update is required.
        if (!_scene.Frame.Front.IsResized && allValid)
        {
            return;
        }

        auto SetRenderTarget = [&](PingPongTexture& renderTarget, const Vector2i& res)
        {
            renderTarget.Release();

            auto viewportRes   = GetViewportResolution();
            auto renderTexInfo = SDL_GPUTextureCreateInfo
            {
                .type                 = SDL_GPU_TEXTURETYPE_2D,
                .format               = SDL_GetGPUSwapchainTextureFormat(_device, _window),
                .usage                = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER,
                .width                = (uint)res.x,
                .height               = (uint)res.y,
                .layer_count_or_depth = 1,
                .num_levels           = 1
            };
            renderTarget.Read()  = SDL_CreateGPUTexture(_device, &renderTexInfo);
            renderTarget.Write() = SDL_CreateGPUTexture(_device, &renderTexInfo);
        };

        // Update all render targets.
        SetRenderTarget(_renderTargets[(int)RenderTargetType::Virtual240p], GetViewportResolution());
        SetRenderTarget(_renderTargets[(int)RenderTargetType::Virtual480p], GetViewportResolution());
        SetRenderTarget(_renderTargets[(int)RenderTargetType::Native],      GetViewportResolution());
    }

    SDL_GPUTexture* Renderer::GetDepthTexture()
    {
        if (_depthTarget != nullptr && !_scene.Frame.Front.IsResized)
        {
            return _depthTarget;
        }

        if (_depthTarget != nullptr)
        {
            SDL_ReleaseGPUTexture(_device, _depthTarget);
        }

        auto viewportRes  = GetViewportResolution();
        auto depthTexInfo = SDL_GPUTextureCreateInfo
        {
            .type                 = SDL_GPU_TEXTURETYPE_2D,
            .format               = SDL_GPU_TEXTUREFORMAT_D32_FLOAT,
            .usage                = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET,
            .width                = (uint)viewportRes.x,
            .height               = (uint)viewportRes.y,
            .layer_count_or_depth = 1,
            .num_levels           = 1
        };
        return SDL_CreateGPUTexture(_device, &depthTexInfo);
    }

    SDL_GPUSampler& Renderer::GetActiveSampler()
    {
        const auto& options = g_App.GetOptions();

        return *_samplers[(int)options->TextureFilter];
    }
}
