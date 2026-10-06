#include "Framework.h"
#include "Psx.h"
#include "Game/Screens/Options/MenuGraphics.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Application.h"
#include "Assets/TranslationKeys.h"
#include "Game/Bodyprog/Events/MapMsg.h"
#include "Game/Bodyprog/Text/TextDraw.h"
#include "Game/Screens/Options/Options.h"
#include "Game/Screens/Options/SelectionGraphics.h"
#include "Game/Screens/Options/Utils.h"
#include "Renderer/Renderer.h"
#include "Utils/Translator.h"

using namespace Silent::Assets;
using namespace Silent::Renderer;

namespace Silent::Game
{
    static void Options_DrawConfigString(const Vector2i& pos, const std::string& str, bool isSelectedEntry)
    {
        constexpr int ARROW_SPACE = 4;

        const auto& input = g_App.GetInput();

        // Submit string.
        Gfx_StringPositionSet(pos.x, pos.y);
        float width = Gfx_StringDraw("{M}" + str) * (RETRO_SCREEN_SPACE_RES.y / SCREEN_SPACE_RES.y);

        // Submit arrows.
        if (isSelectedEntry)
        {
            bool isLeftHeld  = input.GetAction(In::Left).IsHeld(0.0f, GUI_PULSE_STATE_MIN);
            bool isRightHeld = input.GetAction(In::Right).IsHeld(0.0f, GUI_PULSE_STATE_MIN);

            int arrowOffset = (int)ceilf(width * 0.5f) + ARROW_SPACE;
            Options_DrawArrow(pos - Vector2i(arrowOffset, 0), SelectionArrowType::Left,  isLeftHeld  && !isRightHeld);
            Options_DrawArrow(pos + Vector2i(arrowOffset, 0), SelectionArrowType::Right, isRightHeld && !isLeftHeld);
        }
    }

    static void Options_DrawConfigKey(const Vector2i& pos, const std::string& key, bool isSelectedEntry)
    {
        const auto& translator = g_App.GetTranslator();

        Options_DrawConfigString(pos, translator(key), isSelectedEntry);
    }

    /** @brief Submits a notched bar to draw.
     *
     * @param pos Screen position in retro pixels (320x240).
     * @param activeCount Active notch count.
     * @param isSelectedEntry Is highlighted by the cursor.
     */
    static void Options_DrawConfigBar(const Vector2i& pos, int activeCount, bool isSelectedEntry)
    {
        constexpr int  DEPTH                = 24;
        constexpr int  ARROW_SPACE          = 4;
        constexpr int  NOTCH_SPACE          = 6;
        constexpr auto NOTCH_QUAD_BACK_BASE = s_Quad2d
        {
            .vertex0 = Vector2i(0, -13),
            .vertex1 = Vector2i(0,  0),
            .vertex2 = Vector2i(5, -13),
            .vertex3 = Vector2i(5,  0)
        };
        constexpr auto NOTCH_QUAD_FRONT_BASE = s_Quad2d
        {
            .vertex0 = Vector2i(1, -12),
            .vertex1 = Vector2i(1, -1),
            .vertex2 = Vector2i(4, -12),
            .vertex3 = Vector2i(4, -1)
        };

        constexpr auto COLOR_ACTIVE_FRONT   = Color::From8Bit(230, 230, 230);
        constexpr auto COLOR_ACTIVE_BACK    = Color::From8Bit(164, 164, 164);
        constexpr auto COLOR_INACTIVE_FRONT = Color::From8Bit(131, 131, 131);
        constexpr auto COLOR_INACTIVE_BACK  = Color::From8Bit(65,  65,  65);

        const auto& input    = g_App.GetInput();
        auto&       renderer = g_App.GetRenderer();

        // Submit notches.
        int submittedCount = 0;
        for (int i = -(BAR_NOTCH_COUNT / 2); i < (BAR_NOTCH_COUNT / 2); i++)
        {
            submittedCount++;

            bool isActive = submittedCount <= activeCount;
            auto offset   = Vector2i(i * NOTCH_SPACE, 0);

            // Submit back quad.
            auto backQuad = Shape2d::CreateQuad((pos + offset)+ NOTCH_QUAD_BACK_BASE.vertex0,
                                                (pos + offset)+ NOTCH_QUAD_BACK_BASE.vertex1,
                                                (pos + offset)+ NOTCH_QUAD_BACK_BASE.vertex2,
                                                (pos + offset)+ NOTCH_QUAD_BACK_BASE.vertex3,
                                                isActive ? COLOR_ACTIVE_BACK : COLOR_INACTIVE_BACK,
                                                isActive ? COLOR_ACTIVE_BACK : COLOR_INACTIVE_BACK,
                                                isActive ? COLOR_ACTIVE_BACK : COLOR_INACTIVE_BACK,
                                                isActive ? COLOR_ACTIVE_BACK : COLOR_INACTIVE_BACK,
                                                DEPTH, ScaleMode::VerticalEdge, BlendMode::Opaque);
            renderer.SubmitShape2d(backQuad);

            // Submit front quad.
            auto frontQuad = Shape2d::CreateQuad((pos + offset) + NOTCH_QUAD_FRONT_BASE.vertex0,
                                                 (pos + offset) + NOTCH_QUAD_FRONT_BASE.vertex1,
                                                 (pos + offset) + NOTCH_QUAD_FRONT_BASE.vertex2,
                                                 (pos + offset) + NOTCH_QUAD_FRONT_BASE.vertex3,
                                                 isActive ? COLOR_ACTIVE_FRONT : COLOR_INACTIVE_FRONT,
                                                 isActive ? COLOR_ACTIVE_FRONT : COLOR_INACTIVE_FRONT,
                                                 isActive ? COLOR_ACTIVE_FRONT : COLOR_INACTIVE_FRONT,
                                                 isActive ? COLOR_ACTIVE_FRONT : COLOR_INACTIVE_FRONT,
                                                 DEPTH - 1, ScaleMode::VerticalEdge, BlendMode::Opaque);
            renderer.SubmitShape2d(frontQuad);
        }

        // Submit arrows.
        if (isSelectedEntry)
        {
            bool isLeftHeld  = input.GetAction(In::Left).IsHeld(0.0f, GUI_PULSE_STATE_MIN);
            bool isRightHeld = input.GetAction(In::Right).IsHeld(0.0f, GUI_PULSE_STATE_MIN);

            int arrowOffset = ((BAR_NOTCH_COUNT / 2) * NOTCH_SPACE) + ARROW_SPACE;
            Options_DrawArrow(pos - Vector2i(arrowOffset, 0), SelectionArrowType::Left,  isLeftHeld  && !isRightHeld);
            Options_DrawArrow(pos + Vector2i(arrowOffset, 0), SelectionArrowType::Right, isRightHeld && !isLeftHeld);
        }
    }

    /** @brief Submits gold bullet points next to the listed entries and a highlight indicating the selected entry to
     * draw in options menus.
     *
     * @param @todo
     */
    static void Options_DrawSelectionHighlight(const std::pair<int, int>& widths)
    {
        constexpr int  ENTRY_OFFSET_X = 25;
        constexpr auto LINE_BASE      = Vector2i(31, 72);
        constexpr int  LINE_HEIGHT    = 16;

        static auto selectionHighlightFrom = Vector2i::Zero;
        static auto selectionHighlightTo   = Vector2i::Zero;

        // @todo Account for scrolling.
        // Set active selection highlight position references.
        if (g_OptionsMenu_SelectionHighlightTimer == Q12(0.0f))
        {
            int entryIdxFrom = g_OptionsMenu_PrevSelectedEntry - g_OptionsMenu_VisibleEntriesStartIdx;
            int entryIdxTo   = g_OptionsMenu_SelectedEntry     - g_OptionsMenu_VisibleEntriesStartIdx;

            selectionHighlightFrom = LINE_BASE + Vector2i(ENTRY_OFFSET_X + widths.first,  entryIdxFrom * LINE_HEIGHT);
            selectionHighlightTo   = LINE_BASE + Vector2i(ENTRY_OFFSET_X + widths.second, entryIdxTo   * LINE_HEIGHT);
        }

        // Compute sine-based interpolation alpha. @todo Sine-based math is wrong, using linear for now.
        //q19_12 interpAlpha = Math_Sin(g_OptionsMenu_SelectionHighlightTimer);
        q19_12 interpAlpha = Q12_DIV(g_OptionsMenu_SelectionHighlightTimer, LINE_CURSOR_TIMER_MAX);

        // Draw active selection highlight.
        auto highlightLine      = s_Line2d{};
        highlightLine.vertex0.x = LINE_BASE.x;
        highlightLine.vertex1.x = selectionHighlightFrom.x +
                                  Q12_MULT(selectionHighlightTo.x - selectionHighlightFrom.x, interpAlpha);
        highlightLine.vertex1.y = selectionHighlightFrom.y +
                                  Q12_MULT(selectionHighlightTo.y - selectionHighlightFrom.y, interpAlpha);
        highlightLine.vertex0.y = highlightLine.vertex1.y;
        Options_DrawHighlight(highlightLine);
    }

    void Options_DrawEntries(const std::string& headingStrKey, const std::vector<MenuEntry>& entries)
    {
        constexpr auto HEADING_STR_POS = Vector2i(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 7);
        constexpr auto LINE_BASE       = Vector2i(56, 70);
        constexpr auto BULLET_OFFSET   = Vector2i(-25, 2);
        constexpr auto LINE_HEIGHT     = 16;
        constexpr auto CONFIG_OFFSET   = Vector2i(194, 0);

        const auto& input      = g_App.GetInput();
        const auto& translator = g_App.GetTranslator();

        Gfx_StringColorSet(StringColorId_White);

        // Submit heading string.
        Gfx_StringPositionSet(HEADING_STR_POS.x, HEADING_STR_POS.y);
        Gfx_StringDraw("{M}" + translator(headingStrKey));

        int visibleEntriesEndIdx = std::min(g_OptionsMenu_VisibleEntriesStartIdx + VISIBLE_ENTRY_COUNT_MAX,
                                            (int)entries.size());

        // Run through entries.
        auto widths = std::pair<int, int>{};
        for (int i = g_OptionsMenu_VisibleEntriesStartIdx; i < visibleEntriesEndIdx; i++)
        {
            const auto& entry = entries[i];

            // Compute parameters.
            bool isSelected = i == g_OptionsMenu_SelectedEntry;
            int  relIdx     = i - g_OptionsMenu_VisibleEntriesStartIdx;
            auto pos        = LINE_BASE + Vector2i(0, relIdx * LINE_HEIGHT);
            auto configPos  = pos + CONFIG_OFFSET;

            // Submit bullet point.
            auto bulletPos = (LINE_BASE + BULLET_OFFSET) +
                             Vector2i(0, (i - g_OptionsMenu_VisibleEntriesStartIdx) * LINE_HEIGHT);
            bool isActive  = i == (g_OptionsMenu_SelectedEntry - g_OptionsMenu_VisibleEntriesStartIdx);
            Options_DrawBulletPoint(bulletPos, isActive);

            // Submit string.
            Gfx_StringPositionSet(pos.x, pos.y);
            float width = Gfx_StringDraw(translator(entry.EntryStringKey)) *
                          (RETRO_SCREEN_SPACE_RES.y / SCREEN_SPACE_RES.y);

            // Store line widths. @todo Scrolling issues.
            if (i == (g_OptionsMenu_PrevSelectedEntry - g_OptionsMenu_VisibleEntriesStartIdx))
            {
                widths.first = (int)ceilf(width);
            }
            else if (i == (g_OptionsMenu_SelectedEntry - g_OptionsMenu_VisibleEntriesStartIdx))
            {
                widths.second = (int)ceilf(width);
            }

            // Submit config graphics.
            if (std::holds_alternative<MenuEntryBoolBinding>(entry.Binding))
            {
                const auto& binding   = std::get<MenuEntryBoolBinding>(entry.Binding);
                const char* configKey = binding.GetState() ? KEY_OPTIONS_MENU_CONFIG_ON : KEY_OPTIONS_MENU_CONFIG_OFF;

                Options_DrawConfigKey(configPos, configKey, isSelected);
            }
            else if (std::holds_alternative<MenuEntryRangeBinding>(entry.Binding))
            {
                const auto& binding = std::get<MenuEntryRangeBinding>(entry.Binding);

                int configVal = binding.GetValue();
                Options_DrawConfigString(configPos, binding.Prefix + std::to_string(configVal), isSelected);
            }
            else if (std::holds_alternative<MenuEntryEnumBinding>(entry.Binding))
            {
                const auto& binding = std::get<MenuEntryEnumBinding>(entry.Binding);

                int configIdx = binding.GetIdx();
                if (configIdx >= 0 && configIdx < binding.ConfigStringKeys.size())
                {
                    const auto& configKey = binding.ConfigStringKeys[configIdx];

                    Options_DrawConfigKey(configPos, configKey, isSelected);
                }
            }
            else if (std::holds_alternative<MenuEntryBarBinding>(entry.Binding))
            {
                const auto& binding = std::get<MenuEntryBarBinding>(entry.Binding);

                int activeCount = (int)floorf(((float)binding.GetValue() / (float)binding.Max) * BAR_NOTCH_COUNT);
                Options_DrawConfigBar(configPos, activeCount, isSelected);
            }
            else if (std::holds_alternative<MenuEntryLanguageBinding>(entry.Binding))
            {
                const auto& binding = std::get<MenuEntryLanguageBinding>(entry.Binding);

                const auto& translator = g_App.GetTranslator();
                const auto& locale     = translator.GetLocales()[binding.GetLocaleIdx()];

                Options_DrawConfigString(pos + CONFIG_OFFSET, locale.Label, isSelected);
            }
        }

        Options_DrawSelectionHighlight(widths);
    }
}
