#pragma once

#include "Renderer/Common/Constants.h"
#include "Renderer/Common/Enums.h"

using namespace Silent::Renderer;

namespace Silent::Game
{
    constexpr char MSG_CODE_COLOR         = 'C'; /** Set color. */
    constexpr char MSG_CODE_DISPLAY_ALL   = 'D'; /** Display message instantly with no rollout. */
    constexpr char MSG_CODE_END           = 'E'; /** End message. */
    constexpr char MSG_CODE_HALF_HEIGHT   = 'H'; /** Half-height glyphs. */
    constexpr char MSG_CODE_JUMP          = 'J'; /** Jump timer. */
    constexpr char MSG_CODE_LINE_POSITION = 'L'; /** Set next line position override. */
    constexpr char MSG_CODE_ALIGN_CENTER  = 'M'; /** Align center. */
    constexpr char MSG_CODE_NEWLINE       = 'N'; /** Newline. */
    constexpr char MSG_CODE_SELECT        = 'S'; /** Display dialog prompt with selectable entries. */
    constexpr char MSG_CODE_TAB           = 'T'; /** Inset line. */
    constexpr char MSG_CODE_END_PAGE      = 'P'; /** End page. */
    constexpr char MSG_CODE_ALIGN_RIGHT   = 'R'; /** Align right. */

    constexpr float SERIF_FONT_SCALE = RETRO_PIXEL_SCALE.y * 16.0f;

    /** @brief Message return codes. @todo Convert to flags. */
    enum e_MsgReturnCode
    {
        MsgReturnCode_None    = 0,
        MsgReturnCode_End     = 1,
        MsgReturnCode_Select2 = 2,
        MsgReturnCode_Select3 = 3,
        MsgReturnCode_Select4 = 4,

        MsgReturnCode_EndPage    = 19,
        MsgReturnCode_DisplayAll = 20
    };

    /** @brief String color IDs for strings displayed in screen space.
     * Used as indices into `STRING_COLORS`.
     */
    enum e_StringColorId
    {
        StringColorId_Gold      = 0,
        StringColorId_DarkGrey  = 1,
        StringColorId_Green     = 2,
        StringColorId_Nuclear   = 3,
        StringColorId_Red       = 4,
        StringColorId_Black     = 5,
        StringColorId_LightGrey = 6,
        StringColorId_White     = 7,

        StringColorId_Count
    };

    /** @brief Processed message node types. */
    enum class NodeType
    {
        Text,
        Command
    };

    /** @brief Processed message node. */
    struct MsgNode
    {
        NodeType    Type  = NodeType::Text;
        std::string Value = {};

        char  GetCode() const;
        int   GetIntArg() const;
        float GetTimeArg() const;
    };

    /** @brief Message page. */
    struct MsgPage
    {
        std::vector<MsgNode> Nodes      = {};
        std::vector<float>   LineWidths = {};
    };

    /** @brief Parsed message. */
    struct ParsedMsg
    {
        std::vector<MsgPage> Pages      = {};
        std::string          FontName   = {};
        float                LineHeight = 0.0f;
    };

    extern Vector2i g_StringPosition;
    extern int      g_MapMsg_AudioType;

    /** @brief Parses a tagged message.
     *
     * @param msg Tagged message.
     * @param fontName Name of the font to use.
     * @param lineHeight Height per line in screen percent.
     * @return Parsed message.
     */
    ParsedMsg GetParsedMsg(const std::string& msg, const std::string& fontName, float lineHeight);

    /** @brief Draws a string in screen space.
     *
     * @param str String to draw.
     * @param fontName Name of the font to use.
     * @param pos Position in screen percent.
     * @param scale Scale relative to the screen height.
     * @param color Color.
     * @param styleFlags Style flags.
     * @param alignMode Alignment mode.
     * @return String width.
     */
    float DrawString(const std::string& str, const std::string& fontName, const Vector2& pos, float scale,
                     const Color& color, int styleFlags, AlignMode alignMode);

    /** @brief Draws a parsed message in screen space.
     *
     * @param msg Parsed message to draw.
     * @param pos Start position in screen percent.
     * @param scale Scale relative to the screen height.
     * @param glyphCount Consecutive glyphs to draw from the message. Used for rollout.
     * @param pageIdx Index of the page to draw in the message.
     * @return Message return code.
     */
    e_MsgReturnCode DrawParsedMsg(const ParsedMsg& msg, const Vector2& pos, float scale,
                                  int styleFlags, int displayLength = INT_MAX, int pageIdx = 0);

    /** @brief Sets the global position of the next string to be drawn by `Gfx_StringDraw`.
     *
     * @param x X screen position.
     * @param y Y screen position.
     */
    void Gfx_StringPositionSet(int posX, int posY);

    /** @brief Sets the global color state of the next string drawn by `Gfx_StringDraw`.
     *
     * @param colorId ID of the new color to set (`e_ColorId`).
     */
    void Gfx_StringColorSet(e_StringColorId colorId);

    /** @brief Resets the global `g_StringLayerIdx` to `DEFAULT_TEXT_LAYER_IDX`. */
    void Gfx_StringLayerIdxSet(s32 layerIdx);

    /** @brief Resets global map message parameters to defaults. */
    void Gfx_MapMsg_Reset();

    /** @brief Draws a string in screen space using 12x16 glyphs. The position and color must be set by
     * `Gfx_StringPositionSet` and `Gfx_StringColorSet` before calling this function.
     *
     * @note References glyphs in `FONT16.TIM`. The texture is loaded into VRAM across multiple texture pages,
     * hence why the texture is a single row with 4-pixel padding every 21st glyph instead of a stacked arrangement.
     *
     * @param str String to draw.
     * @param strLength Number of consecutive glyphs to draw from the string.
     */
    void Gfx_StringDraw(const std::string& str, int displayLength = INT_MAX, bool isHalfHeight = false, AlignMode alignMode = AlignMode::BottomLeft);

    /** @brief Draws an integer string in screen space using 12x16 glyphs.
     *
     * @param widthMin Minimum width of the integer string.
     * @param strLength Number of consecutive glyphs to draw from the integer string.
     */
    void Gfx_StringDrawInt(s32 widthMin, s32 displayLength = INT_MAX);
}
