#pragma once

namespace Silent::Renderer
{
    enum class AlignMode;
    enum class ScaleMode;

    /** @brief Computes a triangle normal from vertices in a clockwise winding order.
     *
     * @param vert0 First vertex.
     * @param vert1 Second vertex.
     * @param vert2 Third vertex.
     * @return Triangle normal.
     */
    Vector3 GetNormal(const Vector3& vert0, const Vector3& vert1, const Vector3& vert2);

    /** @brief Gets the normalized alignment pivot for a given alignment mode.
     *
     * @param alignMode Alignment mode.
     * @return Normalized alignment pivot.
     */
    Vector2 GetAlignmentPivot(AlignMode alignMode);

    /** @brief Computes a screen position snapped to a virtual screen grid.
     *
     * @param pos Screen position to snap.
     * @param virtualHeight Virtual screen grid height.
     */
    Vector2 GetGridAlignedScreenPercent(const Vector2& pos, float virtualHeight);

    /** @brief Computes the screen aspect ratio correction scale according to a provided scale mode.
     *
     * @param scaleMode Scale mode.
     * @return Screen aspect correction.
     */
    Vector2 GetScreenAspectCorrection(ScaleMode scaleMode);

    /** @brief Converts a screen position in retro pixels to percent.
     *
     * @param pos Screen position in retro pixels (320x240 resolution).
     * @return Screen position in percent.
     */
    Vector2 ConvertRetroScreenPixelsToPercent(const Vector2i& pos);

    /** @brief Converts a position in percent to retro screen pixels.
     *
     * @param pos Screen position in retro screen pixels (320x240 resolution).
     * @return Screen position in percent.
     */
    Vector2i ConvertScreenPercentToRetroPixels(const Vector2& pos);

    /** @brief Converts a screen position in percent to normalized device coordinates.
     *
     * @param pos Screen position in percent.
     * @return Normalized device coordinates (viewable range `[-1.0f, 1.0f]`).
     */
    Vector2 ConvertScreenPercentToNdc(const Vector2& pos);

    /** @brief Converts normalized device coordinates to a screen position in percent.
     *
     * @param ndc Normalized device coordinates (viewable range `[-1.0f, 1.0f]`).
     * @return Screen position in percent.
     */
    Vector2 ConvertNdcToScreenPercent(const Vector2& ndc);
}
