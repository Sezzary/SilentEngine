#include "Framework.h"
#include "Debug/Menu/Tabs/General.h"

#include "Debug/GameData.h"

namespace Silent::Debug
{
    void AddGeneralTab()
    {
        if (ImGui::BeginTabItem("General"))
        {
            if (ImGui::BeginTable("Status", 2))
            {
                // `Sys state` info.
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Sys state:", 0, 0);
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%d", *g_SysState, 0, 1);

                // `Sys state steps` info.
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Sys state steps:", 1, 0);
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%d, %d, %d", g_SysStateSteps[0], g_SysStateSteps[1], g_SysStateSteps[2], 1, 1);

                // `Sys state steps data` info.
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Sys state steps data:", 2, 0);
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%d, %d", g_SysStateStepData[0], g_SysStateStepData[1], 2, 1);

                // `Sys state counter` info.
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Sys state counter:", 3, 0);
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%d", *g_SysStateCounter, 3, 1);

                // `Game state` info.
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Game state:", 4, 0);
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%d", *g_GameState, 4, 1);

                // `Game state steps` info.
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Game state steps:", 5, 0);
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%d, %d, %d", g_GameStateSteps[0], g_GameStateSteps[1], g_GameStateSteps[2], 5, 1);

                // `Game state counter` info.
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Game state counter:", 6, 0);
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%d", *g_GameStateCounter, 6, 1);

                // `Game state step counter` info.
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Game state step counter:", 7, 0);
                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%d", *g_GameStateStepCounter, 7, 1);

                ImGui::EndTable();
            }

            ImGui::EndTabItem();
        }
    }
}