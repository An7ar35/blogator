#include "ErrorCode.h"

#include <array>

#include "../../../sys/terminate.h"

using namespace blogator::configuration::parser::specs;

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
            "Error code has no description available - pls. check error code falls in the blogator::configuration::parser::specs::ErrorCode enum range."
        };
        arr[ ErrorCode::SYNTAX_ERROR ] = {
            "syntax error",
            R"(This error occurs when a syntax is malformed/unrecognised.)"
        };
        arr[ ErrorCode::INVALID_NAMESPACE_SEPARATOR ] = {
            "invalid namespace separator format",
            R"(This error occurs when 2+ colons are used as a namespace separator in a key. Only 1 colon ':' is required.")"
        };
        arr[ ErrorCode::INVALID_CHARACTER_IN_KEY ] = {
            "invalid character in key",
            R"(This error occurs when a key has an invalid character.)"
        };
        arr[ ErrorCode::INVALID_VALUE_NAME_FORMAT ] = {
            "invalid value name format",
            R"(This error occurs when a named value is incorrectly formatted.)"
        };
        arr[ ErrorCode::EOF_IN_COMMENT_BLOCK ] = {
            "EOF in comment block",
            R"(This error occurs when EOF is encountered inside a block comment. Block comments need to be closed: "/* ... */" )"
        };
        arr[ ErrorCode::EOF_IN_STRING_LITERAL ] = {
            "EOF in string literal",
            R"(This error occurs when EOF is encountered inside a string literal. I.e.: no closing '"' char was found prior EOF.)"
        };
        arr[ ErrorCode::ASYMMETRIC_VALUE_NESTING ] = {
            "asymmetric value nesting",
            R"(This error occurs when the number of curly brackets opened does not match their closing.)"
        };
        arr[ ErrorCode::EOF_IN_VALUE ] = {
            "EOF in value",
            R"(This error occurs when EOF is encountered without a semi-colon terminator on a key-value declaration.)"
        };
        return std::move( arr );

    } catch( ... ) {
        TERMINATE( "[configuration::parser::specs::ErrorCode::] Failed to load error data (loadErrorStrings)." );
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

