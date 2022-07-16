#include "ErrorCode.h"

#include <array>

#include "../../../sys/terminate.h"

using namespace blogator::parser::specs::markdown;

struct Description {
    std::string text;
    std::string detailed;
};

/**
 * [STATIC/LOCAL] Error description loader
 * @return Array
 */
static std::array<Description, ErrorCode::ENUM_END> loadErrorStrings() noexcept {
    try {
        auto arr = std::array<Description, ErrorCode::ENUM_END>();

        arr[ ErrorCode::NONE ] = {
            "no error",
            "No error."
        };
        arr[ ErrorCode::UNKNOWN ] = {
            "unknown error code",
            "Error code has no description available - pls. check error code falls in the blogator::parser::specs::markdown::ErrorCode enum range."
        };
        arr[ ErrorCode::BLOCK_MISALIGNMENT ] = {
            "block misalignment",
            R"(This error occurs when a block's prefix is not aligned with the previous one.)"
        };
        arr[ ErrorCode::BLOCK_LINEFEED_SEPARATOR_MISSING ] = {
            "block line-feed separator missing",
            R"(This error occurs when 2 sequential blocks are not separated properly with 2 x LF ("\n\n").)"
        };
        arr[ ErrorCode::HORIZONTAL_RULE_NEW_LINE_SEPARATOR_MISSING ] = {
            "horizontal-rule new line separator missing",
            R"(This error occurs when there isn't at least a line separation between a horizontal rule and the next block.)"
        };
        arr[ ErrorCode::INLINE_CONTENT_AFTER_HEADING ] = {
            "inline content after heading",
            R"(This error occurs when content that is not a block ID is found after a heading is closed ('atx-style'). For example: "## heading ## content.)"
        };
        arr[ ErrorCode::INVALID_BLOCK_ID_NAME ] = {
            "invalid block id name",
            R"(This error occurs when a block id name is invalid (valid format: \"-?[_a-zA-Z]+[_a-zA-Z0-9-]*\"). The block ID declaration will be treated as normal text.)"
        };
        arr[ ErrorCode::INVALID_CHARACTER_IN_BLOCK_ID_NAME ] = {
            "invalid character in block id name",
            R"(This error occurs when an invalid character is encountered in a block ID name (valid format: \"-?[_a-zA-Z]+[_a-zA-Z0-9-]*\"). The block ID declaration will be treated as normal text.)"
        };
        arr[ ErrorCode::EMPTY_BLOCK ] = {
            "empty block",
            R"(This error occurs when a block is declared but has no content within.)"
        };
        arr[ ErrorCode::MISALIGNED_BLOCK_CONTENT ] = {
            "misaligned block content",
            R"(This error occurs when content inside a block is not aligned with the indentation of said block.)"
        };
        arr[ ErrorCode::EOF_IN_CODE_BLOCK ] = {
            "EOF in code-block",
            R"(This error occurs when end-of-file is reached inside a code block declaration.)"
        };
        arr[ ErrorCode::EOF_IN_BLOCKQUOTE ] = {
            "EOF in blockquote",
            R"(This error occurs when end-of-file is reached inside a blockquote declaration.)"
        };
        arr[ ErrorCode::EOF_IN_BLOCK_ID ] = {
            "EOF in block id",
            R"(This error occurs when end-of-file is reached inside a possible block ID.)"
        };
        arr[ ErrorCode::EOF_IN_HYPERLINK ] = {
            "EOF in hyperlink",
            R"(This error occurs when end-of-file reached inside a hyperlink URL declaration.)"
        };
        arr[ ErrorCode::EOF_IN_IMAGE ] = {
            "EOF in image",
            R"(This error occurs when end-of-file reached inside an image source declaration.)"
        };
        arr[ ErrorCode::INVALID_WHITESPACE_IN_CODE_BLOCK_DECLARATION ] = {
            "invalid whitespace in code-block declaration",
            R"(This error occurs when whitespace is found after opening the code-block. The format for a code-block should be: ```<?lang>\n ... \n```)"
        };
        arr[ ErrorCode::INVALID_LANGUAGE_TAG_IN_CODE_BLOCK ] = {
            "invalid language tag in code-block",
            R"(This error occurs when the language tag declared contain one or more invalid characters (tag should be solely composed of ascii alpha characters).)"
        };
        arr[ ErrorCode::INVALID_FORMAT_IN_CODE_BLOCK_DECLARATION ] = {
            "invalid format in code block declaration",
            R"(This error occurs when the format on the code block declaration line is incorrect. The format for a code-block should be: ```<?lang>\n ... \n```)"
        };
        arr[ ErrorCode::INLINED_CODE_BLOCK ] = {
            "inlined code-block",
            R"(This error occurs when a code block is opened and closed on the same line. The opening and closing declaration (```) should be at the start of separate lines with the code content between them.)"
        };
        arr[ ErrorCode::VOID_SQUARE_BRACKET_DEFINITION ] = {
            "void square bracket definition",
            R"(This error occurs when a hyperlink or footnote declaration is missing its description content. Accepted formats: [text](url) or [^0])"
        };
        arr[ ErrorCode::INVALID_SQUARE_BRACKET_DECLARATION_FORMAT ] = {
            "invalid square bracket declaration format",
            R"(This error occurs when a hyperlink/image/footnote declaration ("[..") is made but its format is not correct. The declaration is parsed as normal text in this event.)"
        };
        arr[ ErrorCode::UNDEFINED_IMAGE_ALT_TEXT ] = {
            "undefined image alt-text",
            R"(This error occurs when an image declaration is missing its alt-text. Expected format: ![alt-text](image.jpg))"
        };
        arr[ ErrorCode::UNDEFINED_IMAGE_SOURCE ] = {
            "undefined image source",
            R"(This error occurs when an image declaration is missing its source. Expected format: ![alt-text](image.jpg))"
        };
        arr[ ErrorCode::NEWLINE_IN_IMAGE_SOURCE ] = {
            "newline in image source",
            R"(This error occurs when an image source declaration is abruptly interrupted by a newline.)"
        };
        arr[ ErrorCode::UNDEFINED_HYPERLINK_TEXT ] = {
            "undefined hyperlink text",
            R"(This error occurs when a hyperlink declaration is missing its text. Expected format: [text](url))"
        };
        arr[ ErrorCode::UNDEFINED_HYPERLINK_URL ] = {
            "undefined hyperlink URL",
            R"(This error occurs when a hyperlink declaration is missing its URL. Expected format: [text](url))"
        };
        arr[ ErrorCode::NEWLINE_IN_HYPERLINK_URL ] = {
            "newline in hyperlink URL",
            R"(This error occurs when a hyperlink URL declaration is abruptly interrupted by a newline.)"
        };
        arr[ ErrorCode::MISMATCHED_LIST_PREFIX ] = {
            "mismatched list prefix",
            R"(This error occurs when a different prefix type from the first list item's is used. Consistency is preferred.)"
        };
        arr[ ErrorCode::TABLE_COLUMN_MISMATCH ] = {
            "table column mismatch",
            R"(This error occurs when the number of columns found in a row does not match the number of headings for the table. The table is parsed as a normal paragraph in this case.)"
        };
        arr[ ErrorCode::TABLE_COLUMN_DEFINITION_INVALID ] = {
            "table column definition invalid",
            R"(This error occurs when the definition of a column in the 2nd row is invalid. The table is parsed as a normal paragraph in this case. Expected format: "---"/":--"/"--:"/":-:" with, optionally, more hyphen characters.)"
        };
        arr[ ErrorCode::DEFINITION_LIST_EMPTY_DEFINITION ] = {
            "definition list empty definition",
            R"(This error occurs when a ": " starting definition in a definition list has no content before EOF or a newline.)"
        };

        arr[ ErrorCode::UNDEFINED_HEADING ] = {
            "undefined heading",
            R"(This error occurs when a heading block is declared without any text for the heading (e.g.: \"#  \n\").)"
        };

        return std::move( arr );

    } catch( ... ) {
        TERMINATE( "[parser::specs::markdown::ErrorCode::] Failed to load error data (loadErrorStrings)." );
    }
}

static std::array<Description, ErrorCode::ENUM_END> error_descriptions = loadErrorStrings();

/**
 * Gets the description string for an error code
 * @param err ::markdown::ErrorCode::* enum
 * @return Short description string
 */
const std::string & ErrorCode::str( int err ) {
    if( ( err = abs( err ) ) >= ENUM_END )
        return error_descriptions[ErrorCode::UNKNOWN].text;
    else
        return error_descriptions[err].text;
}

/**
 * Gets the detailed description for an error code
 * @param err ::markdown::ErrorCode::* enum
 * @return Long detailed description string
 */
const std::string & ErrorCode::detailed( int err ) {
    if( ( err = abs( err ) ) >= ENUM_END )
        return error_descriptions[ErrorCode::UNKNOWN].detailed;
    else
        return error_descriptions[err].detailed;
}