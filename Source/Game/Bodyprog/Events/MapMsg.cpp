#include "Framework.h"
#include "Psx.h"
#include "Game/Bodyprog/Events/MapMsg.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Application.h"
#include "Game/Bodyprog/Screen/ScreenData.h"
#include "Game/Bodyprog/Screen/ScreenDraw.h"
#include "Game/Bodyprog/Sound/SoundSystem.h"
#include "Game/Bodyprog/Text/TextDraw.h"
#include "Game/Main/FsQueue.h"
#include "Input/Input.h"
#include "Renderer/Common/Utils.h"

using namespace Silent::Input;
using namespace Silent::Utils;

namespace Silent::Game
{
    static s32   g_MapMsg_CurrentIdx       = 0;
    static int   g_MapMsg_CurrentPageIdx   = 0;
    static q3_12 g_MapMsg_SelectFlashTimer = 0;
    static auto  g_MapMsg_Cached           = ParsedMsg{};

    s_MapMsgSelect g_MapMsg_Select;
    s8             g_MapMsg_SelectCancelIdx;

    /** @brief Draws a string in screen space using 12x16 glyphs and returns a map message code.
     *
     * @param mapMsg Map message to draw.
     * @param displayLength Number of consecutive glyphs to draw from the map message.
     * @param pageIdx Message page index.
     * @return Message return code (`e_MsgReturnCode`).
     */
    static e_MsgReturnCode Gfx_MapMsg_StringDraw(const std::string& mapMsg, int displayLength, int pageIdx,
                                                 bool isHalfHeight = false)
    {
        int styleFlags = (int)TextStyleFlags::Gradient |
                         (int)TextStyleFlags::Shadow   |
                         g_SysWork.enableHalfHeightGlyphs ? (int)TextStyleFlags::HalfHeight : (int)TextStyleFlags::None;
        DrawParsedMsg(g_MapMsg_Cached, ConvertRetroScreenPixelsToPercent(g_StringPosition), SERIF_FONT_SCALE,
                      styleFlags, displayLength, pageIdx);
    }

    s32 Gfx_MapMsg_Draw(s32 mapMsgIdx)
    {
        constexpr int FINISH_CUTSCENE = 0xFF;
        constexpr int FINISH_MAP_MSG  = 0xFF;

        const auto& input   = g_App.GetInput();
        const auto& options = g_App.GetOptions();

        static int  stateMachineIdx0;
        static int  stateMachineIdx1;
        static int  displayLength;
        static int  activeMapMsgIdx;
        static int  displayLengthInc;
        static bool loadAudio;

        s32 temp_s1;
        s32 temp;
        s32 var_a1;

        // Check for user input.
        bool hasInput = false;
        if (input.GetAction(In::Enter).IsClicked() ||
            input.GetAction(In::Cancel).IsClicked())
        {
            hasInput = true;
        }

        g_SysWork.playerWork.player.properties.player.gasWeaponPowerTimer = Q12(0.0f);
        //func_8004C564(g_SysWork.playerCombat.weaponAttack, WEAPON_ATTACK(EquippedWeaponId_RockDrill, AttackInputType_Tap));

        // Cache new message.
        auto fontName = (options->TextQuality == TextQualityType::Retro) ? "RetroSerif" : "ModernSerif";
        if (activeMapMsgIdx != mapMsgIdx || fontName != g_MapMsg_Cached.FontName)
        {
            g_MapMsg_Cached = GetParsedMsg(g_MapOverlayHdr.mapMessages[mapMsgIdx], fontName, SERIF_FONT_SCALE);

            if (activeMapMsgIdx != mapMsgIdx)
            {
                g_SysWork.isMgsStringSet = false;
            }
        }

        switch (g_SysWork.isMgsStringSet)
        {
            case true:
            {
                if (g_SysWork.bgmStatusFlags & BgmStatusFlag_VoiceDialog)
                {
                    /*if (Sd_AudioStreamingCheck() == 4)
                    {
                        loadAudio = false;
                        break;
                    }*/

                    if (loadAudio)
                    {
                        break;
                    }
                }
                else
                {
                    loadAudio = false;
                }

                Gfx_StringColorSet(StringColorId_White);
                Gfx_StringPositionSet(40, 160);

                displayLength += displayLengthInc;
                displayLength  = CLAMP(displayLength, 0, MAP_MESSAGE_DISPLAY_ALL_LENGTH);

                if (g_MapMsg_AudioLoadBlock != 0 && g_SysWork.mapMsgTimer > Q12(0.0f))
                {
                    g_SysWork.mapMsgTimer -= g_DeltaTimeRaw;
                    g_SysWork.mapMsgTimer  = CLAMP(g_SysWork.mapMsgTimer, Q12(0.0f), INT_MAX);
                }

                temp_s1 = stateMachineIdx0;
                if (temp_s1 == NO_VALUE)
                {
                    if (g_MapMsg_AudioLoadBlock == 0)
                    {
                        //Game_TimerUpdate();
                    }

                    temp = stateMachineIdx1;
                    if (temp == temp_s1)
                    {
                        if (g_MapMsg_Select.maxIdx == temp)
                        {
                            if (!((g_MapMsg_AudioLoadBlock & (1 << 0)) || !hasInput) ||
                                (g_MapMsg_AudioLoadBlock != 0 && g_SysWork.mapMsgTimer == Q12(0.0f)))
                            {
                                stateMachineIdx1 = FINISH_MAP_MSG;

                                if (g_SysWork.bgmStatusFlags & BgmStatusFlag_VoiceDialog)
                                {
                                    SD_Call(19);
                                }
                                break;
                            }
                        }
                        else if (input.GetAction(In::Cancel).IsClicked())
                        {
                            g_MapMsg_Select.maxIdx           = temp;
                            g_MapMsg_Select.selectedEntryIdx = g_MapMsg_SelectCancelIdx;

                            //Sd_SfxPlay(Sfx_MenuCancel, 0, Q8_CLAMPED(0.25f));

                            if (g_SysWork.silentYesSelection)
                            {
                                g_SysWork.silentYesSelection = false;
                            }

                            stateMachineIdx1 = FINISH_MAP_MSG;
                            break;
                        }
                        else if (input.GetAction(In::Enter).IsClicked())
                        {
                            g_MapMsg_Select.maxIdx = temp;

                            if (g_MapMsg_Select.selectedEntryIdx == (s8)g_MapMsg_SelectCancelIdx)
                            {
                                //Sd_SfxPlay(Sfx_MenuCancel, 0, Q8_CLAMPED(0.25f));
                            }
                            else if (!g_SysWork.silentYesSelection)
                            {
                                //Sd_SfxPlay(Sfx_MenuConfirm, 0, Q8_CLAMPED(0.25f));
                            }

                            if (g_SysWork.silentYesSelection)
                            {
                                g_SysWork.silentYesSelection = false;
                            }

                            stateMachineIdx1 = FINISH_MAP_MSG;
                            break;
                        }
                    }
                    else if ((!(g_MapMsg_AudioLoadBlock & (1 << 0)) && hasInput && g_MapMsg_Select.maxIdx != 0) ||
                            (g_MapMsg_AudioLoadBlock != 0 && g_SysWork.mapMsgTimer == Q12(0.0f)))
                    {
                        if (g_MapMsg_Select.maxIdx != NO_VALUE)
                        {
                            g_MapMsg_Select.maxIdx = NO_VALUE;
                            stateMachineIdx1       = FINISH_MAP_MSG;
                            break;
                        }

                        g_MapMsg_CurrentIdx++;
                        g_SysWork.mapMsgTimer = g_MapMsg_Select.maxIdx;

                        //var_a1 = Gfx_MapMsg_WidthsCompute(g_MapMsg_CurrentIdx);

                        displayLength = 0;
                        stateMachineIdx0 = 0;

                        if (g_MapMsg_AudioLoadBlock == MapMsgAudioLoadBlock_J2)
                        {
                            loadAudio = false;
                            return MapMsgState_Idle;
                        }

                        if (g_SysWork.bgmStatusFlags & BgmStatusFlag_VoiceDialog)
                        {
                            SD_Call(19);
                        }

                        loadAudio = true;
                        return MapMsgState_Finish;
                    }
                }
                else
                {
                    if (hasInput)
                    {
                        displayLength = MAP_MESSAGE_DISPLAY_ALL_LENGTH;
                    }
                }

                stateMachineIdx0 = 0;
                stateMachineIdx1 = Gfx_MapMsg_SelectionUpdate(g_MapMsg_CurrentIdx, &displayLength);

                if (stateMachineIdx1 != 0 && stateMachineIdx1 < MsgReturnCode_Select4)
                {
                    stateMachineIdx0 = NO_VALUE;
                }
            }
            case false:
            {
                g_SysWork.mapMsgTimer            = NO_VALUE;
                g_MapMsg_Select.maxIdx           = NO_VALUE;
                g_MapMsg_Select.selectedEntryIdx = 0;
                g_MapMsg_AudioLoadBlock          = 0;
                g_MapMsg_CurrentIdx              = mapMsgIdx;
                stateMachineIdx0                 = 0;
                stateMachineIdx1                 = 0;
                activeMapMsgIdx                  = mapMsgIdx;
                displayLength                    = 0;
                displayLengthInc                 = 2; // Advance 2 glyphs at a time.

                Gfx_MapMsg_Reset();
                //var_a1 = Gfx_MapMsg_WidthsCompute(g_MapMsg_CurrentIdx);

                loadAudio                = true;
                g_SysWork.isMgsStringSet = true;
                return MapMsgState_Finish;
            }
        }

        if (stateMachineIdx1 != FINISH_MAP_MSG)
        {
            return MapMsgState_Idle;
        }

        g_SysWork.isMgsStringSet = false;
        displayLength         = 0;

        if (g_SysWork.bgmStatusFlags & BgmStatusFlag_VoiceDialog)
        {
            loadAudio = true;
        }

        return g_MapMsg_Select.selectedEntryIdx + 1;
    }

    s32 Gfx_MapMsg_SelectionUpdate(u8 mapMsgIdx, s32* displayLength)
    {
        constexpr int STRING_LINE_OFFSET = 16;

        int mapMsgCode = Gfx_MapMsg_StringDraw(g_MapOverlayHdr.mapMessages[mapMsgIdx], *displayLength,
                                               g_MapMsg_CurrentPageIdx);

        g_MapMsg_SelectFlashTimer += g_DeltaTimeRaw;
        if (g_MapMsg_SelectFlashTimer >= Q12(0.5f))
        {
            g_MapMsg_SelectFlashTimer -= Q12(0.5f);
        }

        switch (mapMsgCode)
        {
            case NO_VALUE:
            case MsgReturnCode_None:
                g_MapMsg_SelectFlashTimer = Q12(0.0f);
                break;

            case MsgReturnCode_EndPage:
                g_MapMsg_CurrentPageIdx++;
                break;

            case MsgReturnCode_End:
                g_MapMsg_CurrentPageIdx = 0;
                break;

            case MsgReturnCode_Select2:
            case MsgReturnCode_Select3:
            case MsgReturnCode_Select4:
                g_MapMsg_Select.maxIdx   = 1;
                g_MapMsg_SelectCancelIdx = (mapMsgCode == MsgReturnCode_Select3) ? 2 : 1;

                if (mapMsgCode == MsgReturnCode_Select4)
                {
                    // Shows selection prompt with map messages at indices 0 and 1.
                    // All maps have "Yes" and "No" as messages 0 and 1, respectively.
                    for (int i = 0; i < 2; i++)
                    {
                        if (g_MapMsg_Select.selectedEntryIdx == i)
                        {
                            Gfx_StringColorSet((e_StringColorId)(((g_MapMsg_SelectFlashTimer >> 10) * 3) + 4));
                        }
                        else
                        {
                            Gfx_StringColorSet(StringColorId_White);
                        }

                        Gfx_StringPositionSet(32, (STRING_LINE_OFFSET * i) + 98);
                        Gfx_StringDraw(g_MapOverlayHdr.mapMessages[i], MAP_MESSAGE_DISPLAY_ALL_LENGTH);
                    }

                    mapMsgCode = MsgReturnCode_Select2;
                }
                else
                {
                    // Shows selection prompt with 2 or 3 map messages from current index + 1/2/3.
                    // Requires prompt options to be arranged sequentially in the map message array, e.g.
                    // `[idx]`:     "Select one of 3 options. ~S3"
                    // `[idx + 1]`: "Option 1"
                    // `[idx + 2]`: "Option 2"
                    // `[idx + 3]`: "Option 3"
                    for (int i = 0; i < mapMsgCode; i++)
                    {
                        if (g_MapMsg_Select.selectedEntryIdx == i)
                        {
                            Gfx_StringColorSet((e_StringColorId)(((g_MapMsg_SelectFlashTimer >> 10) * 3) + 4));
                        }
                        else
                        {
                            Gfx_StringColorSet(StringColorId_White);
                        }

                        Gfx_StringPositionSet(32, (STRING_LINE_OFFSET * i) + 96);
                        Gfx_StringDraw(g_MapOverlayHdr.mapMessages[(mapMsgIdx + i) + 1], MAP_MESSAGE_DISPLAY_ALL_LENGTH);
                    }
                }

                if (g_Controller0->buttonFlags.clicked & ControllerFlag_LStickHighUp &&
                    g_MapMsg_Select.selectedEntryIdx != 0)
                {
                    g_MapMsg_SelectFlashTimer = Q12(0.0f);
                    g_MapMsg_Select.selectedEntryIdx--;

                    //Sd_SfxPlay(Sfx_MenuMove, 0, Q8_CLAMPED(0.25f));
                }

                if (g_Controller0->buttonFlags.clicked & ControllerFlag_LStickHighDown &&
                    g_MapMsg_Select.selectedEntryIdx != (mapMsgCode - 1))
                {
                    g_MapMsg_SelectFlashTimer = Q12(0.0f);
                    g_MapMsg_Select.selectedEntryIdx++;

                    //Sd_SfxPlay(Sfx_MenuMove, 0, Q8_CLAMPED(0.25f));
                }

                mapMsgCode = NO_VALUE;
                break;

            // @todo This case is inactive.
            case MsgReturnCode_DisplayAll:
                *displayLength = MAP_MESSAGE_DISPLAY_ALL_LENGTH;
                break;
        }

        if (g_SysWork.mapMsgDisplayAll)
        {
            *displayLength = MAP_MESSAGE_DISPLAY_ALL_LENGTH;
        }

        return mapMsgCode;
    }

    void func_8003708C(s16* ptr0, u16* ptr1) // 0x8003708C
    {
        s32 var0;
        s16 var1;
        s32 var3;
        s32 shift;
        s32 i;

        var0 = 0;
        var1 = 0;

        for (i = 0; i < 12; i++)
        {
            shift = (i & 0x3) * 4;
            var3  = (*ptr1 >> shift) & 0xF;
            if (i != 0 && var3 == 11 && var0 != 0)
            {
                var1 |= 11 << shift;
            }

            var0 = 0;
            if (var3 != 0 && var3 != 11)
            {
                var1 |= 11 << shift;
                var0  = 1;
            }

            if ((i & 0x3) == 3 || i == 12)
            {
                ptr1++;
                *ptr0++ = var1;
                var1    = 0;
            }
        }
    }

    void func_80037124() // 0x80037124
    {
        g_MapMsg_Select.maxIdx = NO_VALUE;
        //func_8003652C();
        //DrawSync(SyncMode_Wait);
    }
}
