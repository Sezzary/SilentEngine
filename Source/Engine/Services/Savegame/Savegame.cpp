#include "Framework.h"
#include "Engine/Services/Savegame/Savegame.h"

#include "Engine/Application.h"
#include "Engine/Services/Filesystem.h"
#include "Engine/Services/Savegame/Generated/savegame_generated.h"

namespace Silent::Services
{
    static const char* SAVE_LOCATION_NAMES[] =
    {
        "Anywhere",
        "Cafe",
        "Bus",
        "Store",
        "Infirmary",
        "Doghouse",
        "Gordon",
        "Church",
        "Garage",
        "Police",
        "Reception",
        "Room_302",
        "Director's",
        "Jewelry_shop",
        "Pool_hall",
        "Antique_shop",
        "Theme_park",
        "Boat",
        "Bridge",
        "Motel",
        "Lighthouse",
        "Sewer",
        "Nowhere",
        "Child's_room",
        "Next_fear"
    };

    const std::vector<SavegameMetadata>& SavegameManager::GetSlotMetadata(int slotIdx)
    {
        Assert(slotIdx < _slotMetadata.size(), "Attempted to get metadata for invalid save slot.");

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

        // TODO: Fill metadata.

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

        Log("Saved game to slot " + std::to_string(slotIdx + 1) +
            ", file " + std::to_string(fileIdx + 1) +
            ", savegame " + std::to_string(saveIdx + 1) + ".",
            LogLevel::Info);
    }

    void SavegameManager::Load(int slotIdx, int fileIdx, int saveIdx)
    {
        auto saveFile = GetSavegamePath(slotIdx, fileIdx, saveIdx);

        // Open savegame buffer file.
        auto inputFile = std::ifstream(saveFile, std::ios::binary);
        if (!inputFile.is_open())
        {
            Log("Attempted to load missing savegame for slot " + std::to_string(slotIdx + 1) +
                ", file " + std::to_string(fileIdx + 1) +
                ", savegame " + std::to_string(saveIdx + 1) + ".",
                LogLevel::Warning, LogMode::Debug);
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

        Log("Loaded game from slot " + std::to_string(slotIdx + 1) +
            ", file " + std::to_string(fileIdx + 1) +
            ", savegame " + std::to_string(saveIdx + 1) + ".",
            LogLevel::Info);
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
        Assert(slotIdx < _slotMetadata.size(), "Attempted to get savegame path for invalid slot.");

        const auto& fs = g_App.GetFilesystem();

        auto slotFolderName = SAVEGAME_SLOT_FOLDER_NAME_BASE + std::to_string(slotIdx + 1);
        auto fileFolderName = SAVEGAME_SLOT_FILE_FOLDER_NAME_BASE + std::to_string(fileIdx + 1);
        auto saveFilename   = std::to_string(saveIdx + 1) + SAVEGAME_FILE_EXT;
        return fs.GetWorkFolder() / SAVEGAME_FOLDER_NAME / slotFolderName / fileFolderName / saveFilename;
    }

    SavegameMetadata SavegameManager::GetMetadata(const std::filesystem::path& saveFile) const
    {
        // Open savegame buffer file.
        auto inputFile = std::ifstream(saveFile, std::ios::binary);
        if (!inputFile.is_open())
        {
            Log("Attempted to get metadata for missing savegame file `" + saveFile.string() + "'.", LogLevel::Warning, LogMode::Debug);
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
        
        // TODO: Read metadata from savegame buffer.
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
            auto slotFolderName = SAVEGAME_SLOT_FOLDER_NAME_BASE + std::to_string(slotIdx + 1);
            auto slotFolder     = fs.GetWorkFolder() / slotFolderName;
            if (!std::filesystem::exists(slotFolder) || !std::filesystem::is_directory(slotFolder))
            {
                continue;
            }

            // Collect file folders.
            auto fileFolders = std::vector<std::filesystem::path>{};
            for (const auto& fileFolder : std::filesystem::directory_iterator(slotFolder))
            {
                if (fileFolder.is_directory())
                {
                    fileFolders.push_back(fileFolder.path());
                }
            }

            // Sort file folders.
            std::sort(fileFolders.begin(), fileFolders.end(), [](const std::filesystem::path& file0, const std::filesystem::path& file1)
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
            for (const auto& fileFolder : fileFolders)
            {
                for (const auto& saveFile : std::filesystem::directory_iterator(fileFolder))
                {
                    if (saveFile.is_regular_file() && saveFile.path().extension() == SAVEGAME_FILE_EXT)
                    {
                        saveFiles.push_back(saveFile.path());
                    }
                }
            }

            // Sort savegame files.
            std::sort(saveFiles.begin(), saveFiles.end(), [](const std::filesystem::path& file0, const std::filesystem::path& file1)
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

        // TODO

        return save;
    }

    std::unique_ptr<flatbuffers::FlatBufferBuilder> SavegameManager::ToSavegameBuffer(const Savegame& save) const
    {
        auto saveBuffer = std::make_unique<flatbuffers::FlatBufferBuilder>();

        // TODO

        return saveBuffer;
    }
}
