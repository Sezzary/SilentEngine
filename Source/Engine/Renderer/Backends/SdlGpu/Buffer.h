#pragma once

namespace Silent::Renderer
{
    /** @brief GPU buffer containing objects of type `T`. */
	template <typename T>
    class Buffer
    {
    private:
        // =======
        // Fields
        // =======

        SDL_GPUDevice*         _device   = nullptr;
        SDL_GPUBuffer*         _buffer   = nullptr;
        SDL_GPUTransferBuffer* _transfer = nullptr;

    public:
        // =============
        // Constructors
        // =============

        template <typename T>
        Buffer(SDL_GPUDevice& device, SDL_GPUBufferUsageFlags usage, std::span<T> objs)
        {
            _device = &device;

            auto bufferInfo = SDL_GPUBufferCreateInfo
            {
                .usage = usage,
                .size  = objs.size_bytes()
            };

            // Create buffer.
            _buffer = SDL_CreateGPUBuffer(&device, &bufferInfo);
            if (_buffer == nullptr)
            {
                Log("Failed to create buffer: " + std::string(SDL_GetError()));
            }

            auto transferInfo = SDL_GPUTransferBufferCreateInfo
            {
                .size  = objs.size_bytes(),
                .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD
            };

            // Create transfer buffer.
            _transfer = SDL_CreateGPUTransferBuffer(_device, &transferInfo);
            if (_transfer == nullptr)
            {
                Log("Failed to create transfer buffer: " + std::string(SDL_GetError()));
            }
        }

        //===========
        // Utilities
        //===========

        /** @brief Uploads object data to the buffer.
         *
         * @param objs Object data to transfer to the buffer.
         * @param startIdx Start index in the buffer at which to transfer the object data.
         */
        template <typename T>
        void Upload(SDL_GPUCopyPass& copyPass, std::span<T> objs, uint startIdx)
        {
            auto* data = (T*)SDL_MapGPUTransferBuffer(_device, _transfer, false);
            memcpy(data, objs.data(), objs.size_bytes());
            SDL_UnmapGPUTransferBuffer(_device, _transfer);

            auto loc = SDL_GPUTransferBufferLocation
            {
                .transfer_buffer = _transfer
            };

            auto region = SDL_GPUBufferRegion
            {
                .buffer = _buffer,
                .size   = objs.size_bytes(),
                .offset = sizeof(T) * startIdx
            };

            SDL_UploadToGPUBuffer(copyPass, &loc, &region, true);

            // @todo Call these in outer scope.
            //SDL_EndGPUCopyPass(copyPass);
            //SDL_SubmitGPUCommandBuffer(cmdBuffer);
        }
    };
};
