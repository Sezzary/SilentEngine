#pragma once

namespace Silent::Renderer
{
    /** @brief GPU texture. */
    class Texture
    {
    private:
        // =======
        // Fields
        // =======

        SDL_GPUDevice*  _device  = nullptr;
        SDL_GPUTexture* _texture = nullptr;

    public:
        // =============
        // Constructors
        // =============

        /** @brief Constructs an uninitialized default `Texture`. */
        Texture() = default;

        /** @brief Initializes the texture and uploads it to the GPU.
         * If the TIM asset isn't already loaded, it will be loaded as a preliminary step.
         *
         * @param device GPU device.
         * @param copyPass Copy pass.
         * @param assetIdx TIM asset index.
         * @exception `std::runtime_error` if the asset is invalid.
         */
        void Initialize(SDL_GPUDevice& device, SDL_GPUCopyPass& copyPass, int assetIdx);

        /** @brief Gracefully destroys the `Texture` and frees GPU resources. */
        ~Texture();

        void Bind(SDL_GPURenderPass& renderPass, SDL_GPUSampler& sampler);
    };
}
