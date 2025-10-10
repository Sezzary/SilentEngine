#pragma once

namespace Silent::Renderer
{
    /** @brief Pipeline types. */
    enum class PipelineType
    {
        Triangle,

        Count
    };

    /** @brief Pipeline configuration data. */
    struct PipelineConfig
    {
        PipelineType Type = PipelineType::Triangle;

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
        std::vector<SDL_GPUColorTargetDescription>  ColorTargetDescs  = {};
    };

    /** @brief Central pipeline manager. */
    class PipelineManager
    {
    private:
        // =======
        // Fields
        // =======

        SDL_GPUDevice*                                                 _device    = nullptr; /** GPU device. */
        std::array<SDL_GPUGraphicsPipeline*, (int)PipelineType::Count> _pipelines = {};      /** Available pipelines. */

    public:
        // =============
        // Constructors
        // =============

        /** @brief Constructs an uninitialized default `PipelineManager`. */
        PipelineManager() = default;

        // ==========
        // Utilities
        // ==========

        /** @brief Initializes all pipelines.
         *
         * @param window App window.
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

        /** @brief Initializes a graphics pipeline with vertex and fragment shaders.
         *
         * @param window App window.
         * @param config Pipeline configuration details.
         */
        void InitializeGraphicsPipeline(SDL_Window& window, const PipelineConfig& config);

        /** @brief Loads a shader.
         *
         * @param filename Shader filename. Suffix must be `.vert` or `.frag`.
         * @param samplerCount Sampler count.
         * @param uniBufferCount Uniform buffer count.
         * @param stroageBufferCount StorageBuffer count.
         * @param storageTexCount Storage texture count.
         * @return Compiled GPU shader.
         */
        SDL_GPUShader* LoadShader(const std::string& filename, uint samplerCount, uint uniBufferCount, uint storageBufferCount, uint storageTexCount);
    };
}
