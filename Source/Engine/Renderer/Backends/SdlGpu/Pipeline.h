#pragma once

namespace Silent::Renderer
{
    /** @brief Graphics pipeline types. */
    enum class PipelineType
    {
        Fill,
        Line,

        Count
    };

    /** @brief Central graphics pipeline manager. */
    class PipelineManager
    {
    private:
        // =======
        // Fields
        // =======

        SDL_GPUDevice*                                                 _device    = nullptr;
        std::array<SDL_GPUGraphicsPipeline*, (int)PipelineType::Count> _pipelines = {};

    public:
        // =============
        // Constructors
        // =============

        PipelineManager() = default;

        // ==========
        // Utilities
        // ==========

        /** @brief Initializes all pipelines and their shaders.
         *
         * @param device GPU device.
         */
        void Initialize(SDL_Window& window, SDL_GPUDevice& device);

        /** @brief Binds a graphics pipeline for use in rendering.
         *
         * @param pipelineType Pipeline to bind.
         * @param renderPass Render pass used in rendering to bind the pipeline to.
         */
        void Bind(PipelineType pipelineType, SDL_GPURenderPass& renderPass);

    private:
        // ========
        // Helpers
        // ========

        SDL_GPUShader* LoadShader(const std::string& filename, uint samplerCount, uint uniBufferCount, uint storageBufferCount, uint storageTexCount);
    };
}
