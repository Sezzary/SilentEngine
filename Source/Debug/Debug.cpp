#include "Framework.h"
#include "Debug/Debug.h"

#include "Application.h"
#include "Debug/Menu/Main.h"
#include "Debug/Scratchpad.h"
#include "Renderer/Renderer.h"
#include "Services/Clock.h"
#include "Services/Filesystem.h"
#include "Services/Options.h"
#include "Utils/Parallel.h"
#include "Utils/Utils.h"

using namespace Silent::Assets;
using namespace Silent::Renderer;
using namespace Silent::Services;
using namespace Silent::Utils;

namespace Silent::Debug
{
    constexpr char LOGGER_NAME[]     = "Logger";
    constexpr int  MESSAGE_COUNT_MAX = 128;

    DebugWork g_Work = {};

    bool CheckPage(Page page)
    {
        const auto& options = g_App.GetOptions();
        return options->EnableDebugMode && (page == g_Work.Page || page == Page::None);
    }

    void Initialize()
    {
        constexpr char LOG_FILENAME[]       = "Log.txt";
        constexpr char LOG_FORMAT_PATTERN[] = "[%Y-%b-%d %T] [%^%l%$] %v";
        constexpr char IMGUI_FILENAME[]     = "imgui.ini";

        const auto& fs = g_App.GetFilesystem();

        // Set file and console log targets.
        auto path        = fs.GetAppDirectory() / LOG_FILENAME;
        auto fileSink    = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path.string(), true); // `true` = create new log at launch.
        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto logger      = std::make_shared<spdlog::logger>(LOGGER_NAME, spdlog::sinks_init_list{ fileSink, consoleSink });

        // Initialize logger.
        spdlog::initialize_logger(logger);
        logger->set_level(spdlog::level::info);
        logger->flush_on(spdlog::level::info);
        logger->set_pattern(LOG_FORMAT_PATTERN);

        // Initialize ImGui.
        ImGui::CreateContext();
        auto imguiPath             = (fs.GetWorkDirectory() / IMGUI_FILENAME).string();
        ImGui::GetIO().IniFilename = CopyString(imguiPath.c_str(), imguiPath.size());

        // Reserve `Messages` size.
        g_Work.Messages.reserve(MESSAGE_COUNT_MAX);
    }

    void Deinitialize()
    {
        spdlog::shutdown();
    }

    void Update()
    {
        Scratchpad();

        // Check if debug menu is enabled.
        if (!g_Work.EnableDebugMenu)
        {
            g_Work.Messages.clear();
            return;
        }

        CreateMenu();
        //ImGui::ShowDemoWindow();

        g_Work.Messages.clear();
    }

    void Msg(const char* msg, ...)
    {
        constexpr int BUFFER_SIZE = 255;

        // Check if debug menu is enabled.
        if (!g_Work.EnableDebugMenu)
        {
            return;
        }

        // Check if `g_Work.Messages` is full.
        if (g_Work.Messages.size() >= MESSAGE_COUNT_MAX)
        {
            Log("Attempted to add message to full container.", LogLevel::Warning, LogMode::Debug);
            return;
        }

        // Initialize string buffer.
        char buffer[BUFFER_SIZE];
        std::memset(buffer, 0, BUFFER_SIZE);

        // Format string.
        va_list args;
        va_start(args, msg);
        vsnprintf(buffer, BUFFER_SIZE, msg, args);
        va_end(args);

        // @lock Restrict `Messages` access.
        static auto mutex = std::mutex();
        {
            auto lock = ParallelLock(mutex);

            // Add message.
            g_Work.Messages.push_back(buffer);
        }
    }

    void Log(const std::string& msg, LogLevel level, LogMode mode, bool repeat)
    {
        // Ignore debug mode logs in release build.
        if constexpr (!IS_DEBUG_BUILD)
        {
            if (mode == LogMode::Debug)
            {
                return;
            }
        }

        // @lock Restrict logger access.
        static auto mutex = std::mutex();
        {
            auto lock = ParallelLock(mutex);

            static auto prevMsg = std::string();
            if (prevMsg == msg && !repeat)
            {
                return;
            }
            prevMsg = msg;

            // Get logger instance.
            auto logger = spdlog::get(LOGGER_NAME);
            if (logger == nullptr)
            {
                return;
            }

            // Log message.
            switch (level)
            {
                default:
                case LogLevel::Info:
                {
                    logger->info(msg);
                    break;
                }
                case LogLevel::Warning:
                {
                    logger->warn(msg);
                    break;
                }
                case LogLevel::Error:
                {
                    logger->error(msg);
                    break;
                }
                case LogLevel::Critical:
                {
                    logger->critical(msg);
                    break;
                }
            }

            logger->flush();
        }
    }

    void Assert(bool cond, const std::string& msg)
    {
        if constexpr (IS_DEBUG_BUILD)
        {
            if (!cond)
            {
                throw std::runtime_error("Assertion failed. " + msg);
            }
        }
    };

    void StartTimer()
    {
        if constexpr (IS_DEBUG_BUILD)
        {
            g_Work.StartTime = SDL_GetPerformanceCounter();
        }
    }

    void EndTimer()
    {
        if constexpr (IS_DEBUG_BUILD)
        {
            uint64 endTime  = SDL_GetPerformanceCounter();
            uint64 duration = endTime - g_Work.StartTime;
            Msg("Execution (μs): %d", duration);
        }
    }

    void CreateGui(std::function<void()> drawFunc)
    {
        auto& renderer = g_App.GetRenderer();
        renderer.SubmitDebugGui(drawFunc);
    }

    void CreateLine(const Vector2& from, const Vector2& to, const Color& color, ScaleMode scaleMode, Page page)
    {
        auto& renderer = g_App.GetRenderer();
        renderer.SubmitDebugLine(from, to, color, scaleMode, page);
    }

    void CreateLine(const Vector3& from, const Vector3& to, const Color& color, Page page)
    {
        auto& renderer = g_App.GetRenderer();
        renderer.SubmitDebugLine(from, to, color, page);
    }

    void SubmitDebugTriangle(const Vector2& vert0, const Vector2& vert1, const Vector2& vert2, const Color& color, ScaleMode scaleMode, Page page)
    {
        auto& renderer = g_App.GetRenderer();
        renderer.SubmitDebugTriangle(vert0, vert1, vert2, color, scaleMode, page);
    }

    void CreateTriangle(const Vector3& vert0, const Vector3& vert1, const Vector3& vert2, const Color& color, Page page)
    {
        auto& renderer = g_App.GetRenderer();
        renderer.SubmitDebugTriangle(vert0, vert1, vert2, color, page);
    }

    void CreateTarget(const Vector3& center, const Quaternion& rot, float radius, const Color& color, Page page)
    {
        if (!CheckPage(page))
        {
            return;
        }

        auto& renderer = g_App.GetRenderer();

        auto rotMatrix = rot.ToRotationMatrix();

        auto from0 = center + Vector3::Transform(Vector3(radius,  0.0f, 0.0f), rotMatrix);
        auto to0   = center + Vector3::Transform(Vector3(-radius, 0.0f, 0.0f), rotMatrix);
        renderer.SubmitDebugLine(from0, to0, color, page);

        auto from1 = center + Vector3::Transform(Vector3(0.0f,  radius, 0.0f), rotMatrix);
        auto to1   = center + Vector3::Transform(Vector3(0.0f, -radius, 0.0f), rotMatrix);
        renderer.SubmitDebugLine(from1, to1, color, page);

        auto from2 = center + Vector3::Transform(Vector3(0.0f, 0.0f,  radius), rotMatrix);
        auto to2   = center + Vector3::Transform(Vector3(0.0f, 0.0f, -radius), rotMatrix);
        renderer.SubmitDebugLine(from2, to2, color, page);
    }

    void CreateBox(const OrientedBoundingBox& obb, const Color& color, bool isWireframe, Page page)
    {
        if (!CheckPage(page))
        {
            return;
        }

        auto& renderer = g_App.GetRenderer();

        auto corners = obb.GetCorners();

        // Wireframe.
        if (isWireframe)
        {
            renderer.SubmitDebugLine(corners[0], corners[1], color, page);
            renderer.SubmitDebugLine(corners[1], corners[2], color, page);
            renderer.SubmitDebugLine(corners[2], corners[3], color, page);
            renderer.SubmitDebugLine(corners[3], corners[0], color, page);

            renderer.SubmitDebugLine(corners[4], corners[5], color, page);
            renderer.SubmitDebugLine(corners[5], corners[6], color, page);
            renderer.SubmitDebugLine(corners[6], corners[7], color, page);
            renderer.SubmitDebugLine(corners[7], corners[4], color, page);

            renderer.SubmitDebugLine(corners[0], corners[4], color, page);
            renderer.SubmitDebugLine(corners[1], corners[5], color, page);
            renderer.SubmitDebugLine(corners[2], corners[6], color, page);
            renderer.SubmitDebugLine(corners[3], corners[7], color, page);
        }
        // Solid.
        else
        {
            renderer.SubmitDebugTriangle(corners[0], corners[1], corners[2], color, page);
            renderer.SubmitDebugTriangle(corners[0], corners[2], corners[3], color, page);

            renderer.SubmitDebugTriangle(corners[4], corners[5], corners[6], color, page);
            renderer.SubmitDebugTriangle(corners[4], corners[6], corners[7], color, page);

            renderer.SubmitDebugTriangle(corners[0], corners[1], corners[4], color, page);
            renderer.SubmitDebugTriangle(corners[1], corners[4], corners[5], color, page);

            renderer.SubmitDebugTriangle(corners[1], corners[2], corners[5], color, page);
            renderer.SubmitDebugTriangle(corners[2], corners[5], corners[6], color, page);

            renderer.SubmitDebugTriangle(corners[2], corners[3], corners[6], color, page);
            renderer.SubmitDebugTriangle(corners[3], corners[6], corners[7], color, page);

            renderer.SubmitDebugTriangle(corners[0], corners[3], corners[4], color, page);
            renderer.SubmitDebugTriangle(corners[3], corners[4], corners[7], color, page);
        }
    }

    void CreateSphere(const BoundingSphere& sphere, const Color& color, bool isWireframe, Page page)
    {
        constexpr int WIREFRAME_SEGMENT_COUNT = 24;

        if (!CheckPage(page))
        {
            return;
        }

        auto& renderer = g_App.GetRenderer();

        // Wireframe.
        if (isWireframe)
        {
            // Draw circles in XY, YZ, and XZ planes.
            for (int plane = 0; plane < Vector3::AXIS_COUNT; plane++)
            {
                for (int i = 0; i < WIREFRAME_SEGMENT_COUNT; i++)
                {
                    float theta0 = (((float)i       / WIREFRAME_SEGMENT_COUNT) * 2.0f) * PI;
                    float theta1 = (((float)(i + 1) / WIREFRAME_SEGMENT_COUNT) * 2.0f) * PI;

                    auto point0 = Vector3::Zero;
                    auto point1 = Vector3::Zero;
                    switch (plane)
                    {
                        // XY plane.
                        case 0:
                        {
                            point0 = sphere.Center + Vector3(sphere.Radius * glm::cos(theta0), sphere.Radius * glm::sin(theta0), 0.0f);
                            point1 = sphere.Center + Vector3(sphere.Radius * glm::cos(theta1), sphere.Radius * glm::sin(theta1), 0.0f);
                            break;
                        }
                        // YZ plane.
                        case 1:
                        {
                            point0 = sphere.Center + Vector3(0.0f, sphere.Radius * glm::cos(theta0), sphere.Radius * glm::sin(theta0));
                            point1 = sphere.Center + Vector3(0.0f, sphere.Radius * glm::cos(theta1), sphere.Radius * glm::sin(theta1));
                            break;
                        }
                        // ZX plane.
                        case 2:
                        {
                            point0 = sphere.Center + Vector3(sphere.Radius * glm::cos(theta0), 0.0f, sphere.Radius * glm::sin(theta0));
                            point1 = sphere.Center + Vector3(sphere.Radius * glm::cos(theta1), 0.0f, sphere.Radius * glm::sin(theta1));
                            break;
                        }
                    }
                    renderer.SubmitDebugLine(point0, point1, color, page);
                }
            }
        }
        // Solid.
        else
        {
            // @todo
        }
    }

    void CreateCylinder(const Vector3& center, const Quaternion& rot, float radius, float length, const Color& color, bool isWireframe, Page page)
    {
        if (!CheckPage(page))
        {
            return;
        }

        auto& renderer = g_App.GetRenderer();

        // Wireframe.
        if (isWireframe)
        {
            // @todo
        }
        // Solid.
        else
        {
            // @todo
        }
    }

    void CreateCone(const Vector3& center, const Quaternion& rot, float radius, float length, const Color& color, bool isWireframe, Page page)
    {
        if (!CheckPage(page))
        {
            return;
        }

        auto& renderer = g_App.GetRenderer();

        // Wireframe.
        if (isWireframe)
        {
            // @todo
        }
        // Solid.
        else
        {
            // @todo
        }
    }

    void CreateDebugDiamond(const Vector3& center, const Quaternion& rot, float radius, float length, const Color& color, bool isWireframe, Page page)
    {
        if (!CheckPage(page))
        {
            return;
        }

        auto& renderer = g_App.GetRenderer();

        // Wireframe.
        if (isWireframe)
        {
            // @todo
        }
        // Solid.
        else
        {
            // @todo
        }
    }
}
