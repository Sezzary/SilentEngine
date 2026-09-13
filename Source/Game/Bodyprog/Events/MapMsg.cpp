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

    static s_MapMsgSelect g_MapMsg_Select;
    static int            g_MapMsg_SelectCancelIdx;

    /** @brief Draws map message strings in screen space.
     *
     * @param mapMsg Map message to draw.
     * @param displayLength Number of consecutive glyphs to draw from the map message.
     * @param pageIdx Message page index.
     * @return Message return result.
     */
    static MsgReturnResult Gfx_MapMsg_StringDraw(const std::string& mapMsg, int displayLength, int pageIdx)
    {
        int styleFlags = (int)TextStyleFlags::Gradient |
                         (int)TextStyleFlags::Shadow   |
                         g_SysWork.enableHalfHeightGlyphs ? (int)TextStyleFlags::HalfHeight : (int)TextStyleFlags::None;
        return DrawParsedMsg(g_MapMsg_Cached, ConvertRetroScreenPixelsToPercent(g_StringPosition), SERIF_FONT_SCALE,
                             styleFlags, displayLength, pageIdx);
    }

    int Gfx_MapMsg_Draw(int mapMsgIdx)
    {
        constexpr int ROLLOUT_ADVANCE = 2;

        static int          rolloutState;  // } @todo Confusing, rework completely.
        static MsgSelectArg menuSelection; // }
        static int          displayLength;
        static int          activeMapMsgIdx;
        static int          displayLengthInc;
        static bool         loadAudio;

        const auto& input   = g_App.GetInput();
        const auto& options = g_App.GetOptions();

        // Check for user input to skip.
        bool skip = false;
        if (input.GetAction(In::Enter).IsClicked() ||
            input.GetAction(In::Cancel).IsClicked())
        {
            skip = true;
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
                Gfx_StringPositionSet(SCREEN_WIDTH / 8, (SCREEN_HEIGHT / 3) * 2);

                displayLength += displayLengthInc;
                displayLength  = CLAMP(displayLength, 0, MAP_MESSAGE_DISPLAY_ALL_LENGTH);

                if (g_MapMsg_AudioType != MapMsgAudioType_None && g_SysWork.mapMsgTimer > Q12(0.0f))
                {
                    g_SysWork.mapMsgTimer -= g_DeltaTimeRaw;
                    g_SysWork.mapMsgTimer  = CLAMP(g_SysWork.mapMsgTimer, Q12(0.0f), INT_MAX);
                }

                if (rolloutState == NO_VALUE)
                {
                    if (g_MapMsg_AudioType == MapMsgAudioType_None)
                    {
                        //Game_TimerUpdate();
                    }

                    bool isAudioUnskippable = g_MapMsg_AudioType == MapMsgAudioType_VoiceClipUnskippable ||
                                              g_MapMsg_AudioType == MapMsgAudioType_VoiceStream;

                    if (menuSelection == MsgSelectArg::None && rolloutState == NO_VALUE)
                    {
                        if (g_MapMsg_Select.maxIdx == (int)menuSelection)
                        {
                            if (!(isAudioUnskippable || !skip) ||
                                (g_MapMsg_AudioType != MapMsgAudioType_None && g_SysWork.mapMsgTimer == Q12(0.0f)))
                            {
                                menuSelection = MsgSelectArg::None;

                                if (g_SysWork.bgmStatusFlags & BgmStatusFlag_VoiceDialog)
                                {
                                    SD_Call(19);
                                }
                                break;
                            }
                        }
                        // Cancel selection.
                        else if (input.GetAction(In::Cancel).IsClicked())
                        {
                            g_MapMsg_Select.maxIdx           = (int)menuSelection;
                            g_MapMsg_Select.selectedEntryIdx = g_MapMsg_SelectCancelIdx;

                            //Sd_SfxPlay(Sfx_MenuCancel, 0, Q8(0.25f));

                            if (g_SysWork.silentYesSelection)
                            {
                                g_SysWork.silentYesSelection = false;
                            }

                            menuSelection = MsgSelectArg::None;
                            break;
                        }
                        // Enter selection.
                        else if (input.GetAction(In::Enter).IsClicked())
                        {
                            g_MapMsg_Select.maxIdx = (int)menuSelection;

                            if (g_MapMsg_Select.selectedEntryIdx == g_MapMsg_SelectCancelIdx)
                            {
                                //Sd_SfxPlay(Sfx_MenuCancel, 0, Q8(0.25f));
                            }
                            else if (!g_SysWork.silentYesSelection)
                            {
                                //Sd_SfxPlay(Sfx_MenuConfirm, 0, Q8(0.25f));
                            }

                            if (g_SysWork.silentYesSelection)
                            {
                                g_SysWork.silentYesSelection = false;
                            }

                            menuSelection = MsgSelectArg::None;
                            break;
                        }
                    }
                    else if ((!isAudioUnskippable && skip && g_MapMsg_Select.maxIdx != 0) ||
                            (g_MapMsg_AudioType != MapMsgAudioType_None && g_SysWork.mapMsgTimer == Q12(0.0f)))
                    {
                        if (g_MapMsg_Select.maxIdx != NO_VALUE)
                        {
                            g_MapMsg_Select.maxIdx = NO_VALUE;
                            menuSelection          = MsgSelectArg::None;
                            break;
                        }

                        g_MapMsg_CurrentIdx++;
                        g_SysWork.mapMsgTimer = g_MapMsg_Select.maxIdx;

                        displayLength = 0;
                        rolloutState  = 0;

                        if (g_MapMsg_AudioType == MapMsgAudioType_VoiceStream)
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
                    if (skip)
                    {
                        displayLength = MAP_MESSAGE_DISPLAY_ALL_LENGTH;
                    }
                }

                rolloutState = 0;
                menuSelection = Gfx_MapMsg_SelectionUpdate(g_MapMsg_CurrentIdx, &displayLength);

                if (menuSelection == MsgSelectArg::Select2 || menuSelection == MsgSelectArg::Select3)
                {
                    rolloutState = NO_VALUE;
                }
            }
            case false:
            {
                g_SysWork.mapMsgTimer            = NO_VALUE;
                g_MapMsg_Select.maxIdx           = NO_VALUE;
                g_MapMsg_Select.selectedEntryIdx = 0;
                g_MapMsg_AudioType               = MapMsgAudioType_None;
                g_MapMsg_CurrentIdx              = mapMsgIdx;
                rolloutState                     = 0;
                menuSelection                    = MsgSelectArg::None;
                activeMapMsgIdx                  = mapMsgIdx;
                displayLength                    = 0;
                displayLengthInc                 = ROLLOUT_ADVANCE;

                Gfx_MapMsg_Reset();

                loadAudio                = true;
                g_SysWork.isMgsStringSet = true;
                return MapMsgState_Finish;
            }
        }

        if (menuSelection != MsgSelectArg::None)
        {
            return MapMsgState_Idle;
        }

        g_SysWork.isMgsStringSet = false;
        displayLength            = 0;

        if (g_SysWork.bgmStatusFlags & BgmStatusFlag_VoiceDialog)
        {
            loadAudio = true;
        }

        return g_MapMsg_Select.selectedEntryIdx + 1;
    }

    MsgSelectArg Gfx_MapMsg_SelectionUpdate(int mapMsgIdx, int* displayLength)
    {
        constexpr int STRING_LINE_OFFSET = 16;

        const auto& input = g_App.GetInput();

        auto result = Gfx_MapMsg_StringDraw(g_MapOverlayHdr.mapMessages[mapMsgIdx], *displayLength,
                                            g_MapMsg_CurrentPageIdx);

        g_MapMsg_SelectFlashTimer += g_DeltaTimeRaw;
        if (g_MapMsg_SelectFlashTimer >= Q12(0.5f))
        {
            g_MapMsg_SelectFlashTimer -= Q12(0.5f);
        }

        // Handle return code.
        switch (result.Code)
        {
            case MsgReturnCode::None:
                g_MapMsg_SelectFlashTimer = Q12(0.0f);
                break;

            case MsgReturnCode::EndPage:
                g_MapMsg_CurrentPageIdx++;
                break;

            case MsgReturnCode::End:
                g_MapMsg_CurrentPageIdx = 0;
                break;
        }

        // Handle selection prompt.
        if (result.Select != MsgSelectArg::None)
        {
            g_MapMsg_Select.maxIdx   = 1;
            g_MapMsg_SelectCancelIdx = (int)result.Select - 1;

            int entryCount = 0;

            // Yes/no selection prompt.
            if (result.Select == MsgSelectArg::YesOrNo)
            {
                // Shows selection prompt with map messages at indices 0 and 1.
                // @note All maps have "Yes" and "No" as messages 0 and 1, respectively.
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

                    Gfx_StringPositionSet(SCREEN_WIDTH / 10, (STRING_LINE_OFFSET * i) + 98);
                    Gfx_StringDraw(g_MapOverlayHdr.mapMessages[i]);
                }

                entryCount = 2;
            }
            // 2-3 option selection prompt.
            else
            {
                // Shows selection prompt with 2 or 3 map messages from current index + 1/2/3.
                // Requires prompt options to be arranged sequentially in the map message array with the last option
                // being to cancel, e.g.
                // `[idx]`:     "Select one of 3 options.{S3}"
                // `[idx + 1]`: "Option 1"
                // `[idx + 2]`: "Option 2"
                // `[idx + 3]`: "Cancel"
                entryCount = (int)result.Select;
                for (int i = 0; i < entryCount; i++)
                {
                    if (g_MapMsg_Select.selectedEntryIdx == i)
                    {
                        Gfx_StringColorSet((e_StringColorId)(((g_MapMsg_SelectFlashTimer >> 10) * 3) + 4));
                    }
                    else
                    {
                        Gfx_StringColorSet(StringColorId_White);
                    }

                    Gfx_StringPositionSet(SCREEN_WIDTH / 10, (STRING_LINE_OFFSET * i) + 98);
                    Gfx_StringDraw(g_MapOverlayHdr.mapMessages[(mapMsgIdx + i) + 1]);
                }
            }

            // Move cursor.
            if (input.GetAction(In::Up).IsClicked(0.5f) &&
                g_MapMsg_Select.selectedEntryIdx != 0)
            {
                g_MapMsg_SelectFlashTimer = Q12(0.0f);
                g_MapMsg_Select.selectedEntryIdx--;

                //Sd_SfxPlay(Sfx_MenuMove, 0, Q8(0.25f));
            }
            if (input.GetAction(In::Down).IsClicked(0.5f) &&
                g_MapMsg_Select.selectedEntryIdx != (entryCount - 1))
            {
                g_MapMsg_SelectFlashTimer = Q12(0.0f);
                g_MapMsg_Select.selectedEntryIdx++;

                //Sd_SfxPlay(Sfx_MenuMove, 0, Q8(0.25f));
            }

            result.Select = MsgSelectArg::None;
        }

        if (result.DisplayAll)
        {
            *displayLength = MAP_MESSAGE_DISPLAY_ALL_LENGTH;
        }

        return result.Select;
    }

    void func_80037124() // 0x80037124
    {
        g_MapMsg_Select.maxIdx = NO_VALUE;
        //func_8003652C();
        //DrawSync(SyncMode_Wait);
    }
}
