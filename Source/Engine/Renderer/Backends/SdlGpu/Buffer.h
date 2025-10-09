#pragma once

namespace Silent::Renderer
{
    /** @brief GPU buffer with data of type `T`. */
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

        Buffer() = default;
        Buffer(SDL_GPUDevice& device, SDL_GPUBufferUsageFlags usageFlags, std::span<const T> objs);

        //===========
        // Utilities
        //===========

        /** @brief Uploads data to the buffer.
         *
         * @todo Rename `objs` to `data`?
         *
         * @param objs Object data to transfer to the buffer.
         * @param startIdx Start index in the buffer at which to transfer the object data.
         */
        void Update(SDL_GPUCopyPass& copyPass, std::span<const T> objs, uint startIdx);

        /** @brief Binds the buffer for drawing.
         *
         * @todo How would you bind multiple?
         *
         * @param renderPass 
         * @param startIdx 
         */
        void Bind(SDL_GPURenderPass& renderPass, uint startIdx);
    };

    template <typename T>
    Buffer<T>::Buffer(SDL_GPUDevice& device, SDL_GPUBufferUsageFlags usageFlags, std::span<const T> objs)
    {
        if (!(usageFlags & (SDL_GPU_BUFFERUSAGE_VERTEX | SDL_GPU_BUFFERUSAGE_INDEX | SDL_GPU_BUFFERUSAGE_INDIRECT)))
        {
            throw std::runtime_error("Attempted to create invalid GPU buffer.");
        }

        _device = &device;

        auto bufferInfo = SDL_GPUBufferCreateInfo
        {
            .usage = usageFlags,
            .size  = (uint)objs.size_bytes()
        };

        // Create buffer.
        _buffer = SDL_CreateGPUBuffer(&device, &bufferInfo);
        if (_buffer == nullptr)
        {
            Log("Failed to create buffer: " + std::string(SDL_GetError()));
        }

        auto transferInfo = SDL_GPUTransferBufferCreateInfo
        {
            .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
            .size  = (uint)objs.size_bytes()
        };

        // Create transfer buffer.
        _transfer = SDL_CreateGPUTransferBuffer(_device, &transferInfo);
        if (_transfer == nullptr)
        {
            Log("Failed to create transfer buffer: " + std::string(SDL_GetError()));
        }
    }

    template <typename T>
    void Buffer<T>::Update(SDL_GPUCopyPass& copyPass, std::span<const T> objs, uint startIdx)
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
            .offset = startIdx * sizeof(T),
            .size   = (uint)objs.size_bytes()
        };

        SDL_UploadToGPUBuffer(&copyPass, &loc, &region, true);
    }

    template <typename T>
    void Buffer<T>::Bind(SDL_GPURenderPass& renderPass, uint startIdx)
    {
        auto bufferBindings      = std::array<SDL_GPUBufferBinding, 1>{};
        bufferBindings[0].buffer = _buffer;
        bufferBindings[0].offset = startIdx * sizeof(T),

        SDL_BindGPUVertexBuffers(&renderPass, 0, bufferBindings.data(), 1);
    }
};
