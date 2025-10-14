#pragma once

namespace Silent::Assets
{
    /** @brief TIM asset data. */
    struct TimAsset
    {
        Vector2i                         Resolution = Vector2i::Zero;
        std::vector<byte>                Pixels     = {}; // @todo Make this `PixelIdxs`.
        std::vector<std::vector<uint16>> Cluts      = {};
    };

    /** @brief Parses a TIM file to a usable asset.
     *
     * @param filename Absolute asset file path on the system.
     * @return Parsed TIM asset data as a `void` pointer.
     */
    std::shared_ptr<void> ParseTim(const std::filesystem::path& filename);
}
