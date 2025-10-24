#pragma once

#include "Renderer/Common/Enums.h"

namespace Silent::Renderer
{
    enum class BlendMode;
    struct     PipelineConfig;

    /** @brief Pipeline types. */
    enum class PipelineType
    {
        Primitive2d,
        Primitive2dTextured,

        /** Post-process */

        Vignette,

        Count
    };

    /** @brief Central pipeline manager. */
    class PipelineManager
    {
    private:
        // ==========
        // Constants
        // ==========

        static constexpr uint PIPELINE_COUNT = ((int)PipelineType::Count * (int)BlendMode::Count) * 2;

        // =======
        // Fields
        // =======

        SDL_GPUDevice*                                       _device    = nullptr;
        std::array<SDL_GPUGraphicsPipeline*, PIPELINE_COUNT> _pipelines = {};

    public:
        // =============
        // Constructors
        // =============

        /** @brief Constructs an uninitialized default `PipelineManager`. */
        PipelineManager() = default;

        /** @brief Gracefull destroys the `PipelineManager` and releases GPU resources. */
        ~PipelineManager();

        // ==========
        // Utilities
        // ==========

        /** @brief Initializes all pipelines.
         *
         * @param window App window.
         * @param device GPU device.
         */
        void Initialize(SDL_Window& window, SDL_GPUDevice& device);

        /** @brief Binds the graphics pipeline for use in rendering.
         *
         * @param renderPass Render pass used in rendering to bind the pipeline to.
         * @param pipelineType Pipeline to bind.
         */
        void Bind(SDL_GPURenderPass& renderPass, PipelineType pipelineType, BlendMode blendMode);

    private:
        // ========
        // Helpers
        // ========

        /** @brief Initializes the graphics pipeline with vertex and fragment shaders.
         *
         * @param window App window.
         * @param config Pipeline configuration details.
         */
        void InitializeGraphicsPipeline(SDL_Window& window, const PipelineConfig& config);

        /** @brief Loads a vertex or fragment shader.
         *
         * @param filename Shader filename. Suffix must be `.vert` or `.frag`.
         * @param samplerCount Sampler count.
         * @param uniBufferCount Uniform buffer count.
         * @param storageBufferCount Storage buffer count.
         * @param storageTexCount Storage texture count.
         * @return Compiled vertex or fragment shader.
         */
        SDL_GPUShader* LoadShader(const std::string& filename, uint samplerCount, uint storageTexCount, uint storageBufferCount, uint uniBufferCount);

        /** @brief Computes the pipeline index according to the type, blend mode, and wireframe status.
         *
         * @param pipelineType Pipeline type.
         * @param blendMode Blend mode.
         * @param isWireframe Wireframe status.
         * @return Pipeline index.
         */
        int GetPipelineIdx(PipelineType pipelineType, BlendMode blendMode, bool isWireframe);
    };
}
