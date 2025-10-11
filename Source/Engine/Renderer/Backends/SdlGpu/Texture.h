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
        //===========================
        // Constructors, destructors
        //===========================

        /** @brief Constructs an uninitialized default `Texture`. */
        Texture() = default;

        /** @brief Constructs a `Texture` and uploads it to the GPU.
         * If the relevant TIM asset isn't already loaded, it will be load as a preliminary step.
         *
         * @param device GPU device.
         * @param copyPass Copy pass.
         * @param assetIdx TIM asset index.
         */
        Texture(SDL_GPUDevice& device, SDL_GPUCopyPass& copyPass, int assetIdx);

        /** @brief Gracefully destroys the `Texture`, freeing GPU resources. */
        ~Texture();
    };
}
