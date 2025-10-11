#include "Framework.h"
#include "Engine/Renderer/Backends/OpenGl/OpenGl.h"

#include "Engine/Application.h"
#include "Engine/Assets/Assets.h"
#include "Engine/Game/main.h"
#include "Engine/Renderer/Backends/OpenGl/ElementBuffer.h"
#include "Engine/Renderer/Backends/OpenGl/ShaderProgram.h"
#include "Engine/Renderer/Backends/OpenGl/Texture.h"
#include "Engine/Renderer/Backends/OpenGl/VertexArray.h"
#include "Engine/Renderer/Backends/OpenGl/VertexBuffer.h"
#include "Engine/Renderer/Common/View.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Services/Filesystem.h"
#include "Engine/Services/Options.h"
#include "Engine/Services/Time.h"
#include "Utils/Utils.h"

using namespace Silent::Assets;
using namespace Silent::Services;
using namespace Silent::Utils;

namespace Silent::Renderer
{
    void OpenGlRenderer::Initialize(SDL_Window& window)
    {
        Log("Using OpenGL renderer:");

        _type   = RendererType::OpenGl;
        _window = &window;

        // Create OpenGL context.
        _context = SDL_GL_CreateContext(_window);
        if (!_context)
        {
            throw std::runtime_error("Failed to create SDL OpenGL context: " + std::string(SDL_GetError()));
        }

        // Load OpenGL functions using GLAD.
        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
        {
            throw std::runtime_error("Failed to load OpenGL functions.");
        }

        // Enable vsync.
        SDL_GL_SetSwapInterval(1);

        // Set up viewport.
        auto res = GetScreenResolution();
        glViewport(0, 0, res.x, res.y);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Reserve memory.
        _primitives2d.reserve(PRIMITIVE_2D_COUNT_MAX);

        CreateShaderProgram();

        ImGui_ImplSDL3_InitForOpenGL(_window, _context);
        ImGui_ImplOpenGL3_Init(OPEN_GL_VERSION);

        auto gpu    = std::string((const char*)glGetString(GL_RENDERER));
        auto vendor = std::string((const char*)glGetString(GL_VENDOR));
        Log("    GPU: " + gpu + ", " + vendor);

        auto version = std::string((const char*)glGetString(GL_VERSION));
        Log("    Version: " + version);

        auto shadingLangVersion = std::string((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
        Log("    Shading language version: " + shadingLangVersion);

        int attribCountMax = 0;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attribCountMax);
        Log("    " + std::to_string(attribCountMax) + " vertex attributes available.", LogLevel::Info, LogMode::Debug);

        int varyingVarCountMax = 0;
        glGetIntegerv(GL_MAX_VARYING_VECTORS, &varyingVarCountMax);
        Log("    " + std::to_string(varyingVarCountMax) + " varying variables available.", LogLevel::Info, LogMode::Debug);

        int combinedTexUnitCountMax = 0;
        glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &combinedTexUnitCountMax);
        Log("    " + std::to_string(combinedTexUnitCountMax) + " combined texture image units available.", LogLevel::Info, LogMode::Debug);

        int vertTexImageUnitCountMax = 0;
        glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &vertTexImageUnitCountMax);
        Log("    " + std::to_string(vertTexImageUnitCountMax) + " vertex texture image units available.", LogLevel::Info, LogMode::Debug);

        int texSizeMax = 0;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSizeMax);
        Log("    " + std::to_string(texSizeMax)  + " max texture size.", LogLevel::Info, LogMode::Debug);

        int uniBlockSizeMax = 0;
        glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &uniBlockSizeMax);
        Log("    " + std::to_string(uniBlockSizeMax) + " max uniform block size.", LogLevel::Info, LogMode::Debug);

        int renderBufferSizeMax = 0;
        glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &renderBufferSizeMax);
        Log("    " + std::to_string(renderBufferSizeMax) + " max renderbuffer size.", LogLevel::Info, LogMode::Debug);
    }

    void OpenGlRenderer::Deinitialize()
    {
        for (auto& [keyName, shaderProg] : _shaderPrograms)
        {
            shaderProg.Delete();
        }

        // Delete objects.
        _vertexArray.Delete();
        _vertexPositionBuffer.Delete();
        _vertexColorBuffer.Delete();
        _vertexTexCoordBuffer.Delete();
        _elementBuffer.Delete();
    }

    void OpenGlRenderer::Update()
    {
        // Reset.
        _drawCallCount = 0;
        UpdateViewport();

        // Draw frame.
        Draw3dScene();
        Draw2dScene();
        DrawDebugGui();

        // Swap buffers.
        if (!SDL_GL_SwapWindow(_window))
        {
            Log("Failed to swap render buffer: " + std::string(SDL_GetError()), LogLevel::Warning);
        }

        // Clear scene. @todo

        // Clear primitives
        _primitives2d.clear();
    }

    void OpenGlRenderer::RefreshTextureFilter()
    {
        //_texture0.RefreshFilter();
        //_texture1.RefreshFilter();
    }

    void OpenGlRenderer::SaveScreenshot() const
    {
        constexpr uint COLOR_CHANNEL_COUNT = 3; // RGB.

        const auto& fs = g_App.GetFilesystem();

        // Get window size.
        auto res = GetScreenResolution();

        // Ensure directory exists.
        auto timestamp = GetCurrentDateString() + "_" + GetCurrentTimeString();
        auto filename  = (SCREENSHOT_FILENAME_BASE + timestamp) + PNG_FILE_EXT;
        auto path      = fs.GetScreenshotsDirectory() / filename;
        std::filesystem::create_directories(path.parent_path());

        // Capture screenshot.
        auto pixels = std::vector<uint8>((res.x * res.y) * COLOR_CHANNEL_COUNT);
        glReadPixels(0, 0, res.x, res.y, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());

        // Flip pixels vertically.
        int rowSize = res.x * COLOR_CHANNEL_COUNT;
        for (int y = 0; y < (res.y / 2); y++)
        {
            int oppositeY = (res.y - y) - 1;
            for (int x = 0; x < rowSize; x++)
            {
                std::swap(pixels[(y * rowSize) + x], pixels[(oppositeY * rowSize) + x]);
            }
        }

        // Write screenshot file.
        if (stbi_write_png(path.string().c_str(), res.x, res.y, COLOR_CHANNEL_COUNT, pixels.data(), res.x * COLOR_CHANNEL_COUNT))
        {
            Log("Saved screenshot.", LogLevel::Info, LogMode::DebugRelease, true);
            return;
        }

        Log("Failed to save screenshot.", LogLevel::Warning, LogMode::DebugRelease, true);
    }

    void OpenGlRenderer::LogError(const std::string& msg) const
    {
        if constexpr (IS_DEBUG_BUILD)
        {
            uint errorCode = glGetError();
            while (errorCode != GL_NO_ERROR)
            {
                auto error = std::string();
                switch (errorCode)
                {
                    default:
                    case GL_NO_ERROR:
                    {
                        return;
                    }
                    case GL_INVALID_ENUM:
                    {    
                        error = "INVALID ENUM";
                        break;
                    }
                    case GL_INVALID_VALUE:
                    {    
                        error = "INVALID VALUE";
                        break;
                    }
                    case GL_INVALID_OPERATION:
                    {    
                        error = "INVALID OPERATION";
                        break;
                    }
                    case GL_STACK_OVERFLOW:
                    {    
                        error = "STACK OVERFLOW";
                        break;
                    }
                    case GL_STACK_UNDERFLOW:
                    {    
                        error = "STACK UNDERFLOW";
                        break;
                    }
                    case GL_OUT_OF_MEMORY:
                    {    
                        error = "OUT OF MEMORY";
                        break;
                    }
                    case GL_INVALID_FRAMEBUFFER_OPERATION:
                    {    
                        error = "INVALID FRAMEBUFFER OPERATION";
                        break;
                    }
                }

                Log("OpenGL " + std::to_string(errorCode) + ": " + msg + " | " + __FILE__ + "(" + std::to_string(__LINE__) + ")", LogLevel::Error, LogMode::Debug);
                uint errorCode = glGetError();
            }
        }
    }

    void OpenGlRenderer::Submit2dPrimitive(const Primitive2d& prim)
    {
        // @todo
    }

    void OpenGlRenderer::SubmitScreenSprite(int assetIdx, const Vector2& uvMin, const Vector2& uvMax, const Vector2& pos, short rot, const Vector2& scale,
                                            const Color& color, int depth, ScreenSpriteAlignMode alignMode, ScreenSpriteScaleMode scaleMode, BlendMode blendMode)
    {
        auto& assets = g_App.GetAssets();

        const auto asset = assets.GetAsset(assetIdx);
        if (asset->Type != AssetType::Tim)
        {
            Log("Attempted to submit non-image asset as screen sprite.", LogLevel::Warning, LogMode::Debug);
            return;
        }

        // @todo
    }

    void OpenGlRenderer::UpdateViewport()
    {
        // Set wireframe mode.
        if (g_DebugData.EnableWireframeMode)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        // Resize viewport if window is resized.
        if (_isResized)
        {
            _isResized = false;

            auto res = GetScreenResolution();
            glViewport(0, 0, res.x, res.y);
        }

        // Clear screen.
        glClearColor(_clearColor.R(), _clearColor.G(), _clearColor.B(), _clearColor.A());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    // Triangle vertices: position (x, y) and texture coordinates (u, v)
    static auto TRIANGLE_VERTICES = std::vector<float>
    {
        0.0f,  0.5f,  0.5f, 1.0f,  // Top vertex
       -0.5f, -0.5f,  0.0f, 0.0f,  // Bottom-left vertex
        0.5f, -0.5f,  1.0f, 0.0f   // Bottom-right vertex
    };

    void OpenGlRenderer::Draw3dScene()
    {
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);

        _view.Move();

        auto  res    = GetScreenResolution().ToVector2();
        float aspect = res.x / res.y;
        
        auto modelMat = Matrix::CreateRotationX(glm::radians(-55.0f));

        auto& shaderProg = _shaderPrograms.at("Default");

        shaderProg.Activate();
        shaderProg.SetMatrix("modelMat", modelMat);
        shaderProg.SetMatrix("viewMat", _view.GetMatrix(glm::radians(45.0f), aspect, 0.1f, 100.0f));
        shaderProg.SetFloat("blendAlpha", g_DebugData.BlendAlpha);

        // Cubes

        _vertexCubeBuffer.Bind();
        _vertexCubeArray.Bind();

        /*static float rot = 0.0f;
        rot += 0.5f;
        if (rot > 360.0f)
        {
            rot -= 360.0f;
        }
        for (unsigned int i = 0; i < 10; i++)
        {
            auto modelMat = Matrix::CreateTranslation(cubePositions[i]);
            modelMat.Rotate(glm::radians((20.0f * i) + rot), Vector3(1.0f, 0.3f, 0.5f));
            shaderProg.SetMatrix("modelMat", modelMat);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }*/
    }

    void OpenGlRenderer::Draw2dScene()
    {
        _2dframebuffer.Bind();
        glDepthMask(false);
        glDepthFunc(GL_LESS);

        _view.Move();

        auto  res    = GetScreenResolution().ToVector2();
        float aspect = res.x / res.y;
        
        auto modelMat = Matrix::CreateRotationX(glm::radians(-55.0f));
        auto& shaderProg = _shaderPrograms.at("Default");

        shaderProg.Activate();
        shaderProg.SetMatrix("modelMat", modelMat);
        shaderProg.SetMatrix("viewMat", _view.GetMatrix(glm::radians(45.0f), aspect, 0.1f, 100.0f));
        shaderProg.SetFloat("blendAlpha", g_DebugData.BlendAlpha);

        // Draw the triangle (this goes to the _2dframebuffer's texture)
        /*glBindVertexArray(_triangleVao);
        glBindBuffer(GL_ARRAY_BUFFER, _triangleVbo);
        glDrawArrays(GL_TRIANGLES, 0, 3);  // Draw the triangle
*/
        glDepthMask(true);
        _2dframebuffer.Unbind();
    }

    void OpenGlRenderer::DrawDebugGui()
    {
        // If debug GUI is disabled, return early.
        const auto& options = g_App.GetOptions();
        if (!options->EnableDebugGui)
        {
            _debugGuiDrawCalls.clear();
            return;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // Draw GUIs.
        for (auto& drawCall : _debugGuiDrawCalls)
        {
            drawCall();
        }
        _debugGuiDrawCalls.clear();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void OpenGlRenderer::CreateShaderProgram()
    {
        // Generate shader program.
        _shaderPrograms.emplace("Default", ShaderProgram("Default"));
        _shaderPrograms.emplace("FullscreenQuad", ShaderProgram("FullscreenQuad"));
    }
}
