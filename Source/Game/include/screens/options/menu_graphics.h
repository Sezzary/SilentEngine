#pragma once

#include "Game/include/common.h"

/** @brief Draws a BGM volume bar in the main options menu. */
void Options_MainOptionsMenu_BgmVolumeBarDraw(void);

/** @brief Draws an SFX volume bar in the main options menu. */
void Options_MainOptionsMenu_SfxVolumeBarDraw(void);

/** @brief Draws a volume bar.
 *
 * Called by `Options_MainOptionsMenu_BgmVolumeBarDraw` and `Options_MainOptionsMenu_SfxVolumeBarDraw`.
 */
void Options_MainOptionsMenu_VolumeBarDraw(bool isSfx, uchar vol);

/** @brief Draws the heading and all listed entry strings in the extra options menu. */
void Options_ExtraOptionsMenu_EntryStringsDraw(void);

/** @brief Draws the heading and all listed entry strings in the main options menu. */
void Options_MainOptionsMenu_EntryStringsDraw(void);

/** @brief Draws gold bullet points next to the listed entries and a highlight indicating the
 * selected entry in the extra options menu.
 */
void Options_ExtraOptionsMenu_SelectionHighlightDraw(void);

/** @brief Draws gold bullet points next to the listed entries and a highlight indicating the
 * selected entry in the main options menu.
 */
void Options_MainOptionsMenu_SelectionHighlightDraw(void);

/** @brief Draws the background vignette of Harry in the main and extra options menus. */
void Options_Menu_VignetteDraw(void);

/** @brief Draws configuration strings and blue arrows to the right of the listed entries in the extra options menu. */
void Options_ExtraOptionsMenu_ConfigDraw(void);

/** @brief Draws configuration strings and blue arrows to the right of the listed entries in the main options menu. */
void Options_MainOptionsMenu_ConfigDraw(void);
