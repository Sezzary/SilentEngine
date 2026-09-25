#include "Framework.h"
#include "Debug/Menu/Tabs/Renderer.h"

#include "Application.h"
#include "Debug/Debug.h"
#include "Debug/GameData.h"
#include "Renderer/Renderer.h"
#include "Services/Clock.h"

using namespace Silent::Renderer;
using namespace Silent::Services;

namespace Silent::Debug
{
    void AddRendererTab()
    {
        const auto& clock    = g_App.GetClock();
        const auto& renderer = g_App.GetRenderer();

        if (ImGui::BeginTabItem("Renderer"))
        {
            g_Work.Page = Page::Renderer;

            // `Status` section.
            ImGui::SeparatorText("Status");
            {
                if (ImGui::BeginTable("Status", 2))
                {
                    // `Window resolution` info.
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Window resolution:", 0, 0);
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d, %d", g_App.GetWindowResolution().x, g_App.GetWindowResolution().y, 0, 1);

                    auto perf = clock.GetPerformance();

                    // `FPS` info.
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("FPS:", 0, 0);
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", (int)roundf(perf.Fps), 0, 1);

                    // `Frame time` info.
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Frame time (microsec):", 1, 0);
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", perf.FrameTime, 1, 1);

                    // `Draw calls` info.
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("Draw calls:", 2, 0);
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", renderer.GetDrawCallCount(), 2, 1);

                    ImGui::EndTable();
                }
            }

            // `Wireframe mode` checkbox.
            ImGui::Checkbox("Wireframe mode", &g_Work.EnableWireframeMode);

            ImGui::EndTabItem();
        }
    }
}
