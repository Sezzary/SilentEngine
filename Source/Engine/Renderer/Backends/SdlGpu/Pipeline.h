#pragma once

namespace Silent::Renderer
{
    /** @brief Pipeline types. */
    enum class PipelineType
    {
        Fill,
        Line,
        Triangle,

        Count
    };

    struct PipelineConfig
    {
        PipelineType Type                         = PipelineType::Fill;

        std::string VertexShaderName             = {};
        uint        VertShaderSamplerCount       = 0;
        uint        VertShaderUniBufferCount     = 0;
        uint        VertShaderStorageBufferCount = 0;
        uint        VertShaderStorageTexCount    = 0;

        std::string FragmentShaderName           = {};
        uint        FragShaderSamplerCount       = 0;
        uint        FragShaderUniBufferCount     = 0;
        uint        FragShaderStorageBufferCount = 0;
        uint        FragShaderStorageTexCount    = 0;

        std::vector<SDL_GPUVertexBufferDescription> VertBufferDescs   = {};
        std::vector<SDL_GPUVertexAttribute>         VertBufferAttribs = {};
        SDL_GPUColorTargetDescription               ColorTargetDesc   = {};
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

        void InitializeGraphicsPipeline(SDL_Window& window, const PipelineConfig& config);

        SDL_GPUShader* LoadShader(const std::string& filename, uint samplerCount, uint uniBufferCount, uint storageBufferCount, uint storageTexCount);
    };
}
