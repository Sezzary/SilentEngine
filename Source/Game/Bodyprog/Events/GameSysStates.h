#pragma once

namespace Silent::Game
{
    struct s_EventData;
    struct s_MapPoint2d;
    struct s_RadioNpcInfo;

    extern s_EventData* g_ItemTriggerEvents[];

    extern s_RadioNpcInfo g_RadioNpcInfos[2];

    extern s_MapPoint2d D_800BCDB0;

    /** Related to special item interactions. */
    extern s32 g_ItemTriggerItemIds[5];

    extern u8 g_MapAreaLoadCounter;

    extern s_EventData* g_MapEventData;

    void GameState_InGame_Update();

    void SysState_Gameplay_Update();

    void SysState_GamePaused_Update();

    void SysState_OptionsMenu_Update();

    void func_8003943C();

    void SysState_StatusMenu_Update();

    void GameState_LoadStatusScreen_Update();

    void SysState_MapScreen_Update();

    void GameState_LoadMapScreen_Update();

    void SysState_Fmv_Update();

    void SysState_LoadArea_Update();

    void AreaLoad_UpdatePlayerPosition();

    /** @brief Plays door sound when transitioning areas.
     * Reduntant code. `SysState_LoadArea_Update` also performs this same code
     * in the exact way this function is doing it.
     */
    void AreaLoad_TransitionSound();

    s8 AreaLoad_TransitionFlags();

    void SysState_ReadMessage_Update();

    /** @brief Updates the savegame buffer with the current player SysWork info (position, rotation, health, event index). */
    void SysWork_SavegameUpdatePlayer();

    void func_8003A16C();

    /** @brief Updates SysWork with player info from the savegame buffer (position, rotation, health). */
    void SysWork_SavegameReadPlayer();

    void SysState_SaveMenu_Update();

    void SysState_SaveMenu_Update();

    void SysState_EventCallback_Update();

    void SysState_EventSetFlag_Update();

    void SysState_EventPlaySound_Update();

    void SysState_GameOver_Update();

    void GameState_MapEvent_Update();
}
