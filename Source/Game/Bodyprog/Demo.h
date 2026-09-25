#pragma once

namespace Silent::Game
{
    /** `Demo_FrameCount` */
    extern s32 g_Demo_FrameCount;

    // TODO: Make local. Used in `Demo_Update`.
    extern char D_8002B2D8[]; // "STEP ERROR:[H:"
    extern char D_8002B2E8[]; // "]/[M:"
    extern char D_8002B2F0[]; // "]"

    extern s32 g_Demo_DemoId;

    extern u16 g_Demo_RandSeed;

    extern s32 D_800AFDF0;

    extern e_FsFile g_Demo_DemoFileIdx;

    extern e_FsFile g_Demo_PlayFileIdx;

    extern s_OptionsConfig g_Demo_UserConfigBackup;

    extern u32 g_Demo_PrevRandSeed;

    extern u32 g_Demo_RandSeedBackup;

    extern s32 g_Demo_DemoStep;

    extern s32 g_Demo_VideoPresentInterval;

    /** Something for demo handling. Set in `Fs_QueueChunksLoad`. */
    extern bool g_Demo_IsLoadingChunks;

    bool Demo_SequenceAdvance(s32 incrementAmount);

    void Demo_DemoDataRead();

    void Demo_PlayDataRead();

    s32 Demo_PlayFileBufferSetup();

    void Demo_DemoFileSavegameUpdate();

    void Demo_GameGlobalsUpdate();

    void Demo_GameGlobalsRestore();

    void Demo_GameRandSeedUpdate();

    void Demo_GameRandSeedRestore();

    void Demo_Start();

    void Demo_Stop();

    bool func_8008F434(s32 arg0);

    /** @brief Gets the active demo state.
     *
     * @param gameState Active game state (`e_GameState`).
     * @return Active demo state `(e_DemoState`).
     */
    s32 Demo_StateGet(s32 gameState);

    void Demo_ExitDemo();

    bool func_8008F520();

    /** Generates the backup random demo seed and stores it in `Demo_RandSeedBackup`. */
    void Demo_DemoRandSeedBackup();

    void Demo_DemoRandSeedRestore();

    void Demo_DemoRandSeedAdvance();

    bool Demo_Update();

    bool Demo_ControllerDataUpdate();

    bool Demo_PresentIntervalUpdate();

    bool Demo_GameRandSeedSet();

    bool func_8008F914(q19_12 posX, q19_12 posZ);
}
