#pragma once

#include "Engine/Renderer/Common/Enums.h"

namespace Silent::Renderer
{
    /** @brief Computes a screen position in percent corrected according to the aspect ratio of the screen and a scale mode.
     *
     * @param pos Screen position in percent.
     * @param scaleMode Scale mode.
     * @return Screen position in percent corrected for the screen aspect ratio and scale mode.
     */
    Vector2 GetAspectCorrectScreenPosition(const Vector2 pos, ScaleMode scaleMode);

    /** @brief Converts a screen position in percent to normalized device coordinates.
     *
     * @param pos Screen position in percent with components in the range `[0.0f, 100.0f]`.
     * @return Normalized device coordinates with components in the range `[-1.0f, 1.0f]`.
     */
    Vector2 ConvertScreenPositionToNdc(const Vector2& pos);

    /** @brief Converts normalized device coordinates to a screen position in percent.
     *
     * @param ndc Normalized device coordinates with components in the range `[-1.0f, 1.0f]`.
     * @return Screen position in percent with components in the range `[0.0f, 100.0f]`.
     */
    Vector2 ConvertNdcToScreenPosition(const Vector2& ndc);
}
