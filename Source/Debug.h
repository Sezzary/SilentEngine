#pragma once

namespace Silent::Debug
{
#ifdef _DEBUG
    constexpr bool IS_DEBUG_BUILD = true;
#else
    constexpr bool IS_DEBUG_BUILD = false;
#endif

    /** @brief Log severity levels. */
    enum class LogLevel
    {
        Info,    /** Noteworthy information. */
        Warning, /** Soft execution error with a failsafe. */
        Error,   /** Execution error. May lead to further problems. */
        Critical /** Critical execution error. Leads to a crash. */
    };

    /** @brief Log build modes. */
    enum class LogMode
    {
        Debug,   /** Log in debug build only. */
        Release, /** Log in release build only. */
        All      /** Log in debug and release builds. */
    };

    /** @brief Debug GUI pages. */
    enum class Page
    {
        None,
        Renderer,
        Input,
        Cheats,
        Options
    };

    /** @brief Debug workspace. */
    struct DebugWork
    {
        using TimeType = std::chrono::steady_clock::time_point;

        /** Temp */

        float BlendAlpha = 0.0f;

        /** System (internal) */

        std::vector<std::string> Messages  = {};
        TimeType                 StartTime = {};

        /** System (user) */

        Page Page = Page::Renderer;

        /** Renderer (internal) */

        float    Fps        = 0.0f;
        uint     FrameTime  = 0;
        uint     FrameCount = 0;
        TimeType PrevTime   = {};

        /** Renderer (user) */

        bool EnableWireframeMode = false;

        /** Cheats */

        bool EnableFreezeMode = false;
    };

    /** @brief Global debug workspace data. */
    extern DebugWork g_Work;

    /** @brief Executes scratch code. This is a dedicated space which can be used to test anything. */
    void Scratchpad();

    /** @brief Initializes the debug logger, log file for flushing, and GUI. */
    void Initialize();

    /** @brief Gracefully shuts down the logger. */
    void Deinitialize();

    /** @brief Updates the debug GUI. */
    void Update();

    /** @brief Displays a message in the debug GUI.
     *
     * @param msg Message to display.
     */
    void Message(const char* msg, ...);

    /** @brief Logs a message to the terminal and log file.
     *
     * @param msg Message to log.
     * @param level Log severity level.
     * @param logMode Which build modes to log the message.
     * @param repeat Whether to repeat identical messages.
     */
    void Log(const std::string& msg, LogLevel level = LogLevel::Info, LogMode mode = LogMode::All, bool repeat = false);

    /** @brief Asserts a condition with a failure message. Functional only in the debug build.
     *
     * @param cond Condition to assert.
     * @param msg Message to log in case of failure.
     * @exception `std::runtime_error` if the assertion fails.
     */
    void Assert(bool cond, const std::string& msg);

    /** @brief Starts the debug timer used to test code execution time. Placed before the relevant code block. */
    void StartTimer();

    /** @brief Stops the debug timer and prints the execution time in microseconds via a `Message` call. Placed after the relevant code block. */
    void EndTimer();

    /** @brief Creates a debug GUI and submits it to the renderer for drawing.
     *
     * @param drawFunc Function defining the GUI to construct.
     */
    void CreateGui(std::function<void()> drawFunc);

    /** @brief Creates a 3D line with additive blending and submits it to the renderer for drawing.
     * Used to construct more complex geometry.
     *
     * @param from Start point.
     * @param to End point.
     * @param color Line color.
     * @param page Debug page in which the line will be visible.
     */
    void CreateLine(const Vector3& from, const Vector3& to, const Color& color, Page page = Page::None);

    /** @brief Creates a 3D triangle polygon with additive blending and submits it to the renderer for drawing.
     * Used to construct more complex geometry.
     *
     * @param vert0 First vertex.
     * @param vert1 Second vertex.
     * @param vert2 Third vertex.
     * @param color Triangle color.
     * @param page Debug page in which the triangle will be visible.
     */
    void CreateTriangle(const Vector3& vert0, const Vector3& vert1, const Vector3& vert2, const Color& color, Page page = Page::None);

    /** @brief Creates a 3D reticle-shaped target with additive blending and submits it to the renderer for drawing.
     *
     * @param center Center position.
     * @param rot Rotation in space.
     * @param radius Uniform radius.
     * @param color Target color.
     * @param page Debug page in which the target will be visible.
     */
    void CreateTarget(const Vector3& center, const Quaternion& rot, float radius, const Color& color, Page page = Page::None);

    /** @brief Creates a 3D box with additive blending and submits it to the renderer for drawing.
     *
     * @param obb Oriented bounding box definition.
     * @param color Box color.
     * @param isWireframe If the box should be wireframe or solid.
     * @param page Debug page in which the box will be visible.
     */
    void CreateBox(const OrientedBoundingBox& obb, const Color& color, bool isWireframe = true, Page page = Page::None);

    /** @brief Creates a 3D sphere with additive blending and submits it to the renderer for drawing.
     *
     * @param sphere Sphere definition.
     * @param color Box color.
     * @param isWireframe If the sphere should be wireframe or solid.
     * @param page Debug page in which the sphere will be visible.
     */
    void CreateSphere(const BoundingSphere& sphere, const Color& color, bool isWireframe = true, Page page = Page::None);

    /** @brief Creates a 3D cylinder with additive blending and submits it to the renderer for drawing.
     *
     * @param center Center position.
     * @param rot Rotation in space.
     * @param radius Circle radius.
     * @param length Vertical length.
     * @param color Cylinder color.
     * @param isWireframe If the cylinder should be wireframe or solid.
     * @param page Debug page in which the cylinder will be visible.
     */
    void CreateCylinder(const Vector3& center, const Quaternion& rot, float radius, float length, const Color& color, bool isWireframe = true, Page page = Page::None);
    
    /** @brief Creates a 3D cone with additive blending and submits it to the renderer for drawing.
     *
     * @param center Center position.
     * @param rot Rotation in space.
     * @param radius Circle radius.
     * @param length Vertical length.
     * @param color Cone color.
     * @param isWireframe If the cone should be wireframe or solid.
     * @param page Debug page in which the cone will be visible.
     */
    void CreateCone(const Vector3& center, const Quaternion& rot, float radius, float length, const Color& color, bool isWireframe = true, Page page = Page::None);

    /** @brief Creates a 3D diamond with additive blending and submits it to the renderer for drawing.
     *
     * @param center Center position.
     * @param rot Rotation in space.
     * @param radius Circle radius.
     * @param length Vertical length.
     * @param color Diamond color.
     * @param isWireframe If the diamond should be wireframe or solid.
     * @param page Debug page in which the diamond will be visible.
     */
    void CreateDebugDiamond(const Vector3& center, const Quaternion& rot, float radius, float length, const Color& color, bool isWireframe = true, Page page = Page::None);
}
