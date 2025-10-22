#pragma once

namespace Silent::Buffers { struct Savegame; }

namespace Silent::Savegame
{
    constexpr uint SAVEGAME_SLOT_COUNT = 2;   // Number of savegame slots, simulating original design around PSX memory cards.
    constexpr uint SAVEGAME_COUNT_MAX  = 165; // Max savegames per file.

    struct Savegame
    {
        int Dummy = 0;
    };

    struct SavegameMetadata
    {
        int  SlotIdx       = 0;
        int  FileIdx       = 0;
        int  DataIdx       = 0;
        int  SaveCount     = 0;
        int  LocationId    = 0;
        uint GameplayTimer = 0;

        bool IsNextFearMode = false;
        int  Flags          = 0;
    };

    class SavegameManager
    {
    private:
        // Fields

        Savegame                                                       _savegame     = {};
        std::array<std::vector<SavegameMetadata>, SAVEGAME_SLOT_COUNT> _slotMetadata = {};

    public:
        // Constructors

        SavegameManager() = default;

        // Getters

        const std::vector<SavegameMetadata>& GetSlotMetadata(int slotIdx);

        // Utilities

        void Initialize();
        void Save(int slotIdx, int fileIdx, int saveIdx);
        void Load(int slotIdx, int fileIdx, int saveIdx);

        // Operators

        const Savegame* operator->() const;
        Savegame*       operator->();

    private:
        // Helpers

        std::filesystem::path GetSavegamePath(int slotIdx, int fileIdx, int saveIdx) const;
        SavegameMetadata      GetMetadata(const std::filesystem::path& saveFile) const;
        
        void                                            PopulateSlotMetadata();
        std::unique_ptr<Savegame>                       FromSavegameBuffer(const Buffers::Savegame& saveBuffer) const;
        std::unique_ptr<flatbuffers::FlatBufferBuilder> ToSavegameBuffer(const Savegame& save) const;
    };
}
