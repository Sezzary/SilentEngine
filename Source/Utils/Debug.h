#pragma once

namespace Silent::Utils::Debug
{
#ifdef _DEBUG
    constexpr bool IS_DEBUG_BUILD = true;
#else
    constexpr bool IS_DEBUG_BUILD = false;
#endif

    enum class LogLevel
    {
        Info,
        Warning,
        Error,
        Critical
    };

    enum class LogMode
    {
        Debug,
        Release,
        DebugRelease
    };

    enum class DebugPage
    {
        None,
        Renderer,
        Input,
        Cheats,
        Options
    };

    struct DebugData
    {
        DebugPage Page = DebugPage::Renderer;

        // Temp

        float BlendAlpha = 0.0f;

        // Renderer

        bool EnableWireframeMode = false;

        // Cheats

        bool EnableFreezeMode = false;
    };

    extern DebugData g_DebugData;

    void Scratchpad();

    void InitializeDebug();
    void DeinitializeDebug();
    void UpdateDebug();

    void Message(const std::string& msg, ...);
    void Log(const std::string& msg, LogLevel level = LogLevel::Info, LogMode mode = LogMode::DebugRelease, bool repeat = false);
    void Assert(bool cond, const std::string& msg);
    void StartTimer();
    void EndTimer();

    void CreateGui(std::function<void()> drawFunc);
    void CreateLine(const Vector3& from, const Vector3& to, const Color& color, DebugPage page = DebugPage::None);
    void CreateTriangle(const Vector3& vert0, const Vector3& vert1, const Vector3& vert2, const Color& color, DebugPage page = DebugPage::None);
    void CreateTarget(const Vector3& center, const Quaternion& rot, float radius, const Color& color, DebugPage page = DebugPage::None);
    void CreateBox(const OrientedBoundingBox& obb, const Color& color, bool isWireframe = true, DebugPage page = DebugPage::None);
    void CreateSphere(const BoundingSphere& sphere, const Color& color, bool isWireframe = true, DebugPage page = DebugPage::None);
    void CreateCylinder(const Vector3& center, const Quaternion& rot, float radius, float length, const Color& color, bool isWireframe = true, DebugPage page = DebugPage::None);
    void CreateCone(const Vector3& center, const Quaternion& rot, float radius, float length, const Color& color, bool isWireframe = true, DebugPage page = DebugPage::None);
}
