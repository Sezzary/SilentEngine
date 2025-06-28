#include "Framework.h"
#include "Engine/Input/Text.h"

#include "Engine/Application.h"
#include "Engine/Input/Action.h"
#include "Utils/Utils.h"

using namespace Silent::Utils;

namespace Silent::Input
{
    static const auto PRINTABLE_ACTION_CHARS = std::unordered_map<ActionId, std::pair<char, char>>
    {
        { In::A,            { 'a', 'A' } },
        { In::B,            { 'b', 'B' } },
        { In::C,            { 'c', 'C' } },
        { In::D,            { 'd', 'D' } },
        { In::E,            { 'e', 'E' } },
        { In::F,            { 'f', 'F' } },
        { In::G,            { 'g', 'G' } },
        { In::H,            { 'h', 'H' } },
        { In::I,            { 'i', 'I' } },
        { In::J,            { 'j', 'J' } },
        { In::K,            { 'k', 'K' } },
        { In::L,            { 'l', 'L' } },
        { In::M,            { 'm', 'M' } },
        { In::N,            { 'n', 'N' } },
        { In::O,            { 'o', 'O' } },
        { In::P,            { 'p', 'P' } },
        { In::Q,            { 'q', 'Q' } },
        { In::R,            { 'r', 'R' } },
        { In::S,            { 's', 'S' } },
        { In::T,            { 't', 'T' } },
        { In::U,            { 'u', 'U' } },
        { In::V,            { 'v', 'V' } },
        { In::W,            { 'w', 'W' } },
        { In::X,            { 'x', 'X' } },
        { In::Y,            { 'y', 'Y' } },
        { In::Z,            { 'z', 'Z' } },
        { In::Num1,         { '1', '!' } },
        { In::Num2,         { '2', '@' } },
        { In::Num3,         { '3', '#' } },
        { In::Num4,         { '4', '$' } },
        { In::Num5,         { '5', '%' } },
        { In::Num6,         { '6', '^' } },
        { In::Num7,         { '7', '&' } },
        { In::Num8,         { '8', '*' } },
        { In::Num9,         { '9', '(' } },
        { In::Num0,         { '0', ')' } },
        { In::Return,       { '\n', '\n' } },
        { In::Space,        { ' ', ' ' } },
        { In::Minus,        { '-', '_' } },
        { In::Equals,       { '=', '+' } },
        { In::BracketLeft,  { '[', '{' } },
        { In::BracketRight, { ']', '}' } },
        { In::Backslash,    { '\\', '|' } },
        { In::Semicolon,    { ';', ':' } },
        { In::Apostrophe,   { '\'', '"' } },
        { In::Comma,        { ',', '<' } },
        { In::Period,       { '.', '>' } },
        { In::Slash,        { '/', '?' } }
    };

    const std::string& TextManager::GetText(const std::string& bufferId) const
    {
        static const auto EMPTY = std::string();

        auto it = _buffers.find(bufferId);
        if (it == _buffers.end())
        {
            Log("Attempted to get text for missing text buffer '" + bufferId + "'.", LogLevel::Warning);
            return EMPTY;
        }

        const auto& [keyId, buffer] = *it;
        return buffer.Text;
    }

    std::vector<std::string> TextManager::GetTextLines(const std::string& bufferId, uint low, uint high) const
    {
        auto it = _buffers.find(bufferId);
        if (it == _buffers.end())
        {
            Log("Attempted to get text lines for missing text buffer '" + bufferId + "'.", LogLevel::Warning);
            return {};
        }

        const auto& [keyId, buffer] = *it;
        if (buffer.Text.empty())
        {
            return {};
        }

        // Compute line range.
        uint lineLow  = (low  == (uint)NO_VALUE) ? 0                        : std::clamp<uint>(low,  0, buffer.LineStarts.size());
        uint lineHigh = (high == (uint)NO_VALUE) ? buffer.LineStarts.size() : std::clamp<uint>(high, 0, buffer.LineStarts.size());
        if (lineLow > lineHigh)
        {
            std::swap(lineLow, lineHigh);
        }

        // Collect lines.
        auto lines = std::vector<std::string>{};
        lines.reserve((lineHigh - lineLow) / buffer.LineWidthMax);
        for (int i = lineLow; i < lineHigh; i++)
        {
            uint lineStart = buffer.LineStarts[i];
            uint lineEnd   = (i < (buffer.LineStarts.size() - 1)) ? buffer.LineStarts[i + 1] : buffer.Text.size();

            auto start = buffer.Text.begin() + lineStart;
            auto end   = buffer.Text.begin() + lineEnd;
            auto line  = std::string(start, end);
            lines.push_back(line);
        }

        return lines;
    }

    uint TextManager::GetCursorPosition(const std::string& bufferId) const
    {
        auto it = _buffers.find(bufferId);
        if (it == _buffers.end())
        {
            Log("Attempted to get cursor position for missing text buffer '" + bufferId + "'.", LogLevel::Warning);
            return 0;
        }

        const auto& [keyId, buffer] = *it;
        return buffer.Cursor;
    }

    void TextManager::InsertBuffer(const std::string& bufferId, uint lineWidthMax, uint charCountMax)
    {
        if (lineWidthMax == 0 || charCountMax == 0)
        {
            Log("Attempted to insert invalid text buffer '" + bufferId +
                "' with max line width " + std::to_string(lineWidthMax) +
                " and character limit " + std::to_string(charCountMax) + ".", LogLevel::Warning);
            return;
        }

        _buffers.try_emplace(bufferId);

        auto& buffer        = _buffers.at(bufferId);
        buffer.LineWidthMax = lineWidthMax;
        buffer.CharCountMax = charCountMax;
    }

    void TextManager::UpdateBuffer(const std::string& bufferId)
    {
        auto it = _buffers.find(bufferId);
        if (it == _buffers.end())
        {
            Log("Attempted to update missing text buffer '" + bufferId + "'.", LogLevel::Warning);
            return;
        }

        auto& [keyId, buffer] = *it;

        // Undo, redo.
        if (HandleHistory(buffer))
        {
            UpdateLineStarts(buffer);
            return;
        }

        // Cut, copy, paste.
        if (HandleClipboard(buffer))
        {
            UpdateLineStarts(buffer);
            return;
        }

        // Add character.
        if (HandleCharacterAdd(buffer))
        {
            UpdateLineStarts(buffer);
            return;
        }

        // Clear characters.
        if (HandleCharacterClear(buffer))
        {
            UpdateLineStarts(buffer);
            return;
        }

        // Move cursor, make selection.
        if (HandleCursorSelection(buffer))
        {
            return;
        }
    }

    void TextManager::RemoveBuffer(const std::string& bufferId)
    {
        auto it = _buffers.find(bufferId);
        if (it == _buffers.end())
        {
            Log("Attempted to clear missing text buffer '" + bufferId + "'.", LogLevel::Warning);
            return;
        }

        _buffers.erase(bufferId);
    }

    bool TextManager::HandleHistory(TextBuffer& buffer)
    {
        const auto& input = g_App.GetInput();

        const auto& shiftAction = input.GetAction(In::Shift);
        const auto& ctrlAction  = input.GetAction(In::Ctrl);
        const auto& zAction     = input.GetAction(In::Z);

        // Undo/redo.
        if (ctrlAction.IsHeld() && zAction.IsHeld())
        {
            if (zAction.IsPulsed(PULSE_DELAY_SEC, PULSE_INITIAL_DELAY_SEC))
            {
                auto& fromStack = shiftAction.IsHeld() ? buffer.Redo : buffer.Undo;
                auto& toStack   = shiftAction.IsHeld() ? buffer.Undo : buffer.Redo;

                if (!fromStack.empty())
                {
                    toStack.push_back(buffer.Text);
                    if (toStack.size() > HISTORY_SIZE_MAX)
                    {
                        toStack.pop_front();
                    }

                    buffer.Text = fromStack.back();
                    fromStack.pop_back();
                }
            }

            return true;
        }

        return false;
    }

    bool TextManager::HandleClipboard(TextBuffer& buffer)
    {
        if (_clipboard.empty() && !buffer.Selection.has_value())
        {
            return false;
        }

        const auto& input = g_App.GetInput();

        const auto& ctrlAction = input.GetAction(In::Ctrl);
        const auto& xAction    = input.GetAction(In::X);
        const auto& cAction    = input.GetAction(In::C);
        const auto& vAction    = input.GetAction(In::V);

        if (ctrlAction.IsHeld())
        {
            // Cut selection.
            if (xAction.IsClicked())
            {
                if (buffer.Selection.has_value())
                {
                    auto start = buffer.Text.begin() + buffer.Selection->first;
                    auto end   = buffer.Text.begin() + buffer.Selection->second;
                    _clipboard = std::string(start, end);
                    buffer.Text.erase(start, end);

                    buffer.Cursor    = buffer.Selection->first;
                    buffer.Selection = std::nullopt;
                    return true;
                }
            }
            // Copy selection.
            else if (cAction.IsClicked())
            {
                if (buffer.Selection.has_value())
                {
                    auto start = buffer.Text.begin() + buffer.Selection->first;
                    auto end   = buffer.Text.begin() + buffer.Selection->second;
                    _clipboard = std::string(start, end);
                    return true;
                }
            }
            // Paste copy.
            else if (vAction.IsClicked())
            {
                if (!_clipboard.empty() && (buffer.Text.size() + _clipboard.size()) <= buffer.CharCountMax)
                {
                    // Replace selection.
                    if (buffer.Selection.has_value())
                    {
                        uint selectLength = buffer.Selection->second - buffer.Selection->first;
                        if (((buffer.Text.size() + _clipboard.size()) - selectLength) <= buffer.CharCountMax)
                        {
                            auto start = buffer.Text.begin() + buffer.Selection->first;
                            auto end   = buffer.Text.begin() + buffer.Selection->second;
                            buffer.Text.erase(start, end);
                            buffer.Text.insert(buffer.Selection->first, _clipboard);

                            buffer.Cursor    = buffer.Selection->first + _clipboard.size();
                            buffer.Selection = std::nullopt;
                            return true;
                        }
                    }
                    // Insert at cursor.
                    else
                    {
                        buffer.Text.insert(buffer.Cursor, _clipboard);
                        buffer.Cursor += _clipboard.size();
                        return true;
                    }
                }
            }
        }

        return false;
    }

    bool TextManager::HandleCharacterAdd(TextBuffer& buffer)
    {
        if (buffer.Text.size() >= buffer.CharCountMax)
        {
            return false;
        }

        const auto& input = g_App.GetInput();

        const auto& shiftAction = input.GetAction(In::Shift);

        bool hasNewChar = false;
        for (auto actionId : ACTION_ID_GROUPS.at(ActionGroupId::Printable))
        {
            const auto& action = input.GetAction(actionId);
            const auto& chars  = PRINTABLE_ACTION_CHARS.at(actionId);

            if (!hasNewChar && action.IsHeld())
            {
                // TODO: Special handling for newline.

                // New action.
                if (!Contains(_prevActionIds, actionId))
                {
                    // Add character.
                    if (action.IsClicked())
                    {
                        PushUndo(buffer);

                        buffer.Text.insert(buffer.Text.begin() + buffer.Cursor, shiftAction.IsHeld() ? chars.second : chars.first);
                        buffer.Cursor++;
                    }

                    _prevActionIds.push_back(actionId);
                    hasNewChar = true;
                }
                // Held action.
                else if (!_prevActionIds.empty() && _prevActionIds.back() == actionId)
                {
                    // Add character.
                    if (action.IsPulsed(PULSE_DELAY_SEC, PULSE_INITIAL_DELAY_SEC))
                    {
                        PushUndo(buffer);

                        buffer.Text.insert(buffer.Text.begin() + buffer.Cursor, shiftAction.IsHeld() ? chars.second : chars.first);
                        buffer.Cursor++;
                        hasNewChar = true;
                    }
                }
            }
            else if (action.IsReleased())
            {
                std::erase(_prevActionIds, actionId);
            }
        }

        return hasNewChar;
    }

    bool TextManager::HandleCharacterClear(TextBuffer& buffer)
    {
        const auto& input = g_App.GetInput();

        const auto& shiftAction = input.GetAction(In::Shift);
        const auto& ctrlAction  = input.GetAction(In::Ctrl);
        const auto& bsAction    = input.GetAction(In::Backspace);
        const auto& delAction   = input.GetAction(In::Delete);

        if (bsAction.IsHeld())
        {
            if (!buffer.Text.empty() && bsAction.IsPulsed(PULSE_DELAY_SEC, PULSE_INITIAL_DELAY_SEC))
            {
                PushUndo(buffer);

                // Erase selection.
                if (buffer.Selection.has_value() && ctrlAction.IsHeld())
                {
                    auto start = buffer.Text.begin() + buffer.Selection->first;
                    auto end   = buffer.Text.begin() + buffer.Selection->second;
                    buffer.Text.erase(start, end);

                    buffer.Cursor    = buffer.Selection->first;
                    buffer.Selection = std::nullopt;
                }
                // Erase back to start.
                else if (shiftAction.IsHeld() && ctrlAction.IsHeld())
                {
                    buffer.Text.erase(0, buffer.Cursor);
                    buffer.Cursor = 0;
                }
                // Erase back to previous space.
                else if (ctrlAction.IsHeld())
                {
                    uint spaceCount  = 0;
                    bool isCurSpace  = buffer.Text.at(buffer.Cursor - 1) == ' ';
                    bool isPrevSpace = isCurSpace;
                    while (buffer.Cursor > 0 &&
                            (isCurSpace == isPrevSpace ||                        // Word or trailing spaces.
                             ((!isCurSpace && isPrevSpace) && spaceCount == 1))) // Word with 1 trailing space after.
                    {
                        // Count trailing spaces.
                        if (isCurSpace)
                        {
                            spaceCount++;
                        }

                        buffer.Text.erase(buffer.Text.begin() + (buffer.Cursor - 1));

                        buffer.Cursor--;
                        if (buffer.Cursor > 0)
                        {
                            isPrevSpace = isCurSpace;
                            isCurSpace  = buffer.Text.at(buffer.Cursor - 1) == ' ';
                        }
                    }
                }
                // Erase back single character.
                else
                {
                    if (buffer.Cursor > 0)
                    {
                        buffer.Text.erase(buffer.Text.begin() + (buffer.Cursor - 1));
                        buffer.Cursor--;
                    }
                }
            }

            return true;
        }
        else if (!buffer.Text.empty() && delAction.IsHeld())
        {
            if (!buffer.Text.empty() && delAction.IsPulsed(PULSE_DELAY_SEC, PULSE_INITIAL_DELAY_SEC))
            {
                PushUndo(buffer);

                // Erase selection.
                if (buffer.Selection.has_value() && ctrlAction.IsHeld())
                {
                    auto start = buffer.Text.begin() + buffer.Selection->first;
                    auto end   = buffer.Text.begin() + buffer.Selection->second;
                    buffer.Text.erase(start, end);

                    buffer.Cursor    = buffer.Selection->first;
                    buffer.Selection = std::nullopt;
                }
                // Erase forward to end.
                else if (shiftAction.IsHeld() && ctrlAction.IsHeld())
                {
                    buffer.Text.erase(buffer.Cursor, buffer.Text.size() - buffer.Cursor);
                }
                // Erase forward to next space.
                else if (ctrlAction.IsHeld())
                {
                    uint spaceCount  = 0;
                    bool isCurSpace  = buffer.Text.at(buffer.Cursor) == ' ';
                    bool isPrevSpace = isCurSpace;
                    while (buffer.Cursor < buffer.Text.size() &&
                            (isCurSpace == isPrevSpace ||                        // Word or trailing spaces.
                             ((!isCurSpace && isPrevSpace) && spaceCount == 1))) // Word with 1 trailing space behind.
                    {
                        // Count trailing spaces.
                        if (isCurSpace)
                        {
                            spaceCount++;
                        }

                        buffer.Text.erase(buffer.Text.begin() + buffer.Cursor);

                        if (buffer.Cursor < buffer.Text.size())
                        {
                            isPrevSpace = isCurSpace;
                            isCurSpace  = buffer.Text.at(buffer.Cursor) == ' ';
                        }
                    }
                }
                // Erase forward single character.
                else
                {
                    if (buffer.Cursor < buffer.Text.size())
                    {
                        buffer.Text.erase(buffer.Text.begin() + buffer.Cursor);
                    }
                }
            }

            return true;
        }

        return false;
    }

    bool TextManager::HandleCursorSelection(TextBuffer& buffer)
    {
        const auto& input = g_App.GetInput();

        const auto& escAction   = input.GetAction(In::Escape);
        const auto& homeAction  = input.GetAction(In::Home);
        const auto& endAction   = input.GetAction(In::End);
        const auto& shiftAction = input.GetAction(In::Shift);
        const auto& ctrlAction  = input.GetAction(In::Ctrl);
        const auto& leftAction  = input.GetAction(In::ArrowLeft);
        const auto& rightAction = input.GetAction(In::ArrowRight);
        const auto& aAction     = input.GetAction(In::A);

        if (buffer.Text.empty())
        {
            return false;
        }

        // Select all.
        if (ctrlAction.IsHeld() && aAction.IsClicked())
        {
            buffer.Selection = std::pair(0, buffer.Text.size());
            buffer.Cursor    = buffer.Text.size();
            return true;
        }
        // Deselect all.
        else if (escAction.IsClicked())
        {
            buffer.Selection = std::nullopt;
            return true;
        }

        // TODO: Move up/down.

        // Move or select to start/end.
        if (homeAction.IsClicked() || endAction.IsClicked())
        {
            // To start.
            if (homeAction.IsClicked())
            {
                if (shiftAction.IsHeld())
                {
                    buffer.Selection = std::pair(0, buffer.Cursor);
                }

                buffer.Cursor = 0;
            }
            // To end.
            else if (endAction.IsClicked())
            {
                if (shiftAction.IsHeld())
                {
                    buffer.Selection = std::pair(buffer.Cursor, buffer.Text.size());
                }

                buffer.Cursor = buffer.Text.size();
            }

            return true;
        }

        // Move left/right.
        if (leftAction.IsHeld() || rightAction.IsHeld())
        {
            if (buffer.Cursor > 0 && leftAction.IsPulsed(PULSE_DELAY_SEC, PULSE_INITIAL_DELAY_SEC))
            {
                uint prevCursor = buffer.Cursor;

                // Move back to previous word.
                if (ctrlAction.IsHeld())
                {
                    uint prevCursor = buffer.Cursor;

                    // Skip spaces before word.
                    while (buffer.Cursor > 0 && buffer.Text[buffer.Cursor - 1] == ' ')
                    {
                        buffer.Cursor--;
                    }

                    // Skip word.
                    while (buffer.Cursor > 0 && buffer.Text[buffer.Cursor - 1] != ' ')
                    {
                        buffer.Cursor--;
                    }
                }
                // Move back to previous character.
                else
                {
                    buffer.Cursor--;
                }

                // Expand selection back.
                if (shiftAction.IsHeld())
                {
                    if (buffer.Selection.has_value())
                    {
                        buffer.Selection->first = buffer.Cursor;
                    }
                    else
                    {
                        buffer.Selection = std::pair(buffer.Cursor, prevCursor);
                    }
                }
            }
            else if (buffer.Cursor < buffer.Text.size() && rightAction.IsPulsed(PULSE_DELAY_SEC, PULSE_INITIAL_DELAY_SEC))
            {
                uint prevCursor = buffer.Cursor;

                // Move forward to next word.
                if (ctrlAction.IsHeld())
                {
                    // Skip current word.
                    while (buffer.Cursor < buffer.Text.size() && buffer.Text[buffer.Cursor] != ' ')
                    {
                        buffer.Cursor++;
                    }

                    // Skip spaces after word.
                    while (buffer.Cursor < buffer.Text.size() && buffer.Text[buffer.Cursor] == ' ')
                    {
                        buffer.Cursor++;
                    }
                }
                // More forward to next character.
                else
                {
                    buffer.Cursor++;
                }

                // Expand selection forward.
                if (shiftAction.IsHeld())
                {
                    if (buffer.Selection.has_value())
                    {
                        buffer.Selection->second = buffer.Cursor;
                    }
                    else
                    {
                        buffer.Selection = std::pair(prevCursor, buffer.Cursor);
                    }
                }
            }

            return true;
        }

        return false;
    }

    void TextManager::PushUndo(TextBuffer& buffer)
    {
        if (buffer.Undo.size() >= HISTORY_SIZE_MAX)
        {
            buffer.Undo.pop_front();
        }
        buffer.Undo.push_back(buffer.Text);

        buffer.Redo.clear();
    }

    void TextManager::UpdateLineStarts(TextBuffer& buffer)
    {
        buffer.LineStarts.clear();
        buffer.LineStarts.push_back(0);

        uint lineStart = 0;
        uint wordStart = 0;

        uint i = 0;
        while (i < buffer.Text.size())
        {
            // Track word boundaries.
            if (buffer.Text[i] == ' ' || buffer.Text[i] == '\n')
            {
                // Wrap before word.
                uint wordEnd = i;
                if ((wordEnd - lineStart) > buffer.LineWidthMax)
                {
                    buffer.LineStarts.push_back(wordStart);
                    lineStart = wordStart;
                }

                // Handle newline explicitly.
                if (buffer.Text[i] == '\n')
                {
                    buffer.LineStarts.push_back(i + 1);
                    lineStart = i + 1;
                }

                wordStart = i + 1;
            }

            i++;
        }

        // Final wrap if needed.
        if ((buffer.Text.size() - lineStart) > buffer.LineWidthMax)
        {
            buffer.LineStarts.push_back(wordStart);
        }
    }
}
