#pragma once

namespace Silent::Renderer
{
    /** @brief Graphics pipeline types. */
    enum class PipelineType
    {
        Fill,
        Line,
        Triangle,

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
         * @param renderPass Render pass used in rendering to bind the pipeline to.
         * @param pipelineType Pipeline to bind.
         */
        void Bind(SDL_GPURenderPass& renderPass, PipelineType pipelineType);

    private:
        // ========
        // Helpers
        // ========

        SDL_GPUShader* LoadShader(const std::string& filename, uint samplerCount, uint uniBufferCount, uint storageBufferCount, uint storageTexCount);

        void InitializeFillPipeline(const std::string& vertShaderName, const std::string fragShaderName);
        void InitializeLinePipeline(const std::string& vertShaderName, const std::string fragShaderName);
        // etc. @todo Dedicated init functions for each pipeline to cleanly encapsulate procedures.
    };
}
