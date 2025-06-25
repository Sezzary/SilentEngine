#pragma once

namespace Silent::Renderer
{
    constexpr auto SCREEN_SPACE_RES = Vector2(100.0f);

    enum class RendererType
    {
        OpenGl,
        SdlGpu,
        Vulkan
    };

    class RendererBase
    {
    protected:
        // Fields

        RendererType _type          = RendererType::OpenGl;
        SDL_Window*  _window        = nullptr;
        bool         _isResized     = false;
        uint         _drawCallCount = 0;

        std::vector<std::function<void()>> _debugGuiDrawCalls = {};
        //std::vector<Primitive2d>           _debugPrimitives2d = {};
        //std::vector<Primitive3d>           _debugPrimitives3d = {};

    public:
        // Constructors

        RendererBase() = default;

        // Getters

        Vector2i     GetScreenResolution() const;
        RendererType GetType() const;

        // Utilities

        void SignalResize();

        virtual void Initialize(SDL_Window& window) = 0;
        virtual void Deinitialize() = 0;
        virtual void Update() = 0;
        virtual void RefreshTextureFilter() = 0;
        virtual void SaveScreenshot() const = 0;
        virtual void LogError(const std::string& msg) const = 0;
        
        // Debug

        void SubmitDebugGui(std::function<void()> drawFunc);
        void SubmitDebugLine(const Vector3& from, const Vector3& to, const Color& color, DebugPage page);
        void SubmitDebugTriangle(const Vector3& vert0, const Vector3& vert1, const Vector3& vert2, const Color& color, DebugPage page);
        void SubmitDebugTarget(const Vector3& center, const Quaternion& rot, float radius, const Color& color, DebugPage page);
        void SubmitDebugBox(const OrientedBoundingBox& obb, const Color& color, bool isWireframe, DebugPage page);
        void SubmitDebugSphere(const BoundingSphere& sphere, const Color& color, bool isWireframe, DebugPage page);
        void SubmitDebugCylinder(const Vector3& center, const Quaternion& rot, float radius, float length, const Color& color, bool isWireframe, DebugPage page);
        void SubmitDebugCone(const Vector3& center, const Quaternion& rot, float radius, float length, const Color& color, bool isWireframe, DebugPage page);
        void SubmitDebugDiamond(const Vector3& center, const Quaternion& rot, float radius, float length, const Color& color, bool isWireframe, DebugPage page);
    };
}
