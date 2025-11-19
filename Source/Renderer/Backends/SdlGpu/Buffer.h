#pragma once

namespace Silent::Renderer
{
    /** @brief GPU vertex, index, or indirect buffer with data of type `T`. */
    template <typename T>
    class Buffer
    {
    private:
        // =======
        // Fields
        // =======

        SDL_GPUDevice*          _device     = nullptr;
        SDL_GPUBuffer*          _buffer     = nullptr;
        SDL_GPUTransferBuffer*  _transfer   = nullptr;
        SDL_GPUBufferUsageFlags _usageFlags = 0;

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
         * @param name Buffer name.
         */
        Buffer(SDL_GPUDevice& device, SDL_GPUBufferUsageFlags usageFlags, uint size, const std::string& name);

        // ==========
        // Utilities
        // ==========

        /** @brief Uploads data to the GPU buffer.
         *
         * @param data New data to transfer to the buffer.
         * @param startIdx Start index in the buffer at which to transfer the new data.
         */
        void Update(SDL_GPUCopyPass& copyPass, std::span<const T> data, uint startIdx);

        /** @brief Binds the GPU buffer for drawing.
         *
         * @param renderPass Render pass.
         * @param startIdx Data start index.
         */
        void Bind(SDL_GPURenderPass& renderPass, uint startIdx);
    };

    template <typename T>
    Buffer<T>::Buffer(SDL_GPUDevice& device, SDL_GPUBufferUsageFlags usageFlags, uint size, const std::string& name)
    {
        _usageFlags = usageFlags;
        if (!(_usageFlags & (SDL_GPU_BUFFERUSAGE_VERTEX | SDL_GPU_BUFFERUSAGE_INDEX | SDL_GPU_BUFFERUSAGE_INDIRECT)))
        {
            throw std::runtime_error("Attempted to create GPU buffer with invalid usage flags.");
        }

        _device = &device;

        auto bufferInfo = SDL_GPUBufferCreateInfo
        {
            .usage = _usageFlags,
            .size  = size * sizeof(T)
        };

        // Create buffer.
        _buffer = SDL_CreateGPUBuffer(&device, &bufferInfo);
        if (_buffer == nullptr)
        {
            Debug::Log(Fmt("Failed to create buffer: {}", SDL_GetError()), Debug::LogLevel::Error);
        }

        // Set buffer name.
        SDL_SetGPUBufferName(_device, _buffer, name.c_str());

        auto transferBufferInfo = SDL_GPUTransferBufferCreateInfo
        {
            .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
            .size  = size * sizeof(T)
        };

        // Create transfer buffer.
        _transfer = SDL_CreateGPUTransferBuffer(_device, &transferBufferInfo);
        if (_transfer == nullptr)
        {
            Debug::Log(Fmt("Failed to create transfer buffer: {}", SDL_GetError()), Debug::LogLevel::Error);
        }
    }

    template <typename T>
    void Buffer<T>::Update(SDL_GPUCopyPass& copyPass, std::span<const T> data, uint startIdx)
    {
        // Map transfer data.
        auto* mappedTransferData = (T*)SDL_MapGPUTransferBuffer(_device, _transfer, false);
        memcpy(mappedTransferData, data.data(), data.size_bytes());
        SDL_UnmapGPUTransferBuffer(_device, _transfer);

        // Upload data.
        auto transferBufferLoc = SDL_GPUTransferBufferLocation
        {
            .transfer_buffer = _transfer
        };
        auto bufferRegion = SDL_GPUBufferRegion
        {
            .buffer = _buffer,
            .offset = startIdx * sizeof(T),
            .size   = (uint)data.size_bytes()
        };
        SDL_UploadToGPUBuffer(&copyPass, &transferBufferLoc, &bufferRegion, true);
    }

    template <typename T>
    void Buffer<T>::Bind(SDL_GPURenderPass& renderPass, uint startIdx)
    {
        auto bufferBindings = SDL_GPUBufferBinding
        {
            .buffer = _buffer,
            .offset = startIdx * sizeof(T)
        };

        if (_usageFlags & SDL_GPU_BUFFERUSAGE_VERTEX)
        {
            SDL_BindGPUVertexBuffers(&renderPass, 0, &bufferBindings, 1);
        }
        else if (_usageFlags & SDL_GPU_BUFFERUSAGE_INDEX)
        {
            SDL_BindGPUIndexBuffer(&renderPass, &bufferBindings, SDL_GPU_INDEXELEMENTSIZE_16BIT);
        }

        // @todo Can indirect buffer be bound?
    }
};
