#pragma once

namespace Silent::Game
{
    struct s_FsImageDesc;

    /** @brief Gamma intensity of background images. */
    extern q0_8 g_Screen_BackgroundImgGamma;

    void Screen_BackgroundImgDraw(const std::string& assetName, bool fit,
                                  int paletteIdx = 0, const Color& bgColor = Color::Clear);

    /** @brief Draws a background image.
     *
     * @note Only applies to menus and the preview of maps when grabbing them.
     */
    void Screen_BackgroundImgDraw(s_FsImageDesc* image);

    /** @brief Sets a black background and slowly fades in a background image.
     *
     * @param image Image to draw.
     */
    void Screen_BackgroundImgDrawBlackFadeIn(s_FsImageDesc* image);

    /** @brief Draws a transition between images.
     *
     * @param assetName0 Previous image to transition from.
     * @param assetName1 Image to transition into.
     * @param alpha Blend alpha to the second image.
     * @param fit `true` for letterboxed screen fit, `false` for fullscreen fill.
     */
    void Screen_BackgroundImgTransition(const std::string& assetName0, const std::string& assetName1, q3_12 alpha,
                                        bool fit);

    /** @brief Applies motion blur to the background from the scene.
     *
     * @note Used only in the loading screen.
     *
     * @return Always returns `false`.
     */
    bool Screen_BackgroundMotionBlur(s32 vBlanks);
}
