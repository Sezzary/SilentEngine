#pragma once

#include "Renderer/Common/Enums.h"

namespace Silent::Renderer
{
    enum class BlendMode;
    struct     PipelineConfig;

    /** @brief Central pipeline manager. */
    class PipelineManager
    {
    private:
        // =======
        // Fields
        // =======

        SDL_GPUDevice*                                    _device    = nullptr;
        std::unordered_map<int, SDL_GPUGraphicsPipeline*> _pipelines = {}; /** Key = pipeline hash, value = pipeline. */

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

        /** @brief Binds the graphics pipeline render stage for use in rendering.
         *
         * @param renderPass Render pass to bind the pipeline to.
         * @param renderStage Pipeline render stage to bind.
         */
        void Bind(SDL_GPURenderPass& renderPass, RenderStage renderStage, BlendMode blendMode);

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

        /** @brief Computes a pipeline hash from a pipeline render stage and blend mode.
         *
         * @param renderStage Pipeline render stage.
         * @param blendMode Blend mode.
         * @return Pipeline hash.
         */
        int GetPipelineHash(RenderStage renderStage, BlendMode blendMode);
    };
}
