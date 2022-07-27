#ifndef BLOGATOR_PARSER_SPECS_MARKDOWN_ERRORCODE_H
#define BLOGATOR_PARSER_SPECS_MARKDOWN_ERRORCODE_H

#include <string>

namespace blogator::parser::specs::markdown {
    /**
     * Parsing error codes and descriptions
     */
    struct ErrorCode {
        enum {
            NONE    = 0,
            UNKNOWN = 1,

            /* Tokeniser errors */
            BLOCK_LINEFEED_SEPARATOR_MISSING,
            BLOCK_MISALIGNMENT,
            HORIZONTAL_RULE_NEW_LINE_SEPARATOR_MISSING,
            INLINE_CONTENT_AFTER_HEADING,
            BLOCK_ID_OVERRIDE,
            INVALID_BLOCK_ID_NAME,
            INVALID_CHARACTER_IN_BLOCK_ID_NAME,
            EMPTY_BLOCK,
            MISALIGNED_BLOCK_CONTENT,
            EOF_IN_CODE_BLOCK,
            EOF_IN_BLOCKQUOTE,
            EOF_IN_BLOCK_ID,
            EOF_IN_HYPERLINK,
            EOF_IN_IMAGE,
            INVALID_WHITESPACE_IN_CODE_BLOCK_DECLARATION,
            INVALID_LANGUAGE_TAG_IN_CODE_BLOCK,
            INVALID_FORMAT_IN_CODE_BLOCK_DECLARATION,
            INLINED_CODE_BLOCK,
            VOID_SQUARE_BRACKET_DEFINITION,
            INVALID_SQUARE_BRACKET_DECLARATION_FORMAT,
            UNDEFINED_IMAGE_ALT_TEXT,
            UNDEFINED_IMAGE_SOURCE,
            NEWLINE_IN_IMAGE_SOURCE,
            UNDEFINED_HYPERLINK_TEXT,
            UNDEFINED_HYPERLINK_URL,
            NEWLINE_IN_HYPERLINK_URL,
            MISMATCHED_LIST_PREFIX,
            TABLE_COLUMN_MISMATCH,
            TABLE_COLUMN_DEFINITION_INVALID,
            DEFINITION_LIST_EMPTY_DEFINITION,

            /* MarkdownToHTML errors */
            UNDEFINED_HEADING,
            INVALID_HEADING_LEVEL,
            ORPHANED_FOOTNOTE_REFERENCE,
            ORPHANED_FOOTNOTE_DEFINITION,

            ENUM_END
        };

        static const std::string & str( int err );
        static const std::string & detailed( int err );
    };
}

#endif //BLOGATOR_PARSER_SPECS_MARKDOWN_ERRORCODE_H