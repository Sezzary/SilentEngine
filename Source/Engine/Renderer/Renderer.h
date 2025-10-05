#pragma once

#include "Engine/Renderer/Common/Objects/Primitive2d.h"

namespace Silent::Renderer
{
    constexpr auto SCREEN_SPACE_RES = Vector2(100.0f);

    enum class RendererType
    {
        OpenGl,
        SdlGpu
    };

    enum class BlendMode
    {
        Opaque,
        Alpha,
        Add,
        Subtract,
        Multiply,
        LumaMultiply
    };

    enum class ScreenSpriteAlignMode
    {
        Center,
        CenterTop,
        CenterBottom,
        CenterLeft,
        CenterRight,
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };

    enum class ScreenSpriteScaleMode
    {
        Fit,
        Fill,
        Stretch
    };

    class RendererBase
    {
    protected:
        // =======
        // Fields
        // =======

        RendererType _type          = RendererType::SdlGpu;
        SDL_Window*  _window        = nullptr;
        bool         _isResized     = false;
        uint         _drawCallCount = 0;

        Color _clearColor = Color::Black;

        std::vector<std::function<void()>> _debugGuiDrawCalls = {};

        std::vector<Primitive2d> _primitives2d = {};
        //std::vector<Primitive3d>           _debugPrimitives3d = {};

    public:
        // =============
        // Constructors
        // =============

        RendererBase() = default;

        // ========
        // Getters
        // ========

        /** @brief Gets the renderer backend type;
         *
         * @return Renderer backend type.
         */
        RendererType GetType() const;

        /** @brief Gets the screen render resolution in pixels.
         *
         * @return Render resolution.
         */
        Vector2i GetScreenResolution() const;

        /** @brief Gets the draw call count for the current render tick.
         *
         * @return Draw call count.
         */
        uint GetDrawCallCount() const;

        // ========
        // Setters
        // ========

        /** @brief Sets the color used to clear the screen.
         *
         * @param color New clear color.
         */
        void SetClearColor(const Color& color);

        // ==========
        // Utilities
        // ==========

        void SignalResize();

        virtual void Initialize(SDL_Window& window) = 0;
        virtual void Deinitialize() = 0;
        virtual void Update() = 0;
        virtual void RefreshTextureFilter() = 0;
        virtual void SaveScreenshot() const = 0;
        virtual void LogError(const std::string& msg) const = 0;

        virtual void SubmitPrimitive2d(const Primitive2d& prim) = 0;
        virtual void SubmitScreenSprite(int assetIdx, const Vector2& pos, short rot, const Vector2& scale, const Color& color,
                                        int depth, ScreenSpriteAlignMode alignMode, ScreenSpriteScaleMode scaleMode, BlendMode blendMode) = 0;

        // ======
        // Debug
        // ======

        void SubmitDebugGui(std::function<void()> drawFunc);
        void SubmitDebugLine(const Vector3& from, const Vector3& to, const Color& color, DebugPage page);
        void SubmitDebugTriangle(const Vector3& vert0, const Vector3& vert1, const Vector3& vert2, const Color& color, DebugPage page);
        void SubmitDebugTarget(const Vector3& center, const Quaternion& rot, float radius, const Color& color, DebugPage page);
        void SubmitDebugBox(const OrientedBoundingBox& obb, const Color& color, bool isWireframe, DebugPage page);
        void SubmitDebugSphere(const BoundingSphere& sphere, const Color& color, bool isWireframe, DebugPage page);
        void SubmitDebugCylinder(const Vector3& center, const Quaternion& rot, float radius, float length, const Color& color, bool isWireframe, DebugPage page);
        void SubmitDebugCone(const Vector3& center, const Quaternion& rot, float radius, float length, const Color& color, bool isWireframe, DebugPage page);
        void SubmitDebugDiamond(const Vector3& center, const Quaternion& rot, float radius, float length, const Color& color, bool isWireframe, DebugPage page);

    private:
        // ========
        // Helpers
        // ========

        virtual void DrawDebugGui() = 0;
        virtual void CreateDebugGui() = 0;
    };

    /** @brief Creates a renderer of a specified backend type.
     *
     * @param type Renderer backend type.
     * @return New renderer.
     */
    std::unique_ptr<RendererBase> CreateRenderer(RendererType type);
}
