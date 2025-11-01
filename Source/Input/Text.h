#pragma once

#include "Input/Action.h"

namespace Silent::Input
{
    /** @brief Text block snapshot. */
    struct TextSnapshot
    {

        std::string                          Text      = {};
        std::optional<std::pair<uint, uint>> Selection = {}; // First = selection start, second = selection end.
        uint                                 Cursor    = 0;
    };

    /** @brief Text block buffer. */
    struct TextBuffer
    {
        TextSnapshot             Snapshot = {};
        std::deque<TextSnapshot> Undo     = {};
        std::deque<TextSnapshot> Redo     = {};

        std::vector<uint> LineStarts   = {};
        uint              LineWidthMax = 0;
        uint              CharCountMax = 0;
    };

    /** @brief Text block manager. */
    class TextManager
    {
    private:
        // ==========
        // Constants
        // ==========

        static constexpr float PULSE_DELAY_SEC         = 0.025f;
        static constexpr float PULSE_INITIAL_DELAY_SEC = 0.3f;
        static constexpr uint  HISTORY_SIZE_MAX        = 128;

        // =======
        // Fields
        // =======

        std::unordered_map<std::string, TextBuffer> _buffers       = {}; /** Key = text block ID, value = text buffer. */
        std::string                                 _clipboard     = {};
        std::vector<ActionId>                       _prevActionIds = {};

    public:
        // =============
        // Constructors
        // =============

        /** @brief Constructs a default `TextManager`. */
        TextManager() = default;

        // ========
        // Getters
        // ========

        /** @brief Gets a text block from a buffer.
         *
         * @param bufferId Text buffer ID.
         * @return Text block.
         */
        const std::string& GetText(const std::string& bufferId) const;

        /** @brief Gets a text block from a buffer split into lines.
         *
         * @param bufferId Text buffer ID.
         * @param low @todo
         * @param high @todo
         * @return Text block split into lines.
         */
        std::vector<std::string> GetTextLines(const std::string& bufferId, uint low, uint high) const;

        /** @brief Gets the position of the text block cursor from a buffer.
         *
         * @param bufferId Text buffer ID.
         * @return Text block cursor position.
         */
        uint GetCursorPosition(const std::string& bufferId) const;

        // ==========
        // Utilities
        // ==========

        void InsertBuffer(const std::string& bufferId, uint lineWidthMax, uint charCountMax);
        void UpdateBuffer(const std::string& bufferId);
        void RemoveBuffer(const std::string& bufferId);

    private:
        // ========
        // Helpers
        // ========

        bool HandleHistory(TextBuffer& buffer);
        bool HandleClipboard(TextBuffer& buffer);
        bool HandleCharacterAdd(TextBuffer& buffer);
        bool HandleCharacterClear(TextBuffer& buffer);
        bool HandleCursorSelection(TextBuffer& buffer);

        void PushUndo(TextBuffer& buffer);
        void UpdateLineStarts(TextBuffer& buffer);
    };
}
