#pragma once

#include "Game/Main/FileInfo.h"

namespace Silent::Game
{
    #define FS_BUFFER_0      (void*)0x8010A600
    #define FS_BUFFER_12     (void*)0x801201B4 // Used for weapon anim.     } Sub-buffers within the 4096-byte buffers?
    #define FS_BUFFER_4      (void*)0x80124384 // Used for player map anim. }
    #define FS_BUFFER_11     (void*)0x80169600 // } Used for DMS cutscene data.
    #define FS_BUFFER_17     (void*)0x80169E00 // }
    #define FS_BUFFER_13     (void*)0x8016AE00 // }
    #define FS_BUFFER_14     (void*)0x800F9600 // }
    #define FS_BUFFER_15     (void*)0x801F3600 // }
    #define FS_BUFFER_16     (void*)0x801EBE00 // }
    #define FS_BUFFER_18     (void*)0x80180600 // }
    #define FS_BUFFER_19     (void*)0x801A1E00 // }
    #define FS_BUFFER_20     (void*)0x80185600 // }
    #define GLOBAL_LM_BUFFER (s_LmHeader*)0x8016B600
    #define IPD_BUFFER       (s_IpdHeader*)0x80175600 // Used by map.
    #define LM_BUFFER_2      (s_LmHeader*)0x8019E600
    #define FS_BUFFER_3      (void*)0x801B2600
    #define FS_BUFFER_8      (void*)0x801B5E80 // Used for loading inventory item models.
    #define FS_BUFFER_7      (void*)0x801C2600 // Used in `b_konami.c`.
    #define FS_BUFFER_2      (void*)0x801CF600
    #define FS_BUFFER_1      (void*)0x801E2600
    #define FS_BUFFER_5      (void*)0x801E3600 // Used in `b_konami.c`.
    #define FS_BUFFER_6      (void*)0x801E4600 // Used in `b_konami.c`.
    #define FS_BUFFER_21     (void*)0x801E5600
    #define FS_BUFFER_10     (void*)0x801EA600
    #define FS_BUFFER_9      (void*)0x801EC600
    #define FS_BUFFER_22     (u_int*)0x801A0200
    #define FS_BUFFER_23     (u_int*)0x801A2200
    #define FS_BUFFER_24     (void*)0x801A0600
    #define FS_BUFFER_25     (void*)0x80167600
    #define FS_BUFFER_26     (void*)0x8018C600
    #define FS_BUFFER_27     (void*)0x80196E00
    #define FS_BUFFER_28     (void*)0x801D6E00
    #define FS_BUFFER_29     (void*)0x80142A00
    #define FS_BUFFER_30     (void*)0x80156A00
    #define FS_BUFFER_31     (void*)0x80102E00
    #define FS_BUFFER_32     (void*)0x80106E00
    #define FS_BUFFER_33     (void*)0x80153A00
    #define FS_BUFFER_34     (void*)0x80163200
    #define FS_BUFFER_35     (void*)0x801950F0

    #define IMAGE_BUFFER   (u_long*)0x801AFA00
    #define IMAGE_BUFFER_0 (u_long*)0x801CFA00
    #define IMAGE_BUFFER_1 (u_long*)0x801C8200
    #define IMAGE_BUFFER_2 (u_long*)0x801ABE00
    #define IMAGE_BUFFER_3 (u_long*)0x801EB600
    #define IMAGE_BUFFER_4 (u_long*)0x801DCE00
    #define IMAGE_BUFFER_5 (u_long*)0x801DE600

    #define TEMP_MEMORY_ADDR (s8*)0x801A2600
    #define CD_ADDR_0        (u_long*)0x801E2600 // Used to temporarily allocate VAB files.

    #define FONT24_BUFFER (u_long*)0x801F5600 // Loaded by `GameFs_StfRollBinLoad`.

    #define HARRY_LM_BUFFER (void*)0x800FE600 /** Harry character model. */
    #define MAP_CHARA_BASE  (void*)0x800FEE00

    /** Model of an item held in the player's hand. */
    #define HELD_ITEM_LM_BUFFER \
        (s_LmHeader*)((HARRY_LM_BUFFER) + Fs_GetFileSize(FILE_CHARA_HERO_ILM))

    /** Up to 4 chara models used in a map. */
    #define MAP_CHARA_LM_BUFFER \
        (s_LmHeader*)((MAP_CHARA_BASE) + Fs_GetFileSize(FILE_CHARA_HERO_ILM))

    /** @brief Post-load types.
     *
     * What to do with a queue entry after its `operation` is done. Might be better described as "file format", but
     * it only applies to two filetypes.
     *
     * See `FsQueueEntry::postLoad`.
     */
    enum e_FsQueuePostLoadType
    {
        FsQueuePostLoadType_None = 0, /** Do nothing. */
        FsQueuePostLoadType_Tim  = 1, /** Parse TIM file (`Fs_QueuePostLoadTim`). Can use `extra.image`. */
        FsQueuePostLoadType_Anm  = 2  /** Parse ANM file maybe (`Fs_QueuePostLoadAnm`). Always uses `extra.anm`. */
    };

    /** @brief @deprecated Extra queue entry data describing where to upload a TIM after reading.
     * See `FsQueueExtra`. 
     */
    struct s_FsImageDesc
    {
        std::string AssetName;
    };

    /** @brief Extra queue entry data describing something related to loading some ANM files.
     * See `FsQueueExtra`.
     */
    struct s_FsAnmDesc
    {
        u32            field_0; // Unknown index.
        u32            charaId_4;
        GsCOORDINATE2* coords_8;
    };

    /** @brief Extra data passed with the queue entry.
     *
     * Seems to be either a TIM descriptor (`image`), or something for ANM files (`anm`).
     *
     * For `image` usage, see `Fs_QueuePostLoadTim`.
     *
     * Unknown what exactly `anm` is, but it is used with preprocess type 2, which is only used
     * for some ANM files, but not others. See `Fs_QueuePostLoadAnm`.
     */
    struct s_FsQueueExtra
    {
        s_FsAnmDesc anm; /** Unknown. Used when loading some ANM files. */
    };

    /** @brief FS queue entry.
     *
     * Entry in the FS queue.
     * Holds the state of one read/seek operation and a pointer to the `FileInfo` of the file it's for.
     */
    struct s_FsQueueEntry
    {
        const s_FileInfo* info;         /** Pointer to the file table entry of the file this entry is for. */
        u8                operation;    /** What to do. See `e_FsQueueOperation`. */
        u8                postLoad;     /** What to do after `operation` is done. See `e_FsQueuePostLoadType`. */
        u8                allocate;     /** Boolean. If `true`, allocate a buffer for `data` from `g_FsMemory`, otherwise use `externalData` */
        void*             externalData; /** Pointer to an external buffer. */
        s_FsQueueExtra    extra;        /** Extra data, used during post-load. */
        void*             data;         /** Output buffer. Either allocated or same as `externalData`. */
    };

    /** @brief FS queue.
     *
     * State of the file system operations queue.
     *
     * The queue is used for async reads from the CD and from PCDRV and async seeking on the CD.
     * Each operation can optionally include a post-process (post-load) step that happens after the main operation is complete.
     * For example, parsing a TIM file after reading it.
     *
     * New entries are added by `Fs_QueueEnqueue`. The queue is processed in `Fs_QueueUpdate`.
     *
     * @note Assuming this is a single struct because there's a `bzero` that zeroes out this entire block.
     */
    struct s_FsQueue
    {
        int Dummy;
    };

    /** The FS queue. See `s_FsQueue`. */
    extern s_FsQueue g_FsQueue;

    /** @brief Checks if a queue entry index has been loaded and post-loaded.
     *
     * @param queueIdx Index of the queue entry to check.
     * @return `true` if the entry has been fully processed, `false` otherwise.
     */
    bool Fs_QueueIsEntryLoaded(int queueIdx);

    /** @brief Gets the number of operations currently in the queue.
     *
     * @return Number of operations in the queue. Includes both pending reads and pending post-loads.
     */
    int Fs_QueueGetLength();

    /** @brief Loads map chunks currently in view.
     *
     * @return `true` if the chunks in view are loaded, `false` otherwise.
     */
    bool Fs_QueueChunksLoad();

    /** @brief Spin-waits for the queue to become empty while calling `Fs_QueueUpdate`.
     * Calls some bodyprog functions before and after the wait, `VSync` during the waits and `DrawSync`
     * after the wait.
     */
    void Fs_QueueWaitForEmpty();

    /** @brief Add a new seek operation to the queue.
     *
     * @param fileIdx File table index of the file to seek to.
     * @return Index of the new queue entry.
     */
    const std::future<void>& Fs_QueueStartSeek(e_FsFile fileIdx);

    const std::future<void>& Fs_QueueStartRead(e_FsFile fileIdx);

    /** @brief Add a new read operation to the queue.
     *
     * @param fileIdx File table index of the file to read.
     * @param dest Destination buffer. Seems there are no size checks.
     * @return Index of the new queue entry.
     */
    const std::future<void>& Fs_QueueStartRead(e_FsFile fileIdx, void* dest);

    /** @brief Add a new TIM read operation to the queue.
     * Adds a read operation with `post-load = FsQueuePostLoadType_Tim`.
     *
     * @param fileIdx File table index of the file to read.
     * @param dest Destination buffer. There are no size checks.
     * @param image Where to upload the TIM in VRAM.
     * @return Index of the new queue entry.
     */
    const std::future<void>& Fs_QueueStartReadTim(e_FsFile fileIdx, void* dest = nullptr, s_FsImageDesc* image = nullptr);

    /** @brief Add a new ANM read operation to the queue.
     * Adds a read operation with `postLoad = FsQueuePostLoadType_Anm`.
     *
     * @note Does not actually take a file number, but instead takes one from an array of structs at 0x800A90FC in bodyprog,
     * using `arg1` as an index. Does not seem to take a `s_FsAnmDesc` pointer either. Maybe by value?
     *
     * @param idx Unknown index.
     * @param charaId Character ID.
     * @param dest Destination buffer.
     * @param coords Character coords.
     * @return Index of the new queue entry.
     */
    const std::future<void>& Fs_QueueStartReadAnm(int idx, int charaId, void* dest, GsCOORDINATE2* coords);

    /** Seems to clear the queue. */
    void Fs_QueueReset();

    /** @brief Ticks post-loading once.
     *
     * Performs one step in the post-loading process according to `entry->postLoadState`. When the whole process is done, returns 1.
     *
     * @param entry Entry to tick.
     * @return `true` when `entry` is done post-loading, `false` otherwise.
     */
    bool Fs_QueueUpdatePostLoad(s_FsQueueEntry* entry);

    /** @brief Parse an ANM file after loading it?
     *
     * Called during post-loading for some ANM files (when `entry->postLoad = 2`).
     * For example, it is called for CLD2.ANM, SRL.ANM, SBL.ANM (maybe ones used for NPCs?), but not for others
     * (e.g. HB_M0S01.ANM).
     *
     * Calls into BODYPROG, where a function gets a `GsCOORDINATE2` from somewhere in the loaded data.
     * Always uses data from `entry->extra.anm`.
     *
     * @param entry Entry to parse.
     * @return Always `true`.
     */
    bool Fs_QueuePostLoadAnm(s_FsQueueEntry* entry);
}
