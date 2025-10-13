#include "Framework.h"
#include "Engine/Renderer/Backends/SdlGpu/Texture.h"

#include "Engine/Application.h"
#include "Engine/Assets/Assets.h"

using namespace Silent::Assets;

namespace Silent::Renderer
{
    Texture::Texture(SDL_GPUDevice& device, SDL_GPUCopyPass& copyPass, int assetIdx)
    {
        auto& assets = g_App.GetAssets();

        // Get asset.
        const auto asset = assets.GetAsset(assetIdx);
        if (asset == nullptr)
        {
            throw std::runtime_error("Attempted to initialize invalid asset " + std::to_string(assetIdx) + " as texture.");
        }

        // Check if asset is a TIM image.
        if (asset->Type != AssetType::Tim)
        {
            throw std::runtime_error("Attempted to initialize non-image asset " + std::to_string(assetIdx) + " as texture.");
        }

        _device = &device;

        // Wait until loaded. @todo Load wait mechanism needs revision.
        if (asset->State != AssetState::Loaded)
        {
            assets.LoadAsset(assetIdx).wait();
        }

        // Get TIM image asset data.
        auto data = GetAssetData<TimAsset>(asset->Data);

        // Create texture.
        auto texInfo = SDL_GPUTextureCreateInfo
        {
            .type                 = SDL_GPU_TEXTURETYPE_2D,
            .format               = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
            .usage                = SDL_GPU_TEXTUREUSAGE_SAMPLER,
            .width                = (uint)data->Resolution.x,
            .height               = (uint)data->Resolution.y,
            .layer_count_or_depth = 1,
            .num_levels           = 1
        };
        _texture = SDL_CreateGPUTexture(_device, &texInfo);

        // Set texture name.
        SDL_SetGPUTextureName(_device, _texture, asset->Name.c_str());

        // Create transfer buffer.
        auto transferBufferInfo = SDL_GPUTransferBufferCreateInfo
        {
            .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
            .size  = (uint)((data->Resolution.x * data->Resolution.y) * 4)
        };
        auto* texTransferBuffer = SDL_CreateGPUTransferBuffer(_device, &transferBufferInfo);

        byte* mappedTexTransferData = (byte*)SDL_MapGPUTransferBuffer(_device, texTransferBuffer, false);
        memcpy(mappedTexTransferData, data->Pixels.data(), (data->Resolution.x * data->Resolution.y) * 4);
        SDL_UnmapGPUTransferBuffer(_device, texTransferBuffer);

        // Upload texture data.
        auto texTransferInfo = SDL_GPUTextureTransferInfo
        {
            .transfer_buffer = texTransferBuffer,
            .offset          = 0
        };
        auto texRegion = SDL_GPUTextureRegion
        {
            .texture = _texture,
            .w       = (uint)data->Resolution.x,
            .h       = (uint)data->Resolution.y,
            .d       = 1
        };
        SDL_UploadToGPUTexture(&copyPass, &texTransferInfo, &texRegion, false);
	    SDL_ReleaseGPUTransferBuffer(_device, texTransferBuffer);
    }

    Texture::~Texture()
    {
        SDL_ReleaseGPUTexture(_device, _texture);
    }

    void Texture::Bind(SDL_GPURenderPass& renderPass, SDL_GPUSampler& sampler)
    {
        auto texSamplerBinding = SDL_GPUTextureSamplerBinding
        {
            .texture = _texture,
            .sampler = &sampler
        };
        SDL_BindGPUFragmentSamplers(&renderPass, 0, &texSamplerBinding, 1);
    }
}
