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
        arr[ ErrorCode::BLOCK_NEW_LINE_SEPARATOR_MISSING ] = {
            "block line-feed separator missing",
            R"(This error occurs when 2 sequential blocks are not separated properly with 2 x LF ("\n\n").)"
        };
        arr[ ErrorCode::INVALID_BLOCK_ID_NAME ] = {
            "invalid block id name",
            R"(This error occurs when a block id name is invalid (valid format: \"-?[_a-zA-Z]+[_a-zA-Z0-9-]*\"). The block ID declaration will be treated as normal text.)"
        };
        arr[ ErrorCode::INVALID_CHARACTER_IN_BLOCK_ID_NAME ] = {
            "invalid character in block id name",
            R"(This error occurs when an invalid character is encountered in a block ID name (valid format: \"-?[_a-zA-Z]+[_a-zA-Z0-9-]*\"). The block ID declaration will be treated as normal text.)"
        };
        arr[ ErrorCode::UNEXPECTED_END_TO_BLOCK_ID_DECLARATION ] = {
            "unexpected end to block id declaration",
            R"(This error occurs when a block id declaration does not have a closing '}' after the ID name. The block ID declaration will be treated as normal text.)"
        };
        arr[ ErrorCode::EOF_IN_CODE_BLOCK ] = {
            "EOF in code-block",
            R"(This error occurs when end-of-file is within a code block declaration.)"
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
        arr[ ErrorCode::UNDEFINED_IMAGE_ALT_TEXT ] = {
            "undefined image alt-text",
            R"(This error occurs when an image declaration is missing its alt-text. Expected format: ![alt-text](image.jpg))"
        };
        arr[ ErrorCode::UNDEFINED_IMAGE_SOURCE ] = {
            "undefined image source",
            R"(This error occurs when an image declaration is missing its source. Expected format: ![alt-text](image.jpg))"
        };
        arr[ ErrorCode::INVALID_IMAGE_DECLARATION_FORMAT ] = {
            "invalid image declaration format",
            R"(This error occurs when an image declaration ("![..") is made but its format is not correct. The declaration is parsed as normal text in this event. Expected format: ![alt-text](image.jpg))"
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