#pragma once

namespace Silent::Game
{
    #define SAVEGAME_ENTRY_BUFFER_0 ((u8*)0x801E09E0) // Slot 1 savegame entry.
    #define SAVEGAME_ENTRY_BUFFER_1 ((u8*)0x801E1430) // Slot 2 savegame entry.

    constexpr int MEMCARD_DEVICE_COUNT_MAX = 8;
    constexpr int MEMCARD_SAVES_COUNT_MAX  = 11;
    constexpr int MEMCARD_FILE_COUNT_MAX   = 15;
    constexpr int MEMCARD_SLOT_COUNT_MAX   = 2;

    constexpr int SAVEGAME_FOOTER_MAGIC = 0xDCDC;
    constexpr int SAVEGAME_COUNT_MAX    = MEMCARD_SAVES_COUNT_MAX * MEMCARD_FILE_COUNT_MAX;

    #define MemCard_ActiveMemCardSlotGet(slotIdx) \
        ((s_SaveScreenElement*)&SAVEGAME_ENTRY_BUFFER_0[2640 * (slotIdx)])

    #define MemCard_StatusGet(status, deviceIdx) \
        status >> (deviceIdx * 3) & 0x7

    #define MemCard_StatusStore(status, deviceIdx) \
        status << (deviceIdx * 3)

    #define MemCard_FileStatusGet(status, deviceIdx) \
        status >> (deviceIdx * 2) & 0x3

    #define MemCard_FileStatusStore(status, deviceIdx) \
        status << (deviceIdx * 2)

    /** @brief Used by `s_SaveScreenElement`. */
    typedef enum _SavegameEntryType
    {
        SavegameEntryType_NoMemCard          = 0,
        SavegameEntryType_UnformattedMemCard = 1,
        SavegameEntryType_CorruptedMemCard   = 2,
        SavegameEntryType_LoadMemCard        = 3,
        SavegameEntryType_OutOfBlocks        = 4,
        SavegameEntryType_NoDataInMemCard    = 5,
        SavegameEntryType_Unk6               = 6,
        SavegameEntryType_CorruptedSave      = 7,
        SavegameEntryType_Save               = 8,
        SavegameEntryType_NewSave            = 9,
        SavegameEntryType_NewFile            = 10
    } e_SavegameEntryType;

    /** @brief Save location IDs. */
    typedef enum _SaveLocationId
    {
        SaveLocationId_Anywhere    = 0,
        SaveLocationId_Cafe        = 1,
        SaveLocationId_Bus         = 2,
        SaveLocationId_Store       = 3,
        SaveLocationId_Infirmary   = 4,
        SaveLocationId_Doghouse    = 5,
        SaveLocationId_Gordon      = 6,
        SaveLocationId_Church      = 7,
        SaveLocationId_Garage      = 8,
        SaveLocationId_Police      = 9,
        SaveLocationId_Reception   = 10,
        SaveLocationId_Room302     = 11,
        SaveLocationId_Directors   = 12,
        SaveLocationId_JewelryShop = 13,
        SaveLocationId_PoolHall    = 14,
        SaveLocationId_AntiqueShop = 15,
        SaveLocationId_ThemePark   = 16,
        SaveLocationId_Boat        = 17,
        SaveLocationId_Bridge      = 18,
        SaveLocationId_Motel       = 19,
        SaveLocationId_Lighthouse  = 20,
        SaveLocationId_Sewer       = 21,
        SaveLocationId_Nowhere     = 22,
        SaveLocationId_ChildsRoom  = 23,
        SaveLocationId_NextFear    = 24
    } e_SaveLocationId;

    typedef enum _MemCardProcess
    {
        MemCardProcess_None          = 0,
        MemCardProcess_Init          = 1,
        MemCardProcess_Load_Game     = 2,
        MemCardProcess_Save_3        = 3,
        MemCardProcess_Load_Settings = 4,
        MemCardProcess_Save_5        = 5,
        MemCardProcess_Format        = 6
    } e_MemCardProcess;

    /** @brief Memory card states */
    typedef enum _MemCardState
    {
        MemCardState_Null        = 0, /** Null state. */
        MemCardState_Unavailable = 1, /** Not connected. */
        MemCardState_Loading     = 2, /** Loading the memory card. */
        MemCardState_Available   = 3,
        MemCardState_Format      = 4, /** Format required. */
        MemCardState_Broken      = 5
    } e_MemCardState;

    typedef enum _FileState
    {
        FileState_Unused  = 0,
        FileState_Used    = 1,
        FileState_Unk2    = 2, /** @unused See `func_80033548`. */
        FileState_Damaged = 3
    } e_FileState;

    /** @brief Memory card process states. */
    typedef enum _MemCardCardState
    {
        MemCardWorkState_Idle          = 0,
        MemCardWorkState_Init          = 1,
        MemCardWorkState_Check         = 2,
        MemCardWorkState_Load          = 3,
        MemCardWorkState_DirRead       = 4,
        MemCardWorkState_FileCreate    = 5,
        MemCardWorkState_FileOpen      = 6,
        MemCardWorkState_FileReadWrite = 7
    } e_MemCardCardState;

    typedef enum _MemCardIoMode
    {
        MemCardIoMode_Init    = 0,
        MemCardIoMode_DirRead = 1, // TODO: Not sure if this is actual purpose yet.
        MemCardIoMode_Read    = 2,
        MemCardIoMode_Write   = 3,
        MemCardIoMode_Create  = 4
    } e_MemCardIoMode;

    typedef enum _MemCardResult
    {
        MemCardResult_NotConnected    = 0,   /** "Card not connected". */
        MemCardResult_Success         = 1,   /** Default code returned when no errors occur. */
        MemCardResult_InitError       = 2,   /** `MemCard_State_Init` `EvSpNEW` "No writing after connection". */
        MemCardResult_InitComplete    = 3,   /** `MemCard_State_Init` `EvSpIOE` "Connected". */
        MemCardResult_LoadError       = 4,   /** `MemCard_State_Load` `EvSpNEW` "Uninitialized card". */
        MemCardResult_NewDevice       = 5,   /** `MemCard_State_DirRead` when `g_MemCard_Work.hasNewDevice`. */
        MemCardResult_NoNewDevice     = 6,   /** `MemCard_State_DirRead` when `!g_MemCard_Work.hasNewDevice`. */
        MemCardResult_FileCreateError = 7,   /** `MemCard_State_FileCreate` after 15 retries. */
        MemCardResult_FileOpenError   = 8,   /** `MemCard_State_FileOpen` after 15 retries. */
        MemCardResult_FileSeekError   = 9,   /** `MemCard_State_FileReadWrite` after 15 retries. */
        MemCardResult_FileIoError     = 10,  /** `MemCard_State_FileReadWrite` after 15 retries. */
        MemCardResult_FileIoComplete  = 11,  /** `MemCard_State_FileReadWrite` `EvSpIOE` "Completed". */
        MemCardResult_Full            = 100, /** Used outside main memcard code. */
        MemCardResult_DamagedData     = 101  /** Used outside main memcard code. */
    } e_MemCardResult;

    /** @note OPM16 build contains some strings related to this split, indicating the names of 6 structs:
     * `MCM_FUNC_WORK`
     * `MC_FILE`
     * `SCE_HEADER`
     * `MC_HEADER`
     * `MC_CONFIG`
     * `MC_PROGRESS`
     */

    // https://github.com/Sparagas/Silent-Hill/blob/1f24eb097a4b99129bc7c9793d23c82244848a27/010%20Editor%20-%20Binary%20Templates/ps1_memory_card.bt#L122C8-L122C17
    typedef struct
    {
        char magic[2];
        u8   iconDisplayFlag;
        u8   blockCount;
        u16  titleNameShiftJis[32];
        s8   field_44[28];       // Reserved/unused?
        s8   iconPalette[32]; // CLUT data copied from `TIM_IMAGE.caddr`.
        s8   textureData[32]; // Copied from `TIM_IMAGE.paddr`.
        s8   unk_A0[352];
    } s_PsxSaveBlock;

    typedef struct
    {
        char filenames[MEMCARD_FILE_COUNT_MAX][21];
        u8   blockCounts[MEMCARD_FILE_COUNT_MAX]; // Size of each file in 8192 byte blocks.
    } s_MemCard_Directory;

    typedef struct
    {
        s32 devicesPending; /** Bitfield of device IDs, each set bit index is an ID that must be read/initialized first. */
        s32 state;          /** `e_CardState` */
        s32 stateStep;
        s32 stateResult;    /** `e_MemCardResult` */
        s32 MemCardIoMode;  /** `e_MemCardIoMode` */
        s32 deviceId;

        s_MemCard_Directory* directories; /** Array of files on the card, pointer supplied by caller to `MemCard_WorkSet`. */

        char  filePath[28];
        s32   createBlockCount; /** Block count passed to `open` when creating new file. */
        s32   seekOffset;
        void* dataBuffer;
        s32   dataSize;
        bool  hasNewDevice;
        s32   fileHandle;
        s32   retryCount;
    } s_MemCard_Work;

    struct s_MemCard_SaveMetadata
    {
        s32 totalSavegameCount;
        u32 gameplayTimer;
        u16 savegameCount;
        u8  locationId;
        u8  isNextFearMode           : 1;
        u8  add290Hours              : 2;
        u8  pickedUpSpecialItemCount : 5; /** See `pickedUpSpecialItemCount` comment in `s_Savegame`. */
    };

    /** @brief Information of elements in the save screen.
     *
     * @note Used to determine both saves and other
     * elements in save screen as it is use to display
     * the "New Save" or "The file is damage" elements.
     * It is also used to display status messages in rectangles,
     * for example when no memory card is inserted or no save is
     * available.
     */
    struct s_SaveScreenElement
    {
        s16                     totalSavegameCount; /** Counter for all savegame instances created throughout the game.
                                                    * The value is derived by running through all savegames on a memory card
                                                    * and picking the one with the largest value.
                                                    *
                                                    * @bug This counter is used to determine if the "New Save" element has
                                                    * been selected, which causes overwrites to not show the "Yes/No" message.
                                                    */
        s16                     savegameCount;
        s8                      type;               /** `e_SavegameEntryType` */
        s8                      deviceId;
        s8                      fileIdx;
        s8                      elementIdx;
        s8                      locationId;
        s_MemCard_SaveMetadata* saveMetadata;
    };

    /** @brief Appended to `ShSavegame` and `ShSaveUserConfig` during agame save. Contains 8-bit XOR checksum + magic.
     * Checksum generated via `MemCard_ChecksumGenerate`.
     */
    typedef struct _SavegameFooter
    {
        u8  checksum_0[2];
        u16 magic_2;
    } s_Savegame_Footer;

    /** @brief Contains `s_Savegame` data with the footer appended to the end containing the checksum + magic. */
    typedef struct _SavegameContainer
    {
        s_Savegame        savegame;
        s_Savegame_Footer footer;
    } s_Savegame_Container;

    /** @brief Contains `s_OptionsConfig` data padded to 128 bytes with a footer at the end containing checksum + magic. */
    typedef struct _SaveUserConfigContainer
    {
        s_OptionsConfig   config;
        s_Savegame_Footer footer;
    } s_Savegame_UserConfigs;

    typedef struct
    {
        s32 totalSavegameCount;
        s32 fileIdx;
        s32 saveIdx;
    } s_MemCard_TotalSavesInfo;

    typedef struct
    {
        s_MemCard_SaveMetadata saveMetadata[MEMCARD_SAVES_COUNT_MAX];
        s_Savegame_Footer      footer;
    } s_MemCard_SaveHeader;

    typedef struct
    {
        s32                   status; /** `e_MemCardState` */
        s8                    fileState[MEMCARD_FILE_COUNT_MAX];
        s_MemCard_SaveHeader* saveHeader; /** Slots saves information. */
        s32                   fileLimit;  /** Max count of files allowed in the memory card. */
    } s_MemCard_DeviceInfo;

    /** @note Some memory card states information. Related to `s_SaveScreenElement`.
     * Very likely used only for process related to memory cards.
     */
    typedef struct
    {
        s32 processId_0;          /** e_MemCardProcess. */
        s32 deviceId_4;
        s32 fileIdx_8;
        s32 saveIdx_C;            /** Index of the save in a determined file. */
        s32 processState_10;      /** States related to specific memory card events. */
        s32 lastMemCardResult_14; /** `e_MemCardResult` */
    } s_MemCard_Process;

    // OPM16 has `MCM_FUNC_WORK` struct with size 0x6D8, close to this 0x718.
    typedef struct
    {
        s_MemCard_DeviceInfo   devices[MEMCARD_DEVICE_COUNT_MAX];
        s_MemCard_Process      saveWork[2]; // This seems to be used mainly for processes. Element 0 is used for general processes while 1 is exclusively used for
        s32                    memCardInitalized;
        s_PsxSaveBlock         saveBlock;
        s_MemCard_SaveHeader   saveInfo;
        s_Savegame_UserConfigs userConfig;
        s_Savegame_Container   saveGame;
    } s_MemCard_SaveWork;

    // ========
    // GLOBALS
    // ========

    /** @brief Basic information required to draw information of elements in save slots.
     * Address access is based on the slot: slot 1 = 0x801E09E0, slot 2 = 0x801E1440.
     *
     * @note Macros for its references are in `saveload.h`:
     * `SAVEGAME_ENTRY_BUFFER_0`
     * `SAVEGAME_ENTRY_BUFFER_1`
     */

    extern u8 g_SlotElementSelectedIdx[2]; // 0 - Slot 1, 1 - Slot 2.

    extern s8 g_SelectedSaveSlotIdx; // 0 - Slot 1, 1 - Slot 2.

    extern s_FsImageDesc D_800A8D98;

    extern u8 D_800A97D7;

    extern bool g_SaveScreen_IsInSaveScreen;

    extern s32 D_800A97DC; // `e_SavegameEntryType`

    extern s8 D_800A97E0;

    extern u32 allFileStatus[];

    extern s8 D_800BCD39; // Boolean.

    /** @brief Defines if the game can use the memory card. */
    extern bool g_MemCard_AvailibityStatus;

    extern s_MemCard_Work g_MemCard_Work; // 0x800B5488

    extern s_MemCard_SaveWork g_MemCard_SaveWork;

    extern s32 g_MemCard_PrevSavegameCount;

    extern s16 g_MemCard_SavegameCount;

    extern s_SaveScreenElement* g_MemCard_ActiveMemCardSlotSaves;

    /** @brief Amount of elements in each memory card. */
    extern u8 g_Savegame_ElementCount0[MEMCARD_SLOT_COUNT_MAX];

    /** @brief Stores all memory cards/devices status. */
    extern u32 g_MemCard_AllMemCardsStatus;

    /** @brief Some determinator for the state of the save screen.
     * 2 - Saving, 3 - Loading.
     */
    extern s8 g_SaveScreen_SaveScreenState;

    /** @brief Counts all elements of all inserted memory cards. */
    extern s16 g_MemCard_TotalElementsCount;

    /** @brief Amount of elements in each memory card. */
    extern u8 g_Savegame_ElementCount1[MEMCARD_SLOT_COUNT_MAX];

    /** @brief Index of selected element in each memory card. */
    extern u8 g_Savegame_SelectedElementIdx;

    extern s8 g_SelectedFileIdx;

    extern s8 g_SelectedDeviceId;

    /** @brief Initializes memory card system.
     * This Initializes the entire memory card handling system while
     * `MemCard_Init` exclusively initalizes the memory cards.
     */
    void MemCard_SysInit();

    /** @brief Clear memory card data in RAM. */
    void MemCard_RamClear(s32 deviceId);

    /** @brief Clear all files status. */
    void MemCard_FileStatusClear(s32 deviceId);

    bool MemCard_AreAllFilesUsed(s32 deviceId);

    void MemCard_SysEnable();

    /** @brief Disables memory card. */
    void MemCard_SysDisable();

    void MemCard_InitStatus();

    void MemCard_StatusInitNotConnected();

    s32 MemCard_AllMemCardsStatusGet();

    void func_8002E8D4();

    void MemCard_StatusInitSuccess();

    s32 func_8002E914();

    bool MemCard_ProcessSet(s32 arg0, s32 deviceId, s32 fileIdx, s32 saveIdx);

    /** @brief Related to formatting logic.
     * Used in: `SAVELOAD.BIN`
     */
    s32 MemCard_LastMemCardResultGet();

    s32 MemCard_AllFilesStatusGet(s32 deviceId);

    s_MemCard_SaveMetadata* MemCard_SaveMetadataGet(s32 deviceId, s32 fileIdx, s32 saveIdx);

    /** @brief Returns the count of files used in the memory card. */
    s32 MemCard_UsedFileCount(s32 deviceId);

    /** @brief Returns the count available files in the memory card. */
    s32 MemCard_FreeFilesCount(s32 deviceId);

    /** @unused Checks if no save have been done in any inserted memory card. */
    bool MemCard_NoSavesDoneCheck(s32* outDeviceId, s32* outFileIdx, s32* outSaveIdx);

    void MemCard_Update(); // Return type assumed.

    void MemCard_Process_Format(s_MemCard_Process* statusPtr);

    void MemCard_Process_Init(s_MemCard_Process* statusPtr);

    // NOT SURE
    s32 MemCard_FileLimitUpdate(s32 deviceId, s_MemCard_Directory* dir);

    void MemCard_Process_Load(s_MemCard_Process* statusPtr);

    void MemCard_Process_Save(s_MemCard_Process* statusPtr);

    void MemCard_SaveInfoClear(s_MemCard_SaveHeader* saveInfo);

    /** Copies user config into an `s_Savegame_UserConfigs` and calculates footer checksum. */
    void MemCard_UserConfigCopy(s_Savegame_UserConfigs* dest, s_OptionsConfig* src);

    s32 MemCard_BiggestTotalSavegameCountGet(s32 deviceId);

    /** Copies savegame into an s_Savegame_Container and calculates footer checksum. */
    void MemCard_GameDataCopy(s_Savegame_Container* dest, s_Savegame* src);

    void MemCard_TotalSavegameCountUpdate(s32 deviceId, s32 fileIdx, s32 saveIdx, s_Savegame* arg3);

    void MemCard_TotalSavegameCountStepUpdate(s32 deviceId, s32 fileIdx, s32 saveIdx);

    void MemCard_SaveWithBiggestTotalSavegameCountGet(s32 deviceId, s_MemCard_TotalSavesInfo* result);

    /** Updates the footer with the checksum of the given data. */
    void MemCard_ChecksumUpdate(s_Savegame_Footer* saveFooter, s8* saveData, s32 saveDataLength);

    /** Generates a checksum of the given saveData and compares it against the checksum value in the footer.
     * Returns 1 if the checksums match, otherwise 0.
     */
    bool MemCard_ChecksumValidate(s_Savegame_Footer* saveFooter, s8* saveData, s32 saveDataLength); // 0x8002FF74

    /** Generates an 8-bit XOR checksum over the given data, only appears used with s_Savegame data. */
    u8 MemCard_ChecksumGenerate(s8* saveData, s32 saveDataLength);

    /** Generates a save filename for the given save index. */
    void MemCard_FilenameGenerate(char* dest, s32 fileIdx);

    void MemCard_SaveBlockInit(s_PsxSaveBlock* saveBlock, s8 blockCount, s32 saveIdx, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);

    /** Unused function? Appears to write `0xFF` to first 128 bytes of card and check if event is triggered. */
    s32 MemCard_DeviceTest(s32 deviceId);

    s32 MemCard_DeviceFormat(s32 deviceId);

    s32 MemCard_FileClear(s32 deviceId, char* fileName);

    s32 MemCard_FileRename(s32 deviceId, char* prevName, char* newName);

    void MemCard_Init();

    void MemCard_EventsInit();

    void MemCard_StateInit();

    void MemCard_SwEventsInit();

    void MemCard_HwEventsInit();

    void MemCard_EventsClose();

    void MemCard_SwEventsClose();

    void MemCard_HwEventsClose();

    s32 MemCard_SwEventsTest();

    void MemCard_SwEventsReset();

    s32 MemCard_HwEventsTest();

    void MemCard_HwEventsReset();

    void MemCard_HwEventSpIOE();

    void MemCard_HwEventSpERROR();

    void MemCard_HwEventSpNEW();

    void MemCard_HwEventSpTIMOUT();

    void MemCard_HwEventSpUNKNOWN();

    s32 MemCard_StateResult(); /** `e_MemCardResult` */

    bool MemCard_WorkSet(e_MemCardIoMode mode, s32 deviceId, s_MemCard_Directory* outDir, char* filename, s32 createBlockCount, s32 fileOffset, void* outBuf, s32 bufSize);

    bool MemCard_MemCardIsIdle();

    void MemCard_StateUpdate();

    s32 MemCard_State_Init();

    s32 MemCard_State_Check();

    s32 MemCard_State_Load();

    s32 MemCard_State_DirRead();

    s32 MemCard_State_FileCreate();

    s32 MemCard_State_FileOpen();

    s32 MemCard_State_FileReadWrite();

    void MemCard_DevicePathGenerate(s32 deviceId, char* res);

    /** @brief Checks if any file is corrupted, unused or used.
     * If any file is used then it return false.
     */
    bool MemCard_FilesAreNotUsedCheck(s32 idx);
}
