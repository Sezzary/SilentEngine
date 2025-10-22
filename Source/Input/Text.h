#pragma once

#include "Input/Action.h"

namespace Silent::Input
{
    struct TextSnapshot
    {

        std::string                          Text      = {};
        std::optional<std::pair<uint, uint>> Selection = {}; // First = selection start, second = selection end.
        uint                                 Cursor    = 0;
    };

    struct TextBuffer
    {
        TextSnapshot             Snapshot = {};
        std::deque<TextSnapshot> Undo     = {};
        std::deque<TextSnapshot> Redo     = {};

        std::vector<uint> LineStarts   = {};
        uint              LineWidthMax = 0;
        uint              CharCountMax = 0;
    };

    class TextManager
    {
    private:
        // Constants

        static constexpr float PULSE_DELAY_SEC         = 0.025f;
        static constexpr float PULSE_INITIAL_DELAY_SEC = 0.3f;
        static constexpr uint  HISTORY_SIZE_MAX        = 128;

        // Fields

        std::unordered_map<std::string, TextBuffer> _buffers       = {}; // Key = ID, value = text buffer.
        std::string                                 _clipboard     = {};
        std::vector<ActionId>                       _prevActionIds = {};

    public:
        // Constructors

        TextManager() = default;

        // Getters

        const std::string&       GetText(const std::string& bufferId) const;
        std::vector<std::string> GetTextLines(const std::string& bufferId, uint low, uint high) const;
        uint                     GetCursorPosition(const std::string& bufferId) const;
        
        // Utilities

        void InsertBuffer(const std::string& bufferId, uint lineWidthMax, uint charCountMax);
        void UpdateBuffer(const std::string& bufferId);
        void RemoveBuffer(const std::string& bufferId);

    private:
        // Helpers

        bool HandleHistory(TextBuffer& buffer);
        bool HandleClipboard(TextBuffer& buffer);
        bool HandleCharacterAdd(TextBuffer& buffer);
        bool HandleCharacterClear(TextBuffer& buffer);
        bool HandleCursorSelection(TextBuffer& buffer);

        void PushUndo(TextBuffer& buffer);
        void UpdateLineStarts(TextBuffer& buffer);
    };
}
