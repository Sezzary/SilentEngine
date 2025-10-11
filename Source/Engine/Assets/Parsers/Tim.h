#pragma once

namespace Silent::Assets
{
    /** @brief TIM asset data. */
    struct TimAsset
    {
        uint               ColorDepth = 0;
        Vector2i           Resolution = Vector2i::Zero;
        std::vector<uint8> Pixels     = {};
    };

    /** @brief Parses a TIM file to a usable asset.
     *
     * @param filename Absolute asset file path on the system.
     * @return Parsed TIM asset data as a `void` pointer.
     */
    std::shared_ptr<void> ParseTim(const std::filesystem::path& filename);
}
