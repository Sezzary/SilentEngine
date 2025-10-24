#include "Framework.h"
#include "Renderer/Backends/SdlGpu/SdlGpu.h"

#include "Application.h"
#include "Renderer/Backends/SdlGpu/Buffer.h"
#include "Renderer/Backends/SdlGpu/Pipeline.h"
#include "Renderer/Backends/SdlGpu/Texture.h"
#include "Renderer/Common/Utils.h"
#include "Renderer/Common/View.h" // @todo Not used yet.
#include "Renderer/Renderer.h"
#include "Services/Filesystem.h"
#include "Services/Options.h"
#include "Utils/Utils.h"

using namespace Silent::Services;
using namespace Silent::Utils;

namespace Silent::Renderer
{
    struct TestUniform
    {
        bool IsFastAlpha = false;
    };

    static auto UniformBuffer = TestUniform{};

    struct PositionTextureVertex
    {
        float x, y, z;
        float u, v;
    };

    // Texture test.
    static SDL_GPUBuffer* VertexBuffer = nullptr;
    static SDL_GPUBuffer* IndexBuffer  = nullptr;
    static Texture TestTexture = Texture();

    void SdlGpuRenderer::Initialize(SDL_Window& window)
    {
        Debug::Log("Using SDL_gpu renderer.");

        _type   = RendererType::SdlGpu;
        _window = &window;

        // Create GPU device.
        int formatFlags = SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL;
        _device         = SDL_CreateGPUDevice(formatFlags, Debug::IS_DEBUG_BUILD, nullptr);
        if (_device == nullptr)
        {
            throw std::runtime_error("Failed to create GPU device: " + std::string(SDL_GetError()));
        }

        // Claim window.
        if (!SDL_ClaimWindowForGPUDevice(_device, _window))
        {
            throw std::runtime_error("Failed to claim window for GPU device: " + std::string(SDL_GetError()));
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

        // Initialize vertex, index, and indirect buffers.
        _buffers.Primitives2d = Buffer<BufferVertex>(*_device, SDL_GPU_BUFFERUSAGE_VERTEX, (PRIMITIVE_2D_COUNT_MAX * 2) * TRIANGLE_VERTEX_COUNT, "2d primitive triangle vertices");

        // Reserve memory.
        _primitives2d.reserve(PRIMITIVE_2D_COUNT_MAX);

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

        // Texture test.
        // ======================

        // Upload transfer data to GPU resources.
        auto* uploadCmdBuffer = SDL_AcquireGPUCommandBuffer(_device);
        auto* copyPass        = SDL_BeginGPUCopyPass(uploadCmdBuffer);

        TestTexture.Initialize(*_device, *copyPass, 1);

        // Create GPU resources.
        auto vertBufferInfo = SDL_GPUBufferCreateInfo
        {
            .usage = SDL_GPU_BUFFERUSAGE_VERTEX,
            .size  = sizeof(PositionTextureVertex) * 4
        };
        VertexBuffer = SDL_CreateGPUBuffer(_device, &vertBufferInfo);
        SDL_SetGPUBufferName(_device, VertexBuffer, "Derg Vertex Buffer");

        auto idxBufferInfo = SDL_GPUBufferCreateInfo
        {
            .usage = SDL_GPU_BUFFERUSAGE_INDEX,
            .size  = sizeof(uint16) * 6
        };
        IndexBuffer = SDL_CreateGPUBuffer(_device, &idxBufferInfo);

        // Set up buffer data.
        auto transferBufferInfo2 = SDL_GPUTransferBufferCreateInfo
        {
            .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
            .size  = (sizeof(PositionTextureVertex) * 4) + (sizeof(uint16) * 6)
        };
        auto* bufferTransferBuffer = SDL_CreateGPUTransferBuffer(_device, &transferBufferInfo2);

        auto* transferData = (PositionTextureVertex*)SDL_MapGPUTransferBuffer(_device, bufferTransferBuffer, false);
        transferData[0]    = PositionTextureVertex{ -1.0f,  1.0f, 0.0f, 0.0f, 0.0f };
        transferData[1]    = PositionTextureVertex{  1.0f,  1.0f, 0.0f, 1.0f, 0.0f };
        transferData[2]    = PositionTextureVertex{  1.0f, -1.0f, 0.0f, 1.0f, 1.0f };
        transferData[3]    = PositionTextureVertex{ -1.0f, -1.0f, 0.0f, 0.0f, 1.0f };

        uint16* idxData = (uint16*)&transferData[4];
        idxData[0]      = 0;
        idxData[1]      = 1;
        idxData[2]      = 2;
        idxData[3]      = 0;
        idxData[4]      = 2;
        idxData[5]      = 3;

        SDL_UnmapGPUTransferBuffer(_device, bufferTransferBuffer);

        auto transferBufferLoc = SDL_GPUTransferBufferLocation
        {
            .transfer_buffer = bufferTransferBuffer,
            .offset          = 0
        };
        auto bufferRegion = SDL_GPUBufferRegion
        {
            .buffer = VertexBuffer,
            .offset = 0,
            .size   = sizeof(PositionTextureVertex) * 4
        };
        SDL_UploadToGPUBuffer(copyPass, &transferBufferLoc, &bufferRegion, false);

        transferBufferLoc = SDL_GPUTransferBufferLocation
        {
            .transfer_buffer = bufferTransferBuffer,
            .offset          = sizeof(PositionTextureVertex) * 4
        };
        bufferRegion = SDL_GPUBufferRegion
        {
            .buffer = IndexBuffer,
            .offset = 0,
            .size   = sizeof(uint16) * 6
        };
        SDL_UploadToGPUBuffer(copyPass, &transferBufferLoc, &bufferRegion, false);
        SDL_ReleaseGPUTransferBuffer(_device, bufferTransferBuffer);

        SDL_EndGPUCopyPass(copyPass);
        SDL_SubmitGPUCommandBuffer(uploadCmdBuffer);
    }

    // @todo Has errors.
    void SdlGpuRenderer::Deinitialize()
    {
        SDL_WaitForGPUIdle(_device);

        ImGui_ImplSDL3_Shutdown();
        ImGui_ImplSDLGPU3_Shutdown();
        ImGui::DestroyContext();

        SDL_ReleaseWindowFromGPUDevice(_device, _window);
        SDL_DestroyGPUDevice(_device);
    }

    void SdlGpuRenderer::Update()
    {
        // Frame setup.
        PrepareFrameData();

        // Acquire command buffer.
        _commandBuffer = SDL_AcquireGPUCommandBuffer(_device);
        if (_commandBuffer == nullptr)
        {
            Debug::Log("Failed to acquire command buffer: " + std::string(SDL_GetError()), Debug::LogLevel::Error);
            ClearFrameData();
            return;
        }

        // Acquire swapchain texture.
        _swapchainTexture = nullptr;
        if (!SDL_WaitAndAcquireGPUSwapchainTexture(_commandBuffer, _window, &_swapchainTexture, nullptr, nullptr))
        {
            Debug::Log("Failed to acquire swapchain texture: " + std::string(SDL_GetError()), Debug::LogLevel::Warning, Debug::LogMode::Debug);
            ClearFrameData();
            return;
        }

        // Draw frame.
        if (_swapchainTexture != nullptr)
        {
            Draw3dScene();
            Draw2dScene();
            DrawPostProcess();
            DrawDebugGui();
        }

        // Submit command buffer to GPU.
        SDL_SubmitGPUCommandBuffer(_commandBuffer);

        // Clear frame setup.
        ClearFrameData();
    }

    void SdlGpuRenderer::SaveScreenshot() const
    {
        constexpr uint COLOR_CHANNEL_COUNT = 3; // RGB.

        const auto& fs = g_App.GetFilesystem();

        // Get window size.
        auto res = GetScreenResolution();

        // Ensure directory exists.
        auto timestamp = GetCurrentDateString() + "_" + GetCurrentTimeString();
        auto filename  = (SCREENSHOT_FILENAME_BASE + timestamp) + PNG_FILE_EXT;
        auto path      = fs.GetScreenshotsDirectory() / filename;
        std::filesystem::create_directories(path.parent_path());

        // Get window surface.
        auto* surface = SDL_GetWindowSurface(_window);
        if (surface == nullptr)
        {
            Debug::Log("Failed to save screenshot: " + std::string(SDL_GetError()), Debug::LogLevel::Warning, Debug::LogMode::DebugRelease, true);
            return;
        }

        // Lock surface to access pixels.
        if (!SDL_LockSurface(surface))
        {
            Debug::Log("Failed to save screenshot: " + std::string(SDL_GetError()), Debug::LogLevel::Warning, Debug::LogMode::DebugRelease, true);
            return;
        }

        // Write screenshot file.
        if (stbi_write_png(path.string().c_str(), res.x, res.y, COLOR_CHANNEL_COUNT, surface->pixels, res.x * COLOR_CHANNEL_COUNT))
        {
            Debug::Log("Saved screenshot.", Debug::LogLevel::Info, Debug::LogMode::DebugRelease, true);
        }
        else
        {
            Debug::Log("Failed to save screenshot.", Debug::LogLevel::Warning, Debug::LogMode::DebugRelease, true);
        }

        SDL_UnlockSurface(surface);
    }

    void SdlGpuRenderer::Draw3dScene()
    {
        // Begin render pass.
        auto colorTargetInfo = SDL_GPUColorTargetInfo
        {
            .texture     = _swapchainTexture,
            .clear_color = SDL_FColor{ _clearColor.R(), _clearColor.G(), _clearColor.B(), _clearColor.A() },
            .load_op     = SDL_GPU_LOADOP_CLEAR,
            .store_op    = SDL_GPU_STOREOP_STORE
        };
        auto& renderPass = *SDL_BeginGPURenderPass(_commandBuffer, &colorTargetInfo, 1, nullptr);

        // Process render pass.
        SDL_EndGPURenderPass(&renderPass);
    }

    void SdlGpuRenderer::Draw2dScene()
    {
        const auto& options = g_App.GetOptions();

        // Process copy pass.
        auto* copyPass = SDL_BeginGPUCopyPass(_commandBuffer);

        auto bufferVerts = std::vector<BufferVertex>{};
        Copy2dPrimitives(*copyPass, bufferVerts);

        SDL_EndGPUCopyPass(copyPass);

        // Begin render pass.
        auto colorTargetInfo = SDL_GPUColorTargetInfo
        {
            .texture  = _swapchainTexture,
            .load_op  = SDL_GPU_LOADOP_LOAD,
            .store_op = SDL_GPU_STOREOP_STORE
        };
        auto& renderPass = *SDL_BeginGPURenderPass(_commandBuffer, &colorTargetInfo, 1, nullptr);

        // Texture test.
        // ===========================

        _pipelines.Bind(renderPass, PipelineType::Primitive2dTextured, BlendMode::Opaque);

        auto bufferBinding = SDL_GPUBufferBinding{ .buffer = VertexBuffer, .offset = 0 };
        SDL_BindGPUVertexBuffers(&renderPass, 0, &bufferBinding, 1);

        bufferBinding = SDL_GPUBufferBinding{ .buffer = IndexBuffer, .offset = 0 };
        SDL_BindGPUIndexBuffer(&renderPass, &bufferBinding, SDL_GPU_INDEXELEMENTSIZE_16BIT);

        TestTexture.Bind(renderPass, *_samplers[(int)options->TextureFilter]);

        SDL_DrawGPUIndexedPrimitives(&renderPass, 6, 1, 0, 0, 0);

        //===============================

        // Bind.
        _pipelines.Bind(renderPass, PipelineType::Primitive2d, BlendMode::FastAlpha);
        _buffers.Primitives2d.Bind(renderPass, 0);

        // Upload uniform data.
        UniformBuffer.IsFastAlpha = true;
        SDL_PushGPUFragmentUniformData(_commandBuffer, 0, &UniformBuffer, sizeof(UniformBuffer));

        // Process render pass.
        SDL_DrawGPUPrimitives(&renderPass, bufferVerts.size(), sizeof(bufferVerts) / sizeof(BufferVertex), 0, 0);

        SDL_EndGPURenderPass(&renderPass);
    }

    void SdlGpuRenderer::DrawPostProcess()
    {
        // Begin render pass.
        auto colorTargetInfo = SDL_GPUColorTargetInfo
        {
            .texture  = _swapchainTexture,
            .load_op  = SDL_GPU_LOADOP_LOAD,
            .store_op = SDL_GPU_STOREOP_STORE
        };
        auto& renderPass = *SDL_BeginGPURenderPass(_commandBuffer, &colorTargetInfo, 1, nullptr);

        // @todo

        // Process render pass.
        SDL_EndGPURenderPass(&renderPass);
    }

    void SdlGpuRenderer::DrawDebugGui()
    {
        // If debug GUI is disabled, return early.
        const auto& options = g_App.GetOptions();
        if (!options->EnableDebugGui)
        {
            return;
        }

        // Start new frame.
        ImGui_ImplSDLGPU3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // Draw GUIs.
        for (auto& drawCall : _debugGuiDrawCalls)
        {
            drawCall();
        }

        // Prepare render data.
        ImGui::Render();
        auto* drawData = ImGui::GetDrawData();
        ImGui_ImplSDLGPU3_PrepareDrawData(drawData, _commandBuffer);

        // Begin render pass.
        auto colorTargetInfo = SDL_GPUColorTargetInfo
        {
            .texture  = _swapchainTexture,
            .load_op  = SDL_GPU_LOADOP_LOAD,
            .store_op = SDL_GPU_STOREOP_STORE
        };
        auto* renderPass = SDL_BeginGPURenderPass(_commandBuffer, &colorTargetInfo, 1, nullptr);

        // Process render pass.
        ImGui_ImplSDLGPU3_RenderDrawData(drawData, _commandBuffer, renderPass);
        SDL_EndGPURenderPass(renderPass);
    }

    void SdlGpuRenderer::Copy2dPrimitives(SDL_GPUCopyPass& copyPass, std::vector<BufferVertex>& bufferVerts)
    {
        // Create 2D primitive vertex buffer data.
        bufferVerts.reserve((_primitives2d.size() * 2) * TRIANGLE_VERTEX_COUNT);
        for (const auto& prim : _primitives2d)
        {
            // 2D triangle primitive.
            if (prim.Vertices.size() == TRIANGLE_VERTEX_COUNT)
            {
                for (const auto& vert : prim.Vertices)
                {
                    //auto pos = GetAspectCorrectScreenPosition(Vector2(vert.Position.x, vert.Position.y), prim.ScaleM);
                    auto ndc = ConvertScreenPositionToNdc(Vector2(vert.Position.x, vert.Position.y));
                    bufferVerts.push_back(BufferVertex
                    {
                        .Position = Vector3(ndc.x, ndc.y, std::clamp((float)prim.Depth / (float)DEPTH_MAX, 0.0f, 1.0f)),
                        .Col      = vert.Col
                    });
                }
            }
            // 2D line or quad primitive.
            else if (prim.Vertices.size() == QUAD_VERTEX_COUNT)
            {
                for (int i : QUAD_TRIANGLE_IDXS)
                {
                    const auto& vert = prim.Vertices[i];

                    //auto pos = GetAspectCorrectScreenPosition(Vector2(vert.Position.x, vert.Position.y), prim.ScaleM);
                    auto ndc = ConvertScreenPositionToNdc(Vector2(vert.Position.x, vert.Position.y));
                    bufferVerts.push_back(BufferVertex
                    {
                        .Position = Vector3(ndc.x, ndc.y, std::clamp((float)prim.Depth / (float)DEPTH_MAX, 0.0f, 1.0f)),
                        .Col      = vert.Col
                    });
                }
            }
        }

        // Update buffer.
        _buffers.Primitives2d.Update(copyPass, ToSpan(bufferVerts), 0);
    }
}
