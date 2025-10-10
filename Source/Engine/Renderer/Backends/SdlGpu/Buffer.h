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

        /** @brief Constructs an uninitialized default `Buffer`. */
        Buffer() = default;

        /** @brief Constructs a vertex, index, or indirect `Buffer`.
         *
         * @param device GPU device.
         * @param usageFlags Buffer usage flags.
         * @param size Static buffer size.
         */
        Buffer(SDL_GPUDevice& device, SDL_GPUBufferUsageFlags usageFlags, uint size);

        //===========
        // Utilities
        //===========

        /** @brief Uploads data to the GPU buffer.
         *
         * @param data New data to transfer to the buffer.
         * @param startIdx Start index in the buffer at which to transfer the new data.
         */
        void Update(SDL_GPUCopyPass& copyPass, std::span<const T> data, uint startIdx);

        /** @brief Binds the GPU buffer for drawing.
         *
         * @todo How would you bind multiple?
         *
         * @param renderPass 
         * @param startIdx 
         */
        void Bind(SDL_GPURenderPass& renderPass, uint startIdx);
    };

    template <typename T>
    Buffer<T>::Buffer(SDL_GPUDevice& device, SDL_GPUBufferUsageFlags usageFlags, uint size)
    {
        if (!(usageFlags & (SDL_GPU_BUFFERUSAGE_VERTEX | SDL_GPU_BUFFERUSAGE_INDEX | SDL_GPU_BUFFERUSAGE_INDIRECT)))
        {
            throw std::runtime_error("Attempted to create GPU buffer with invalid usage flags.");
        }

        _device = &device;

        auto bufferInfo = SDL_GPUBufferCreateInfo
        {
            .usage = usageFlags,
            .size  = size * sizeof(T)
        };

        // Create buffer.
        _buffer = SDL_CreateGPUBuffer(&device, &bufferInfo);
        if (_buffer == nullptr)
        {
            Log("Failed to create buffer: " + std::string(SDL_GetError()));
        }

        auto transferBufferInfo = SDL_GPUTransferBufferCreateInfo
        {
            .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
            .size  = size * sizeof(T)
        };

        // Create transfer buffer.
        _transfer = SDL_CreateGPUTransferBuffer(_device, &transferBufferInfo);
        if (_transfer == nullptr)
        {
            Log("Failed to create transfer buffer: " + std::string(SDL_GetError()));
        }
    }

    template <typename T>
    void Buffer<T>::Update(SDL_GPUCopyPass& copyPass, std::span<const T> data, uint startIdx)
    {
        auto* transferBuffer = (T*)SDL_MapGPUTransferBuffer(_device, _transfer, false);
        memcpy(transferBuffer, data.data(), data.size_bytes());
        SDL_UnmapGPUTransferBuffer(_device, _transfer);

        auto loc = SDL_GPUTransferBufferLocation
        {
            .transfer_buffer = _transfer
        };
        auto region = SDL_GPUBufferRegion
        {
            .buffer = _buffer,
            .offset = startIdx * sizeof(T),
            .size   = (uint)data.size_bytes()
        };
        SDL_UploadToGPUBuffer(&copyPass, &loc, &region, true);
    }

    template <typename T>
    void Buffer<T>::Bind(SDL_GPURenderPass& renderPass, uint startIdx)
    {
        auto bufferBindings      = std::array<SDL_GPUBufferBinding, 1>{};
        bufferBindings[0].buffer = _buffer;
        bufferBindings[0].offset = startIdx * sizeof(T),

        SDL_BindGPUVertexBuffers(&renderPass, 0, bufferBindings.data(), bufferBindings.size());
    }
};
