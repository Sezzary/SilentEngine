#pragma once

#include "Engine/Renderer/Common/Objects/Primitive2d.h"

namespace Silent::Renderer
{
    constexpr auto SCREEN_SPACE_RES         = Vector2(100.0f);
    constexpr auto CLASSIC_SCREEN_SPACE_RES = Vector2(320.0f, 240.0f);
    constexpr uint PRIMITIVE_2D_COUNT_MAX   = 128;

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

        /** @brief Signals a viewport resize. */
        void SignalResize();

        /** @brief Initializes the renderer and its subsystems.
         *
         * @param window Window to claim as the render surface.
         */
        virtual void Initialize(SDL_Window& window) = 0;

        /** @brief Gracefully deinitializes the renderer and its subsystems. */
        virtual void Deinitialize() = 0;

        /** @brief Prepares all GPU data and draws to the render surface. */
        virtual void Update() = 0;

        virtual void RefreshTextureFilter() = 0;

        /** @brief Saves a screenshot of the active render surface to the designated `Screenshots` folder on the system. */
        virtual void SaveScreenshot() const = 0;

        virtual void LogError(const std::string& msg) const = 0;

        /** @brief Submits a 2D primitive shape for drawing.
         *
         * @param prim 2D primitive.
         */
        virtual void SubmitPrimitive2d(const Primitive2d& prim) = 0;

        /** @brief Submits a screen sprite for drawing.
         *
         * @param assedIdx Asset index containing the sprite to draw.
         * @param uvMin Minimum UV position as alpha (top-left).
         * @param uvMax Maximum UV position as alpha (bottom-right).
         * @param pos Screen position in percent.
         * @param rot Sprite rotation.
         * @param scale Sprite scale.
         * @param color Tint color and opacity.
         * @param depth Draw priority. Lower values take precedence.
         * @param alignMode Sprite align mode relative to the screen aspect ratio.
         * @param scaleMode Sprite scale mode relative to the screen aspect ratio.
         * @param blendMode Draw blend mode.
         */
        virtual void SubmitScreenSprite(int assetIdx, const Vector2& uvMin, const Vector2& uvMax, const Vector2& pos, short rot, const Vector2& scale,
                                        const Color& color, int depth, ScreenSpriteAlignMode alignMode, ScreenSpriteScaleMode scaleMode, BlendMode blendMode) = 0;

        // ======
        // Debug
        // ======

        /** @brief Submits a function used to construct an ImGui debug GUI for drawing.
         *
         * @param drawFunc Function defining a debug GUI.
         */
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

        /** @brief Draws a 3D scene. Called before `Draw2dScene`. */
        virtual void Draw3dScene() = 0;

        /** @brief Draws a 2D scene on top of the 3D scene. Called after `Draw3dScene` and before `DrawDebugGui`. */
        virtual void Draw2dScene() = 0;

        /** @brief Draws a debug GUI on top of the 3D and 2D scenes. Called after Draw2dScene. */
        virtual void DrawDebugGui() = 0;
    };

    /** @brief Creates a renderer of a specified backend type.
     *
     * @param type Renderer backend type.
     * @return New renderer.
     */
    std::unique_ptr<RendererBase> CreateRenderer(RendererType type);
}
