#pragma once

#include "Engine/Renderer/Common/Constants.h"
#include "Engine/Renderer/Common/Objects/Primitive2d.h"
#include "Engine/Renderer/Common/Objects/Primitive3d.h"
#include "Engine/Renderer/Common/Objects/Sprite2d.h"
#include "Engine/Renderer/Common/View.h"

namespace Silent::Renderer
{
    /** @brief Renderer backend types. */
    enum class RendererType
    {
        OpenGl,
        SdlGpu
    };

    class RendererBase
    {
    protected:
        // =======
        // Fields
        // =======

        RendererType _type          = RendererType::SdlGpu;
        SDL_Window*  _window        = nullptr;
        View         _view          = View();
        Color        _clearColor    = Color::Black;
        uint         _drawCallCount = 0;
        bool         _isResized     = false;
        
        std::vector<Primitive3d>           _primitives3d      = {};
        std::vector<Primitive2d>           _primitives2d      = {};
        std::vector<Sprite2d>              _sprites2d         = {};
        std::vector<Primitive3d>           _debugPrimitives3d = {};
        std::vector<std::function<void()>> _debugGuiDrawCalls = {};

    public:
        // =============
        // Constructors
        // =============

        /** @brief Constructs an uninitialized default `RendererBase`. @todo Not needed? */
        RendererBase() = default;

        // ========
        // Getters
        // ========

        /** @brief Gets the renderer backend type.
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

        /** @brief Submits a 2D primitive for drawing.
         *
         * @param prim 2D primitive to draw.
         */
        virtual void Submit2dPrimitive(const Primitive2d& prim) = 0;

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
                                        const Color& color, int depth, AlignMode alignMode, ScaleMode scaleMode, BlendMode blendMode) = 0;

        // ======
        // Debug
        // ======

        /** @brief Submits a function used to construct an ImGui debug GUI for drawing.
         *
         * @param drawFunc Function defining a debug GUI.
         */
        void SubmitDebugGui(std::function<void()> drawFunc);

        /** @brief Submits a 3D line with additive blending for drawing.
         * Used to construct more complex geometry.
         *
         * @param from Start point.
         * @param to End point.
         * @param color Line color.
         * @param page Debug page in which the line will be visible.
         */
        void SubmitDebugLine(const Vector3& from, const Vector3& to, const Color& color, DebugPage page);

        /** @brief Submits a 3D triangle polygon with additive blending for drawing.
         * Used to construct more complex geometry.
         *
         * @param vert0 First vertex.
         * @param vert1 Second vertex.
         * @param vert2 Third vertex.
         * @param color Triangle color.
         * @param page Debug page in which the triangle will be visible.
         */
        void SubmitDebugTriangle(const Vector3& vert0, const Vector3& vert1, const Vector3& vert2, const Color& color, DebugPage page);

        /** @brief Submits a 3D reticle-shaped target with additive blending for drawing.
         *
         * @param center Center position.
         * @param rot Rotation in space.
         * @param radius Uniform radius.
         * @param color Target color.
         * @param page Debug page in which the target will be visible.
         */
        void SubmitDebugTarget(const Vector3& center, const Quaternion& rot, float radius, const Color& color, DebugPage page);

        /** @brief Submits a 3D box with additive blending for drawing.
         *
         * @param obb Oriented bounding box definition.
         * @param color Box color.
         * @param isWireframe If the box should be wireframe or solid.
         * @param page Debug page in which the box will be visible.
         */
        void SubmitDebugBox(const OrientedBoundingBox& obb, const Color& color, bool isWireframe, DebugPage page);

        /** @brief Submits a 3D sphere with additive blending for drawing.
         *
         * @param sphere Sphere definition.
         * @param color Box color.
         * @param isWireframe If the sphere should be wireframe or solid.
         * @param page Debug page in which the sphere will be visible.
         */
        void SubmitDebugSphere(const BoundingSphere& sphere, const Color& color, bool isWireframe, DebugPage page);

        /** @brief Submits a 3D cylinder with additive blending for drawing.
         *
         * @param center Center position.
         * @param rot Rotation in space.
         * @param radius Circle radius.
         * @param length Vertical length.
         * @param color Cylinder color.
         * @param isWireframe If the cylinder should be wireframe or solid.
         * @param page Debug page in which the cylinder will be visible.
         */
        void SubmitDebugCylinder(const Vector3& center, const Quaternion& rot, float radius, float length, const Color& color, bool isWireframe, DebugPage page);

        /** @brief Submits a 3D cone with additive blending for drawing.
         *
         * @param center Center position.
         * @param rot Rotation in space.
         * @param radius Circle radius.
         * @param length Vertical length.
         * @param color Cone color.
         * @param isWireframe If the cone should be wireframe or solid.
         * @param page Debug page in which the cone will be visible.
         */
        void SubmitDebugCone(const Vector3& center, const Quaternion& rot, float radius, float length, const Color& color, bool isWireframe, DebugPage page);

        /** @brief Submits a 3D diamond with additive blending for drawing.
         *
         * @param center Center position.
         * @param rot Rotation in space.
         * @param radius Circle radius.
         * @param length Vertical length.
         * @param color Diamond color.
         * @param isWireframe If the diamond should be wireframe or solid.
         * @param page Debug page in which the diamond will be visible.
         */
        void SubmitDebugDiamond(const Vector3& center, const Quaternion& rot, float radius, float length, const Color& color, bool isWireframe, DebugPage page);

    protected:
        // ========
        // Helpers
        // ========

        /** @brief Prepares renderer data used for the current frame. Called at the start of `Update`. */
        void PrepareFrameData();

        /** @brief Clears renderer data used for the previous frame. Called at the end of `Update`. */
        void ClearFrameData();

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
