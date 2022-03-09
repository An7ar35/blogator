#include "ErrorCode.h"

#include <array>

#include "../../../logger/Logger.h"

using namespace blogator::parser::specs::native;

struct Description {
    std::string text;
    std::string detailed;
};

/**
 * [LOCAL] Loads description strings in a container
 * @param arr Container
 */
static std::array<Description, ErrorCode::ENUM_END> loadErrorStrings() noexcept {
    try {
        auto arr = std::array<Description, ErrorCode::ENUM_END>();

        arr[ErrorCode::NONE] = {
            "no error",
            "No error."
        };
        arr[ErrorCode::UNKNOWN] = {
            "unknown error code",
            "Error code has no description available - pls. check error code falls in the blogator::parser::specs::blogator::ErrorCode enum range."
        };
        arr[ErrorCode::INVALID_UTF8_CODEPOINT_START_BYTE] = {
            "invalid utf8 start byte",
            R"(This error occurs if the start byte of a utf8 code point byte sequence is in an invalid range (between 0x80 and 0xC0 or greater than 0xF7).)"
        };
        arr[ErrorCode::INVALID_UTF16_SURROGATE_PAIR] = {
            "invalid utf16 surrogate pair",
            R"(This error occurs if a valid high surrogate is followed by an invalid low surrogate code unit.)"
        };
        arr[ErrorCode::INCOMPLETE_UTF8_CODEPOINT_IN_INPUT_STREAM] = {
            "incomplete utf8 code point",
            R"(This error occurs if the input byte stream contains an incomplete UTF-8 codepoint as a result of abrupt read failure or premature EOF.)"
        };
        arr[ErrorCode::INCOMPLETE_UTF16_HIGH_SURROGATE_IN_INPUT_STREAM] = {
            "incomplete utf16 high surrogate code unit",
            R"(This error occurs if there is not enough bytes available to fully form a high surrogate.)"
        };
        arr[ErrorCode::INCOMPLETE_UTF16_CODEPOINT_IN_INPUT_STREAM] = {
            "incomplete utf16 code point",
            R"(This error occurs if the input byte stream contains an incomplete UTF-16 codepoint as a result of abrupt read failure or premature EOF.)"
        };
        arr[ErrorCode::INCOMPLETE_UTF32_CODEPOINT_IN_INPUT_STREAM] = {
            "incomplete utf32 code point",
            R"(This error occurs if the input byte stream contains an incomplete UTF-32 codepoint as a result of abrupt read failure or premature EOF.)"
        };
        arr[ErrorCode::NULL_CHARACTER] = {
            "Unexpected NULL character encountered",
            R"(This error occurs if text included a U+0000 character. The character will be replaced with U+FFFD.)"
        };
        arr[ErrorCode::TOKEN_OF_UNKNOWN_TYPE] = {
            "unknown token",
            R"(This error occurs if a token of unknown type is passed to the tree construction stage. The token will be ignored.)"
        };

        return std::move( arr );

    } catch( const std::exception &e ) {
        LOG_CRITICAL( "[parser::specs::native::ErrorCode::] Failed to load error data (loadErrorStrings): ", e.what() );
        std::terminate();
    }
}

static std::array<Description, ErrorCode::ENUM_END> error_descriptions = loadErrorStrings();

/**
 * Gets the description string for an error code
 * @param err ::blogator::ErrorCode::* enum
 * @return Short description string
 */
const std::string &blogator::parser::specs::native::ErrorCode::str( int err ) {
    if( ( err = abs( err ) ) >= ENUM_END )
        return error_descriptions[ErrorCode::UNKNOWN].text;
    else
        return error_descriptions[err].text;
}

/**
 * Gets the detailed description for an error code
 * @param err ::blogator::ErrorCode::* enum
 * @return Long detailed description string
 */
const std::string &blogator::parser::specs::native::ErrorCode::detailed( int err ) {
    if( ( err = abs( err ) ) >= ENUM_END )
        return error_descriptions[ErrorCode::UNKNOWN].detailed;
    else
        return error_descriptions[err].detailed;
}
