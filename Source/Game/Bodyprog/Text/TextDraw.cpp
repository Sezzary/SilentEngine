#include "Framework.h"
#include "Psx.h"
#include "Game/Bodyprog/Text/TextDraw.h"

#include "Game/Bodyprog/Bodyprog.h"

#include "Application.h"
#include "Renderer/Renderer.h"
#include "Renderer/Common/Enums.h"
#include "Renderer/Common/Resources/Scene/Text2d.h"
#include "Renderer/Common/Utils.h"
#include "Utils/Font.h"
#include "Utils/Utils.h"

using namespace Silent::Renderer;
using namespace Silent::Utils;

namespace Silent::Game
{
    constexpr int TEXT_DEPTH = 6;

    /** @brief Message line position override types for `MSG_CODE_LINE_POSITION` arguments. */
    enum class MsgLinePositionType
    {
        Subtitle    = 0,
        Information = 1,

        Count
    };

    constexpr auto STRING_COLORS = std::array<Color, StringColorId_Count>
    {
        Color::From8Bit(160, 128, 64),
        Color::From8Bit(32,  32,  32),
        Color::From8Bit(24,  128, 40),
        Color::From8Bit(8,   184, 96),
        Color::From8Bit(128, 0,   0),
        Color::From8Bit(8,   8,   8),
        Color::From8Bit(100, 100, 100),
        Color::From8Bit(128, 128, 128)
    };

    Vector2i g_StringPosition;
    int      g_MapMsg_AudioType;

    static auto g_StringColorId      = StringColorId_White;
    static auto g_SansStringPosition = Vector2i::Zero;
    
    char MsgNode::GetCode() const
    {
        // Retrieve code argument, e.g. `C` in `C1`.
        return Value[0];
    }

    int MsgNode::GetIntArg() const
    {
        // Parse `int` value, e.g. `1` in `C1`.
        auto intStr = std::string();
        for (char c : Value.substr(1, Value.size() - 1))
        {
            intStr += c;
        }

        // Convert string to `int`.
        return std::stoi(intStr);
    }

    float MsgNode::GetFloatArg() const
    {
        // Parse `float` value, e.g. `1.5` in `J0(1.5)`.
        auto floatStr = std::string();
        bool collect = false;
        for (char c : Value)
        {
            // Reached start.
            if (!collect)
            {
                if (c == '(')
                {
                    collect = true;
                }

                continue;
            }

            // Reached end.
            if (c == ')')
            {
                break;
            }

            floatStr += c;
        }

        // Convert string to `float`.
        return std::stof(floatStr);
    }

    /** @brief Parses a tagged message into nodes.
     *
     * @param msg Tagged message.
     * @return Message nodes.
     */
    static std::vector<MsgNode> GetMsgNodes(const std::string& msg)
    {
        auto buffer = std::string();
        bool inCmd  = false;

        // Parse message into nodes.
        auto nodes = std::vector<MsgNode>{};
        for (char c : msg)
        {
            // Start collecting command.
            if (c == '{' && !inCmd)
            {
                if (!buffer.empty())
                {
                    nodes.push_back(MsgNode
                    {
                        .Type  = MsgNodeType::Text,
                        .Value = buffer
                    });

                    buffer.clear();
                }

                inCmd = true;
            }
            // Finish collecting command.
            else if (c == '}' && inCmd)
            {
                nodes.push_back(MsgNode
                {
                    .Type  = MsgNodeType::Command,
                    .Value = buffer
                });

                buffer.clear();
                inCmd = false;
            }
            // Collect `char`s.
            else
            {
                buffer += c;
            }
        }

        // Flush remaining buffer content.
        if (!buffer.empty())
        {
            nodes.push_back(MsgNode
            {
                .Type  = inCmd ? MsgNodeType::Command : MsgNodeType::Text,
                .Value = buffer
            });
        }

        return nodes;
    }

    /** @brief Parses message nodes into pages.
     *
     * @param nodes Message nodes.
     * @param fontName Font name.
     * @return Message pages.
     */
    static std::vector<MsgPage> GetMsgPages(const std::vector<MsgNode>& nodes, const std::string& fontName)
    {
        auto& fonts = g_App.GetFonts();

        // Get font.
        auto* font = fonts.GetFont(fontName);
        if (font == nullptr)
        {
            Debug::Log(Fmt("Attempted to parse tagged message with invalid font `{}`.", fontName),
                       Debug::LogLevel::Error);
            return {};
        }

        // Collect nodes into pages.
        auto pages      = std::vector<MsgPage>{};
        auto curPage    = MsgPage{};
        bool addNewLine = true;
        for (const auto& node : nodes)
        {
            curPage.Nodes.push_back(node);

            if (node.Type == MsgNodeType::Text)
            {
                if (addNewLine)
                {
                    curPage.LineWidths.push_back(0.0f);
                    addNewLine = false;
                }

                auto  shape                = font->GetShapedText(node.Value);
                float fontScaleFactor      = SCREEN_SPACE_RES.x / (float)font->GetPointSize();
                curPage.LineWidths.back() += shape.Width * fontScaleFactor;
            }
            else if (node.Type == MsgNodeType::Command)
            {
                switch (node.GetCode())
                {
                    case MSG_CODE_NEWLINE:
                    {
                        addNewLine = true;
                        break;
                    }
                    case MSG_CODE_END_PAGE:
                    {
                        pages.push_back(curPage);
                        curPage    = {};
                        addNewLine = true;
                    }
                }
            }
        }

        // Flush remaining page.
        if (!curPage.Nodes.empty())
        {
            pages.push_back(std::move(curPage));
        }

        return pages;
    }

    ParsedMsg GetParsedMsg(const std::string& msg, const std::string& fontName, float lineHeight)
    {
        // Get nodes and pages.
        auto nodes = GetMsgNodes(msg);
        auto pages = GetMsgPages(nodes, fontName);

        return ParsedMsg
        {
            .Pages      = std::move(pages),
            .FontName   = fontName,
            .LineHeight = lineHeight
        };
    }

    float DrawString(const std::string& str, const std::string& fontName, const Vector2& pos, float scale,
                     const Color& color, int styleFlags, AlignMode alignMode)
    {
        auto& renderer = g_App.GetRenderer();

        // Submit text.
        auto text = Text2d::CreateText2d(str, fontName,
                                         pos, 0.0f, scale,
                                         color, styleFlags,
                                         TEXT_DEPTH, alignMode);
        renderer.SubmitText2d(text);

        float fontScaleFactor = SCREEN_SPACE_RES.x / (float)text.Font->GetPointSize();
        return (text.Shape.Width * fontScaleFactor) * scale;
    }

    MsgReturnResult DrawParsedMsg(const ParsedMsg& msg, const Vector2& pos, float scale,
                                  int styleFlags, int displayLength, int pageIdx)
    {
        constexpr float TAB_SIZE = (40.0f / RETRO_SCREEN_SPACE_RES.x) * SCREEN_SPACE_RES.x;
        constexpr float MARGIN   = (8.0f  / RETRO_SCREEN_SPACE_RES.y) * SCREEN_SPACE_RES.y;

        struct State
        {
            Vector2         Position     = Vector2::Zero;
            Vector2         LineOffset   = Vector2::Zero;
            Vector2         StringOffset = Vector2::Zero;
            float           LineHeight   = 0.0f;
            e_StringColorId ColorId      = StringColorId_White;
            int             StyleFlags   = (int)TextStyleFlags::None;
            AlignMode       AlignMd      = AlignMode::BottomLeft;
        };

        auto state = State
        {
            .Position   = pos,
            .LineHeight = msg.LineHeight,
            .StyleFlags = styleFlags
        };

        auto result = MsgReturnResult{};
        result.LineWidths.push_back(0.0f);

        // Run through message nodes.
        const auto& page = msg.Pages[pageIdx];
        for (const auto& node : page.Nodes)
        {
            // Skip invalid node.
            if (node.Value.empty())
            {
                continue;
            }

            // Draw text.
            if (node.Type == MsgNodeType::Text)
            {
                // Draw text string.
                int   glyphCount = GetUtf8CodePoints(node.Value).size();
                auto  str        = GetUtf8Substring(node.Value, 0, std::min(glyphCount, displayLength));
                auto  pos        = GetGridAlignedScreenPercent((state.Position + state.LineOffset) + state.StringOffset,
                                                               RETRO_SCREEN_SPACE_RES.y);
                float width      = DrawString(str, msg.FontName, pos, scale,
                                              STRING_COLORS[state.ColorId], state.StyleFlags,
                                              state.AlignMd);
                result.LineWidths.back() += width;

                // Accumulate string offset.
                auto aspectCorrection = GetScreenAspectCorrection(GLYPH_SCALE_MODE);
                state.StringOffset.x += width * aspectCorrection.x;

                // Stop drawing if length exceeded.
                displayLength -= glyphCount;
                if (displayLength <= 0)
                {
                    result.Code = MsgReturnCode::None;
                    return result;
                }
            }
            // Handle command.
            else if (node.Type == MsgNodeType::Command)
            {
                switch (node.GetCode())
                {
                    case MSG_CODE_DISPLAY_ALL:
                    {
                        result.DisplayAll = true;
                        break;
                    }
                    case MSG_CODE_COLOR:
                    {
                        state.ColorId = (e_StringColorId)node.GetIntArg();
                        break;
                    }
                    case MSG_CODE_END:
                    {
                        result.Code = MsgReturnCode::End;
                        break;
                    }
                    case MSG_CODE_HALF_HEIGHT:
                    {
                        g_SysWork.enableHalfHeightGlyphs = true;

                        state.StyleFlags &= (int)TextStyleFlags::HalfHeight;
                        state.LineHeight *= 0.5f;
                        break;
                    }
                    case MSG_CODE_JUMP:
                    {
                        // @todo What do these values mean? This was only set when computing line widths in legacy code.
                        if (node.GetIntArg() == 2)
                        {
                            g_MapMsg_AudioType = 3; // @todo Should use `MapMsgAudioType_VoiceStream`.
                        }

                        // Skip if another message is already in progress.
                        if (g_SysWork.mapMsgTimer != NO_VALUE)
                        {
                            break;
                        }

                        // Set map message timer.
                        g_SysWork.mapMsgTimer = Q12(node.GetFloatArg());
                        break;
                    }
                    case MSG_CODE_LINE_POSITION:
                    {
                        switch ((MsgLinePositionType)node.GetIntArg())
                        {
                            case MsgLinePositionType::Subtitle:
                            {
                                // Compute widest line width.
                                float widestLineWidth = 0.0f;
                                for (float lineWidth : page.LineWidths)
                                {
                                    if (lineWidth > widestLineWidth)
                                    {
                                        widestLineWidth = lineWidth;
                                    }
                                }

                                // @todo How would alignments interact?
                                // Set start line position.
                                float startOffset      = (widestLineWidth * scale) * 0.5f;
                                float blockHeight      = ((page.LineWidths.size() - 1) * state.LineHeight);
                                auto  aspectCorrection = GetScreenAspectCorrection(GLYPH_SCALE_MODE);
                                state.Position         = Vector2((SCREEN_SPACE_RES.x * 0.5f) - (startOffset * aspectCorrection.x),
                                                                 ((SCREEN_SPACE_RES.y - blockHeight) - MARGIN) * aspectCorrection.y);
                                break;
                            }
                            case MsgLinePositionType::Information:
                            {
                                auto aspectCorrection = GetScreenAspectCorrection(GLYPH_SCALE_MODE);
                                state.Position        = ConvertRetroScreenPixelsToPercent(Vector2i(160, 76)) *
                                                        aspectCorrection;
                                break;
                            }
                        }
                        break;
                    }
                    case MSG_CODE_ALIGN_CENTER:
                    {
                        state.AlignMd = AlignMode::CenterBottom;
                        break;
                    }
                    case MSG_CODE_NEWLINE:
                    {
                        state.LineOffset.y += state.LineHeight * GetScreenAspectCorrection(GLYPH_SCALE_MODE).y;
                        state.StringOffset  = 0.0f;
                        result.LineWidths.push_back(0.0f);
                        break;
                    }
                    case MSG_CODE_SELECT:
                    {
                        result.Select = (MsgSelectArg)node.GetIntArg();
                        break;
                    }
                    case MSG_CODE_TAB:
                    {
                        // @todo How are tabs applied?
                        //state.LineOffset.x += TAB_SIZE * GetScreenAspectCorrection(GLYPH_SCALE_MODE).x;
                        break;
                    }
                    case MSG_CODE_END_PAGE:
                    {
                        result.Code = MsgReturnCode::EndPage;
                        break;
                    }
                    case MSG_CODE_ALIGN_RIGHT:
                    {
                        state.AlignMd = AlignMode::BottomRight;
                        break;
                    }
                }
            }
        }

        return result;
    }

    void Gfx_StringPositionSet(int posX, int posY)
    {
        if (posX != NO_VALUE)
        {
            g_StringPosition.x = posX;
        }

        if (posY != NO_VALUE)
        {
            g_StringPosition.y = posY;
        }
    }

    void Gfx_StringColorSet(e_StringColorId colorId)
    {
        g_StringColorId = colorId;
    }

    void Gfx_MapMsg_Reset()
    {
        g_StringColorId                  = StringColorId_White;
        g_SysWork.enableHalfHeightGlyphs = false;
    }

    float Gfx_StringDraw(const std::string& msg, int displayLength, bool isHalfHeight)
    {
        const auto& options  = g_App.GetOptions();
        auto&       renderer = g_App.GetRenderer();

        // Draw string.
        auto colorTag   = "{" + std::to_string((int)g_StringColorId) + "}";
        auto fontName   = (options->TextQuality == TextQualityType::Retro) ? "RetroSerif" : "ModernSerif";
        auto parsedMsg  = GetParsedMsg(colorTag + msg, fontName, SERIF_FONT_LINE_HEIGHT);
        auto pos        = ConvertRetroScreenPixelsToPercent(g_StringPosition);
        int  styleFlags = (int)TextStyleFlags::Gradient |
                          (int)TextStyleFlags::Shadow   |
                          (isHalfHeight ? (int)TextStyleFlags::HalfHeight : (int)TextStyleFlags::None);
        auto result     = DrawParsedMsg(parsedMsg, pos, SERIF_FONT_SCALE, styleFlags, displayLength, 0);

        return result.LineWidths.back();
    }

    float Gfx_StringDrawInt(int lengthMin, int val)
    {
        auto strVal = std::to_string(val);
        while (strVal.size() < lengthMin)
        {
            strVal.insert(strVal.begin(), ' ');
        }

        return Gfx_StringDraw(strVal);
    }

    void Text_Debug_PositionSet(int posX, int posY)
    {
        if (posX != NO_VALUE)
        {
            g_SansStringPosition.x = posX;
        }

        if (posY != NO_VALUE)
        {
            g_SansStringPosition.y = posY;
        }
    }

    float Text_Debug_Draw(const std::string& msg)
    {
        const auto& options  = g_App.GetOptions();
        auto&       renderer = g_App.GetRenderer();

        // Draw string.
        auto colorTag   = "{" + std::to_string(StringColorId_White) + "}";
        auto fontName   = (options->TextQuality == TextQualityType::Retro) ? "RetroSans" : "ModernSans";
        auto parsedMsg  = GetParsedMsg(colorTag + msg, fontName, SANS_FONT_LINE_HEIGHT);
        auto pos        = ConvertRetroScreenPixelsToPercent(g_StringPosition);
        int  styleFlags = (int)TextStyleFlags::Shadow;
        auto result     = DrawParsedMsg(parsedMsg, pos, SANS_FONT_SCALE, styleFlags, INT_MAX, 0);

        return result.LineWidths.back();
    }

    std::string Text_Debug_IntToString(int lengthMin, int val)
    {
        auto strVal = std::to_string(val);
        while (strVal.size() < lengthMin)
        {
            strVal.insert(strVal.begin(), ' ');
        }

        return strVal;
    }
}
