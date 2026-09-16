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

    constexpr float SERIF_FONT_SCALE       = RETRO_PIXEL_SCALE.y * 16.0f;
    constexpr float SANS_FONT_SCALE        = RETRO_PIXEL_SCALE.y * 10.0f;
    constexpr float SERIF_FONT_LINE_HEIGHT = SCREEN_SPACE_RES.y / 14.0f;
    constexpr float SANS_FONT_LINE_HEIGHT  = SCREEN_SPACE_RES.y / 8.0f;

    /** @brief String color IDs for strings displayed in screen space.
     * Used as indices into `STRING_COLORS`.
     * @todo Rename to `e_MsgColorArg`.
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

    /** @brief Message selection prompt aruments. @todo Rework values. `YesOrNo` was 4 in legacy. */
    enum class MsgSelectArg
    {
        None    = NO_VALUE,
        YesOrNo = 0,
        Select2 = 2,
        Select3 = 3
    };

    /** @brief Message return codes. @todo Convert to flags. */
    enum class MsgReturnCode
    {
        None,
        EndPage,
        End
    };

    /** @brief Processed message node types. */
    enum class MsgNodeType
    {
        Text,
        Command
    };

    /** @brief Processed message node. */
    struct MsgNode
    {
        MsgNodeType Type  = MsgNodeType::Text;
        std::string Value = {};

        char  GetCode() const;
        int   GetIntArg() const;
        float GetFloatArg() const;
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

    /** @brief Return result for drawn parsed message. */
    struct MsgReturnResult
    {
        bool               DisplayAll = false;
        std::vector<float> LineWidths = {};
        MsgReturnCode      Code       = MsgReturnCode::None;
        MsgSelectArg       Select     = MsgSelectArg::None;
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

    /** @brief Submits a string to draw in screen space.
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

    /** @brief Submits a parsed message to draw in screen space.
     *
     * @param msg Parsed message to draw.
     * @param pos Start position in screen percent.
     * @param scale Scale relative to the screen height.
     * @param glyphCount Consecutive glyphs to draw from the message. Used for rollout.
     * @param pageIdx Index of the page to draw in the message.
     * @return Message return result.
     */
    MsgReturnResult DrawParsedMsg(const ParsedMsg& msg, const Vector2& pos, float scale,
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

    /** @brief Resets global map message parameters to defaults. */
    void Gfx_MapMsg_Reset();

    /** @brief Submits a simple tagged message to draw in screen space using the serif font.
     * The position and color must be set by * `Gfx_StringPositionSet` and `Gfx_StringColorSet` before calling this
     * function.
     *
     * @note The message must contain a single page.
     *
     * @param msg Tagged message to draw.
     * @param displayLength Number of consecutive glyphs to draw from the string.
     * @param isHalfHeight Use half-height glyphs.
     * @return String width.
     */
    float Gfx_StringDraw(const std::string& msg, int displayLength = INT_MAX, bool isHalfHeight = false);

    /** @brief Draws an integer string in screen space using the serif font.
     *
     * @param lengthMin Minimum length.
     * @param val Integer to draw.
     * @return String width.
     */
    float Gfx_StringDrawInt(int lengthMin, int val);

    /** @brief Sets the position at which text will be drawn.
     *
     * @param posX X text screen position.
     * @param posY Y text screen position.
     */
    void Text_Debug_PositionSet(int posX, int posY);

    /** @brief Submits a simple tagged message to draw in screen space using the serif font.
     * The position and color must be set by `Gfx_StringPositionSet` and `Gfx_StringColorSet` before calling this
     * function.
     *
     * @note The message must contain a single page.
     *
     * @param msg Tagged message to draw.
     */
    float Text_Debug_Draw(const std::string& msg);

    /** @brief Converts an integer to a string.
     *
     * @param lengthMin Minimum length of the integer string.
     * @param val Integer to convert.
     * @return Integer string.
     */
    std::string Text_Debug_IntToString(int lengthMin, int val);
}
