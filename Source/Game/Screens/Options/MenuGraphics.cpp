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
#include "Utils/Translator.h"

using namespace Silent::Assets;

namespace Silent::Game
{
    // @temp
    int g_ExtraOptionsMenu_EntryCount;
    int g_ExtraOptionsMenu_SelectedBloodColorEntry;

    void OptionsMenu_BgmVolumeBarDraw()
    {
        OptionsMenu_VolumeBarDraw(false, g_GameWork.config.volumeBgm);
    }

    void OptionsMenu_SfxVolumeBarDraw()
    {
        OptionsMenu_VolumeBarDraw(true, g_GameWork.config.volumeSe);
    }

    void OptionsMenu_VolumeBarDraw(bool isSfx, uchar vol)
    {
        constexpr int STR_OFFSET_Y = 16;
        constexpr int NOTCH_SIZE_X = 5;
        constexpr int NOTCH_COUNT  = 16;

        // Draw bar notches.
        for (int i = 0; i < NOTCH_COUNT; i++)
        {
            for (int j = 1; j >= 0; j--)
            {
                if (i < (vol / 8))
                {
                    int shade = 160 + (64 * j);
                    //setRGBC0(poly, color0, color0, color0, 0x28);
                }
                else if (i > (vol / 8))
                {
                    int shade = 64 + (64 * j);
                    //setRGBC0(poly, color1, color1, color1, 0x28);
                }
                else
                {
                    int shade = (((vol & 0x7) * 12) + 64) + (64 * j);
                    //setRGBC0(poly, color2, color2, color2, 0x28);
                }

                int xOffset = 24 + (i * 6);
                int offset  = -69;

                int x0Offset = j + 24;
                int x0       = (x0Offset + (i * 6)) & 0xFFFF;
                int yOffset  = j + 56;

                //setXY0Fast(poly, x0,                           (isSfx * STR_OFFSET_Y) + yOffset);
                //setXY1Fast(poly, x0,                           (isSfx * STR_OFFSET_Y) - (j + offset));
                //setXY2Fast(poly, (xOffset - j) + NOTCH_SIZE_X, (isSfx * STR_OFFSET_Y) + yOffset);
                //setXY3Fast(poly, (xOffset - j) + NOTCH_SIZE_X, (isSfx * STR_OFFSET_Y) - (j + offset));
                //addPrim((u8*)ot->org + LAYER_24, poly);
                //GsOUT_PACKET_P = (u8*)poly + sizeof(POLY_F4);
            }
        }
    }

    std::pair<float, float> OptionsMenu_EntriesDraw(const std::vector<MenuEntry>& entries)
    {
        constexpr int  LINE_BASE_X     = 64;
        constexpr int  LINE_BASE_Y     = 80;
        constexpr int  LINE_OFFSET_X   = 16;
        constexpr int  LINE_OFFSET_Y   = 16;
        constexpr auto HEADING_STR_POS = Vector2i(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 6);

        const auto& translator = g_App.GetTranslator();

        Gfx_StringColorSet(StringColorId_White);

        // Submit heading string.
        Gfx_StringPositionSet(HEADING_STR_POS.x, HEADING_STR_POS.y);
        Gfx_StringDraw(translator(KEY_OPTIONS_MENU_HEADING));

        auto widths = std::pair<float, float>{};

        // Submit entry strings.
        int visibleEntriesEndIdx = std::min(g_OptionsMenu_VisibleEntriesStartIdx + VISIBLE_ENTRY_COUNT_MAX,
                                            (int)entries.size());
        for (int i = g_OptionsMenu_VisibleEntriesStartIdx;
             i < visibleEntriesEndIdx;
             i++)
        {
            int relIdx = i - g_OptionsMenu_VisibleEntriesStartIdx;
            Gfx_StringPositionSet(LINE_BASE_X, LINE_BASE_Y + (relIdx * LINE_OFFSET_Y));
            float width = Gfx_StringDraw(translator(entries[i].EntryStringKey));

            // Store line widths.
            if (i == g_OptionsMenu_SelectedEntry)
            {
                widths.second = width;
            }
            else if (i == g_OptionsMenu_PrevSelectedEntry)
            {
                widths.first = width;
            }
        }

        return widths;
    }

    void OptionsMenu_SelectionHighlightDraw(const std::pair<float, float>& widths)
    {
        constexpr int BULLET_QUAD_COUNT = 2;
        constexpr int LINE_BASE_X       = 64;//39;
        constexpr int LINE_BASE_Y       = 66;//58;
        constexpr int LINE_OFFSET_Y     = 16;

        // 12x12 quad.
        constexpr auto BULLET_QUAD_FRONT = s_Quad2d
        {
            .vertex0 = Vector2i(40, 65),
            .vertex1 = Vector2i(40, 77),
            .vertex2 = Vector2i(52, 65),
            .vertex3 = Vector2i(52, 77)
        };

        // 14x14 quad.
        constexpr auto BULLET_QUAD_BACK = s_Quad2d
        {
            .vertex0 = Vector2i(39, 64),
            .vertex1 = Vector2i(39, 76),
            .vertex2 = Vector2i(51, 64),
            .vertex3 = Vector2i(51, 76)
        };

        static auto selectionHighlightFrom = Vector2i::Zero;
        static auto selectionHighlightTo   = Vector2i::Zero;

        // @todo Widths are wrong.
        // Set active selection highlight position references.
        if (g_OptionsMenu_SelectionHighlightTimer == 0)
        {
            int entryIdxFrom       = g_OptionsMenu_PrevSelectedEntry - g_OptionsMenu_VisibleEntriesStartIdx;
            selectionHighlightFrom = Vector2i(LINE_BASE_X + (int)ceilf(widths.first),
                                              LINE_BASE_Y + (entryIdxFrom * LINE_OFFSET_Y));

            int entryIdxTo       = g_OptionsMenu_SelectedEntry - g_OptionsMenu_VisibleEntriesStartIdx;
            selectionHighlightTo = Vector2i(LINE_BASE_X + (int)ceilf(widths.second),
                                            LINE_BASE_Y + (entryIdxTo * LINE_OFFSET_Y));
        }

        // Compute sine-based interpolation alpha.
        q19_12 interpAlpha = Math_Sin(g_OptionsMenu_SelectionHighlightTimer << 7);

        // Draw active selection highlight.
        auto highlightLine      = s_Line2d{};
        highlightLine.vertex0.x = LINE_BASE_X;
        highlightLine.vertex1.x = selectionHighlightFrom.x +
                                  Q12_MULT(selectionHighlightTo.x - selectionHighlightFrom.x, interpAlpha);
        highlightLine.vertex1.y = selectionHighlightFrom.y +
                                  Q12_MULT(selectionHighlightTo.y - selectionHighlightFrom.y, interpAlpha) +
                                  LINE_OFFSET_Y;
        highlightLine.vertex0.y = highlightLine.vertex1.y;
        Options_Selection_HighlightDraw(highlightLine);

        // Draw selection bullet points.
        for (int i = 0; i < VISIBLE_ENTRY_COUNT_MAX; i++)
        {
            // Set bullet quads.
            auto bulletQuads = std::array<s_Quad2d, BULLET_QUAD_COUNT>
            {
                BULLET_QUAD_FRONT,
                BULLET_QUAD_BACK
            };
            for (auto& quad : bulletQuads)
            {
                quad.vertex0.y += i * LINE_OFFSET_Y;
                quad.vertex1.y += i * LINE_OFFSET_Y;
                quad.vertex2.y += i * LINE_OFFSET_Y;
                quad.vertex3.y += i * LINE_OFFSET_Y;
            }

            // Active selection bullet point.
            if (i == (g_OptionsMenu_SelectedEntry - g_OptionsMenu_VisibleEntriesStartIdx))
            {
                Options_Selection_BulletPointDraw(bulletQuads[0], false, false);
                Options_Selection_BulletPointDraw(bulletQuads[1], true,  false);
            }
            // Inactive selection bullet point.
            else
            {
                Options_Selection_BulletPointDraw(bulletQuads[0], false, true);
                Options_Selection_BulletPointDraw(bulletQuads[1], true,  true);
            }
        }
    }

    void OptionsMenu_ConfigDraw()
    {
        const s_Triangle2d FRONT_ARROWS[] =
        {
            { { 40,  14 }, { 48,  6  }, { 48,  22 } },
            { { 96,  14 }, { 88,  6  }, { 88,  22 } },
            { { 40,  30 }, { 48,  22 }, { 48,  38 } },
            { { 96,  30 }, { 88,  22 }, { 88,  38 } },
            { { 19,  46 }, { 27,  38 }, { 27,  54 } },
            { { 124, 46 }, { 116, 38 }, { 116, 54 } },
            { { 12,  62 }, { 20,  54 }, { 20,  70 } },
            { { 131, 62 }, { 123, 54 }, { 123, 70 } },
            { { 12,  78 }, { 20,  70 }, { 20,  86 } },
            { { 131, 78 }, { 123, 70 }, { 123, 86 } }
        };

        const s_Triangle2d BACK_ARROWS[] =
        {
            { { 39,  14 }, { 49,  4  }, { 49,  24 } },
            { { 97,  14 }, { 87,  4  }, { 87,  24 } },
            { { 39,  30 }, { 49,  20 }, { 49,  40 } },
            { { 97,  30 }, { 87,  20 }, { 87,  40 } },
            { { 18,  46 }, { 28,  36 }, { 28,  56 } },
            { { 125, 46 }, { 115, 36 }, { 115, 56 } },
            { { 11,  62 }, { 21,  52 }, { 21,  72 } },
            { { 132, 62 }, { 122, 52 }, { 122, 72 } },
            { { 11,  78 }, { 21,  68 }, { 21,  88 } },
            { { 132, 78 }, { 122, 68 }, { 122, 88 } }
        };

        const char* CONFIG_STR_KEYS[] =
        {
            KEY_OPTIONS_MENU_ON,
            KEY_OPTIONS_MENU_OFF,
            KEY_OPTIONS_MENU_STEREO,
            KEY_OPTIONS_MENU_MONAURAL
        };

        const auto& input = g_App.GetInput();
 
        //Gfx_StringColorSet(StringColorId_White);

        // Draw left/right arrows for subset of options.
        if (g_OptionsMenu_SelectedEntry >= 4 && g_OptionsMenu_SelectedEntry < 9)
        {
            // Draw flashing left/right arrows.
            for (int i = 0; i < 2; i++)
            {
                Options_Selection_ArrowDraw(FRONT_ARROWS[(((g_OptionsMenu_SelectedEntry - 4) * 2) + i)], true);
            }

            // Draw border to highlight flashing left/right arrow corresponding to direction of UI navigation.
            if (input.GetAction(In::Left).IsHeld())
            {
                Options_Selection_ArrowDraw(BACK_ARROWS[(g_OptionsMenu_SelectedEntry - 4) * 2], false);
            }
            if (input.GetAction(In::Right).IsHeld())
            {
                Options_Selection_ArrowDraw(BACK_ARROWS[((g_OptionsMenu_SelectedEntry - 4) * 2) + 1], false);
            }
        }

        for (int i = 0; i < 3; i++)
        {
            switch (i)
            {
                case 0:
                {
                    int strPosX = !g_GameWork.config.vibrationEnabled ? 214 : 216;
                    //Gfx_StringPositionSet(strPosX, 120);

                    int strIdx = g_GameWork.config.vibrationEnabled == 0;
                    //Gfx_StringDraw(OPTIONS_MENU_SOUND_ENTRY_CONFIG_STRINGS[strIdx]);
                    break;
                }
                case 1:
                {
                    int strPosX = !g_GameWork.config.autoLoad ? 214 : 216;
                    //Gfx_StringPositionSet(strPosX, 136);

                    int strIdx = g_GameWork.config.autoLoad == 0;
                    //Gfx_StringDraw(OPTIONS_MENU_SOUND_ENTRY_CONFIG_STRINGS[strIdx]);
                    break;
                }
                case 2:
                {
                    int strPosX = (g_GameWork.config.soundType != 0) ? 194 : 206;
                    //Gfx_StringPositionSet(strPosX, 152);

                    int strIdx = g_GameWork.config.soundType + 2;
                    //Gfx_StringDraw(OPTIONS_MENU_SOUND_ENTRY_CONFIG_STRINGS[strIdx]);
                    break;
                }
            }
        }
    }

    void Options_ExtraOptionsMenu_ConfigDraw()
    {
        constexpr int STR_BASE_Y   = 64;
        constexpr int STR_OFFSET_Y = 16;

        const s_Triangle2d FRONT_ARROWS[] =
        {
            { { 38,  -42 }, { 46,  -50 }, { 46,  -34 } },
            { { 120, -42 }, { 112, -50 }, { 112, -34 } },
            { { 38,  -26 }, { 46,  -34 }, { 46,  -18 } },
            { { 120, -26 }, { 112, -34 }, { 112, -18 } },
            { { 35,  -10 }, { 43,  -18 }, { 43,  -2  } },
            { { 123, -10 }, { 115, -18 }, { 115, -2  } },
            { { 35,   6  }, { 43,  -2  }, { 43,   14 } },
            { { 123,  6  }, { 115, -2  }, { 115,  14 } },
            { { 35,   22 }, { 43,   14 }, { 43,   30 } },
            { { 123,  22 }, { 115,  14 }, { 115,  30 } },
            { { 51,   38 }, { 59,   30 }, { 59,   46 } },
            { { 107,  38 }, { 99,   30 }, { 99,   46 } },
            { { 24,   54 }, { 32,   46 }, { 32,   62 } },
            { { 131,  54 }, { 123,  46 }, { 123,  62 } },
            { { 57,   70 }, { 65,   62 }, { 65,   78 } },
            { { 104,  70 }, { 96,   62 }, { 96,   78 } }
        };

        const s_Triangle2d BACK_ARROWS[] =
        {
            { { 37,  -42 }, { 47,  -52 }, { 47,  -32 } },
            { { 121, -42 }, { 111, -52 }, { 111, -32 } },
            { { 37,  -26 }, { 47,  -36 }, { 47,  -16 } },
            { { 121, -26 }, { 111, -36 }, { 111, -16 } },
            { { 34,  -10 }, { 44,  -20 }, { 44,   0  } },
            { { 124, -10 }, { 114, -20 }, { 114,  0  } },
            { { 34,   6  }, { 44,  -4  }, { 44,   16 } },
            { { 124,  6  }, { 114, -4  }, { 114,  16 } },
            { { 34,   22 }, { 44,   12 }, { 44,   32 } },
            { { 124,  22 }, { 114,  12 }, { 114,  32 } },
            { { 50,   38 }, { 60,   28 }, { 60,   48 } },
            { { 108,  38 }, { 98,   28 }, { 98,   48 } },
            { { 23,   54 }, { 33,   44 }, { 33,   64 } },
            { { 132,  54 }, { 122,  44 }, { 122,  64 } },
            { { 56,   70 }, { 66,   60 }, { 66,   80 } },
            { { 105,  70 }, { 95,   60 }, { 95,   80 } }
        };

        // TODO: Can this be split?
        const char* CONFIG_STRS[] =
        {
            "Press",
            "Switch",

            "Normal",
            "Green",
            "Violet",
            "Black",

            "_",

            "Normal",
            "Reverse",

            "On",
            "Off",

            "Normal",
            "Self_View",

            "x1",
            "x2",
            "x3",
            "x4",
            "x5",
            "x6"
        };

        const auto& input = g_App.GetInput();

        //Gfx_StringColorSet(StringColorId_White);
/*
        // Draw left/right arrows for subset of options.
        if (g_ExtraOptionsMenu_SelectedEntry < OptionsMenuEntry_Count)
        {
            // Draw flashing left/right arrows.
            for (int i = 0; i < 2; i++)
            {
                Options_Selection_ArrowDraw(FRONT_ARROWS[(g_ExtraOptionsMenu_SelectedEntry * 2) + i], true);
            }

            // Draw border to highlight flashing left/right arrow corresponding to direction of UI navigation.
            if (input.GetAction(In::Left).IsHeld())
            {
                Options_Selection_ArrowDraw(BACK_ARROWS[g_ExtraOptionsMenu_SelectedEntry << 1], false);
            }
            if (input.GetAction(In::Right).IsHeld())
            {
                Options_Selection_ArrowDraw(BACK_ARROWS[(g_ExtraOptionsMenu_SelectedEntry << 1) + 1], false);
            }
        }

        // Submit entry strings.
        for (int i = 0; i < g_ExtraOptionsMenu_EntryCount; i++)
        {
            switch (i)
            {
                case OptionsMenuEntry_WeaponCtrl:
                {
                    int strPosX = (g_GameWork.config.extraWeaponCtrl != 0) ? 217 : 212;
                    //Gfx_StringPositionSet(strPosX, STR_BASE_Y + (STR_OFFSET_Y * OptionsMenuEntry_WeaponCtrl));
                    //Gfx_StringDraw(CONFIG_STRS[!g_GameWork.config.extraWeaponCtrl]);
                    break;
                }
                case OptionsMenuEntry_Blood:
                {
                    switch (g_ExtraOptionsMenu_SelectedBloodColorEntry)
                    {
                        case BloodColorMenuEntry_Normal:
                        {
                            //Gfx_StringPositionSet(210, STR_BASE_Y + (STR_OFFSET_Y * OptionsMenuEntry_Blood));
                            break;
                        }
                        case BloodColorMenuEntry_Green:
                        {
                            //Gfx_StringPositionSet(214, STR_BASE_Y + (STR_OFFSET_Y * OptionsMenuEntry_Blood));
                            break;
                        }
                        case BloodColorMenuEntry_Violet:
                        {
                            //Gfx_StringPositionSet(214, STR_BASE_Y + (STR_OFFSET_Y * OptionsMenuEntry_Blood));
                            break;
                        }
                        case BloodColorMenuEntry_Black:
                        {
                            //Gfx_StringPositionSet(217, STR_BASE_Y + (STR_OFFSET_Y * OptionsMenuEntry_Blood));
                            break;
                        }
                    }

                    //Gfx_StringDraw(CONFIG_STRS[g_ExtraOptionsMenu_SelectedBloodColorEntry + 2]);
                    break;
                }
                case OptionsMenuEntry_ViewCtrl:
                {
                    int strPosX = !g_GameWork.config.extraViewCtrl ? 210 : 206;
                    //Gfx_StringPositionSet(strPosX, STR_BASE_Y + (STR_OFFSET_Y * OptionsMenuEntry_ViewCtrl));
                    //Gfx_StringDraw(CONFIG_STRS[((g_GameWork.config.extraViewCtrl != 0) ? 32 : 28) >> 2]);
                    break;
                }
                case OptionsMenuEntry_RetreatTurn:
                {
                    int strPosX = !g_GameWork.config.extraRetreatTurn ? 210 : 206;
                    //Gfx_StringPositionSet(strPosX, STR_BASE_Y + (STR_OFFSET_Y * OptionsMenuEntry_RetreatTurn));
                    //Gfx_StringDraw(CONFIG_STRS[((g_GameWork.config.extraRetreatTurn != 0) ? 32 : 28) >> 2]);
                    break;
                }
                case OptionsMenuEntry_Control:
                {
                    int strPosX = !g_GameWork.config.extraWalkRunCtrl ? 210 : 206;
                    //Gfx_StringPositionSet(strPosX, STR_BASE_Y + (STR_OFFSET_Y * OptionsMenuEntry_Control));
                    //Gfx_StringDraw(CONFIG_STRS[((g_GameWork.config.extraWalkRunCtrl != 0) ? 32 : 28) >> 2]);
                    break;
                }
                case OptionsMenuEntry_AutoAiming:
                {
                    int strPosX = !g_GameWork.config.extraAutoAiming ? 228 : 226;
                    //Gfx_StringPositionSet(strPosX, STR_BASE_Y + (STR_OFFSET_Y * OptionsMenuEntry_AutoAiming));
                    //Gfx_StringDraw(CONFIG_STRS[((g_GameWork.config.extraAutoAiming != 0) ? 40 : 36) >> 2]);
                    break;
                }
                case OptionsMenuEntry_ViewMode:
                {
                    int strPosX = !g_GameWork.config.extraViewMode ? 210 : 200;
                    //Gfx_StringPositionSet(strPosX, STR_BASE_Y + (STR_OFFSET_Y * OptionsMenuEntry_ViewMode));
                    //Gfx_StringDraw(CONFIG_STRS[(g_GameWork.config.extraViewMode ? 48 : 44) >> 2]);
                    break;
                }
                case OptionsMenuEntry_BulletAdjust:
                {
                    //Gfx_StringPositionSet(230, STR_BASE_Y + (STR_OFFSET_Y * OptionsMenuEntry_BulletAdjust));
                    //Gfx_StringDraw(CONFIG_STRS[g_GameWork.config.extraBulletAdjust + 13]);
                    break;
                }
            }
        }
*/
    }
}
