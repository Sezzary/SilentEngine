#pragma once

namespace Silent::Game
{
    struct s_FsImageDesc;

    /** @brief Gamma intensity of background images. */
    extern q0_8 g_Screen_BackgroundImgGamma;

    void Screen_BackgroundImgDraw(const std::string& assetName, bool fit = true, int paletteIdx = 0);

    /** @brief Draws a background image.
     *
     * @note Only applies to menus and the preview of maps when grabbing them.
     */
    void Screen_BackgroundImgDraw(s_FsImageDesc* image);

    /** @brief Draws a background image.
     *
     * @note Only applies to background images of notes or puzzles images.
     */
    void Screen_BackgroundImgDrawAlt(s_FsImageDesc* image);

    void Screen_BackgroundImgTransition(const std::string& assetName0, const std::string& assetName1, q3_12 alpha,
                                        bool fit);

    /** @brief Applies motion blur to the scene.
     *
     * @note Used only in the loading screen.
     */
    bool Screen_BackgroundMotionBlur(s32 vBlanks);
}
