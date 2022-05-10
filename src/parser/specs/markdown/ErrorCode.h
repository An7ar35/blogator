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
            BLOCK_LINE_FEED_SEPARATOR_MISSING, //TODO ?
            INVALID_BLOCK_ID_NAME,
            INVALID_CHARACTER_IN_BLOCK_ID_NAME,
            UNEXPECTED_END_TO_BLOCK_ID_DECLARATION,
            EOF_IN_CODE_BLOCK,
            INVALID_WHITESPACE_IN_CODE_BLOCK_DECLARATION,
            INVALID_LANGUAGE_TAG_IN_CODE_BLOCK,
            INVALID_FORMAT_IN_CODE_BLOCK_DECLARATION,
            INLINED_CODE_BLOCK,


            /* MarkdownToHTML errors */
            UNDEFINED_HEADING,

            ENUM_END
        };

        static const std::string & str( int err );
        static const std::string & detailed( int err );
    };
}

#endif //BLOGATOR_PARSER_SPECS_MARKDOWN_ERRORCODE_H