#include "Framework.h"
#include "Savegame/Savegame.h"

#include "Application.h"
#include "Assets/TranslationKeys.h"
#include "Savegame/Generated/savegame_generated.h"
#include "Services/Filesystem.h"
#include "Utils/Utils.h"

using namespace Silent::Utils;

namespace Silent::Savegame
{
    constexpr char SAVEGAME_SLOT_FILE_DIR_NAME_BASE[] = "File ";
    constexpr char SAVEGAME_SLOT_DIR_NAME_BASE[]      = "Slot ";

    static const std::vector<std::string> SAVE_LOCATION_NAME_KEYS =
    {
        KEY_SAVE_LOC_ANYWHERE,
        KEY_SAVE_LOC_CAFE,
        KEY_SAVE_LOC_BUS,
        KEY_SAVE_LOC_STORE,
        KEY_SAVE_LOC_INFIRMARY,
        KEY_SAVE_LOC_DOGHOUSE,
        KEY_SAVE_LOC_GORDON,
        KEY_SAVE_LOC_CHURCH,
        KEY_SAVE_LOC_GARAGE,
        KEY_SAVE_LOC_POLICE,
        KEY_SAVE_LOC_RECEPTION,
        KEY_SAVE_LOC_ROOM_302,
        KEY_SAVE_LOC_DIRECTORS,
        KEY_SAVE_LOC_JEWELLERY_SHOP,
        KEY_SAVE_LOC_POOL_HALL,
        KEY_SAVE_LOC_ANTIQUE_SHOP,
        KEY_SAVE_LOC_THEME_PARK,
        KEY_SAVE_LOC_BOAT,
        KEY_SAVE_LOC_BRIDGE,
        KEY_SAVE_LOC_MOTEL,
        KEY_SAVE_LOC_LIGHTHOUSE,
        KEY_SAVE_LOC_SEWER,
        KEY_SAVE_LOC_NOWHERE,
        KEY_SAVE_LOC_CHILDS_ROOM,
        KEY_SAVE_LOC_NEXT_FEAR
    };

    const std::vector<SavegameMetadata>& SavegameManager::GetSlotMetadata(int slotIdx)
    {
        Debug::Assert(slotIdx < _slotMetadata.size(), "Attempted to get metadata for invalid save slot.");

        return _slotMetadata[slotIdx];
    }

    void SavegameManager::Initialize()
    {
        PopulateSlotMetadata();
    }

    void SavegameManager::Save(int slotIdx, int fileIdx, int saveIdx)
    {
        // Create savegame buffer.
        auto saveBuffer = ToSavegameBuffer(_savegame);

        // @todo Fill metadata.

        // Ensure directory exists.
        auto saveFile = GetSavegamePath(slotIdx, fileIdx, saveIdx);
        std::filesystem::create_directories(saveFile.parent_path());

        // Write savegame buffer.
        auto outputFile = std::ofstream(saveFile);
        if (outputFile.is_open())
        {
            outputFile.write((const char*)saveBuffer->GetBufferPointer(), saveBuffer->GetSize());
            outputFile.close();
        }

        Debug::Log(Fmt("Saved game to slot {}, file {}, savegame {}.", slotIdx + 1, fileIdx + 1, saveIdx + 1), Debug::LogLevel::Info);
    }

    void SavegameManager::Load(int slotIdx, int fileIdx, int saveIdx)
    {
        auto saveFile = GetSavegamePath(slotIdx, fileIdx, saveIdx);

        // Open savegame buffer file.
        auto inputFile = std::ifstream(saveFile, std::ios::binary);
        if (!inputFile.is_open())
        {
            Debug::Log(Fmt("Attempted to load missing savegame for slot {}, file {}, savegame {}.", slotIdx + 1, fileIdx + 1, saveIdx + 1),
                       Debug::LogLevel::Warning, Debug::LogMode::Debug);
            return;
        }

        // Get file size.
        inputFile.seekg(0, std::ios::end);
        auto fileSize = inputFile.tellg();
        inputFile.seekg(0, std::ios::beg);

        // Read file into buffer object.
        auto fileBuffer = std::vector<char>(fileSize);
        inputFile.read(fileBuffer.data(), fileSize);
        auto* saveBuffer = flatbuffers::GetRoot<Silent::Buffers::Savegame>(fileBuffer.data());

        // Read savegame buffer.
        _savegame = std::move(*FromSavegameBuffer(*saveBuffer));

        Debug::Log(Fmt("Loaded game from slot {}, file {}, savegame {}.", slotIdx + 1, fileIdx + 1, saveIdx + 1), Debug::LogLevel::Info);
    }

    const Savegame* SavegameManager::operator->() const
    {
        return &_savegame;
    }

    Savegame* SavegameManager::operator->()
    {
        return &_savegame;
    }

    std::filesystem::path SavegameManager::GetSavegamePath(int slotIdx, int fileIdx, int saveIdx) const
    {
        Debug::Assert(slotIdx < _slotMetadata.size(), "Attempted to get savegame path for invalid slot.");

        const auto& fs = g_App.GetFilesystem();

        auto slotDirName = SAVEGAME_SLOT_DIR_NAME_BASE + std::to_string(slotIdx + 1);
        auto fileDirName = SAVEGAME_SLOT_FILE_DIR_NAME_BASE + std::to_string(fileIdx + 1);
        auto saveFilename   = std::to_string(saveIdx + 1) + SAVEGAME_FILE_EXT;
        return fs.GetSavegameDirectory() / slotDirName / fileDirName / saveFilename;
    }

    SavegameMetadata SavegameManager::GetMetadata(const std::filesystem::path& saveFile) const
    {
        // Open savegame buffer file.
        auto inputFile = std::ifstream(saveFile, std::ios::binary);
        if (!inputFile.is_open())
        {
            Debug::Log(Fmt("Attempted to get metadata for missing savegame file `{}`.", saveFile.string()), Debug::LogLevel::Warning, Debug::LogMode::Debug);
            return SavegameMetadata
            {
                .SlotIdx        = NO_VALUE,
                .FileIdx        = NO_VALUE,
                .DataIdx        = NO_VALUE,
                .SaveCount      = NO_VALUE,
                .LocationId     = NO_VALUE,
                .GameplayTimer  = 0,
                .IsNextFearMode = false,
                .Flags          = NO_VALUE
            };
        }

        // Get file size.
        inputFile.seekg(0, std::ios::end);
        auto fileSize = inputFile.tellg();
        inputFile.seekg(0, std::ios::beg);

        // Read file into buffer object.
        auto fileBuffer = std::vector<char>(fileSize);
        inputFile.read(fileBuffer.data(), fileSize);
        auto* saveBuffer = flatbuffers::GetRoot<Silent::Buffers::Savegame>(fileBuffer.data());
        
        // @todo Read metadata from savegame buffer.
        auto metadata = SavegameMetadata{};

        return metadata;
    }

    void SavegameManager::PopulateSlotMetadata()
    {
        const auto& fs = g_App.GetFilesystem();

        // Run through slots.
        for (int slotIdx = 0; slotIdx < _slotMetadata.size(); slotIdx++)
        {
            // Get slot folder path.
            auto slotDirName = SAVEGAME_SLOT_DIR_NAME_BASE + std::to_string(slotIdx + 1);
            auto slotDir     = fs.GetWorkDirectory() / slotDirName;
            if (!std::filesystem::exists(slotDir) || !std::filesystem::is_directory(slotDir))
            {
                continue;
            }

            // Collect file folders.
            auto fileDirs = std::vector<std::filesystem::path>{};
            for (const auto& fileDir : std::filesystem::directory_iterator(slotDir))
            {
                if (fileDir.is_directory())
                {
                    fileDirs.push_back(fileDir.path());
                }
            }

            // Sort file folders.
            Sort(fileDirs, [](const std::filesystem::path& file0, const std::filesystem::path& file1)
            {
                auto extractNumber = [](const std::filesystem::path& file)
                {
                    auto filename   = file.string(); // "File [ID]".
                    auto numericStr = std::string();
                    for (char curChar : filename)
                    {
                        if (std::isdigit(curChar))
                        {
                            numericStr += curChar;
                        }
                    }

                    return std::stoi(numericStr);
                };

                return extractNumber(file0) < extractNumber(file1);
            });

            // Collect savegame files.
            auto saveFiles = std::vector<std::filesystem::path>{};
            for (const auto& fileDir : fileDirs)
            {
                for (const auto& saveFile : std::filesystem::directory_iterator(fileDir))
                {
                    if (saveFile.is_regular_file() && saveFile.path().extension() == SAVEGAME_FILE_EXT)
                    {
                        saveFiles.push_back(saveFile.path());
                    }
                }
            }

            // Sort savegame files.
            Sort(saveFiles, [](const std::filesystem::path& file0, const std::filesystem::path& file1)
            {
                auto extractNumber = [](const std::filesystem::path& file)
                {
                    auto filename   = file.stem().string(); // "[idx]".
                    auto numericStr = std::string();
                    for (char curChar : filename)
                    {
                        if (std::isdigit(curChar))
                        {
                            numericStr += curChar;
                        }
                    }

                    return std::stoi(numericStr);
                };

                return extractNumber(file0) < extractNumber(file1);
            });

            // Clear previous metadata.
            auto& slotMetadata = _slotMetadata[slotIdx];
            slotMetadata.clear();

            // Populate savegame metadata.
            for (const auto& saveFile : saveFiles)
            {
                auto metadata = GetMetadata(saveFile);
                slotMetadata.push_back(metadata);
            }
        }
    }

    std::unique_ptr<Savegame> SavegameManager::FromSavegameBuffer(const Buffers::Savegame& saveBuffer) const
    {
        auto save = std::make_unique<Savegame>();

        // @todo

        return save;
    }

    std::unique_ptr<flatbuffers::FlatBufferBuilder> SavegameManager::ToSavegameBuffer(const Savegame& save) const
    {
        auto saveBuffer = std::make_unique<flatbuffers::FlatBufferBuilder>();

        // @todo

        return saveBuffer;
    }
}
