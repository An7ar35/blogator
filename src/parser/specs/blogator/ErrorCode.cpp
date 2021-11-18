#include "ErrorCode.h"

#include <array>

using namespace blogator::parser::specs::blogator;

struct Description {
    std::string text;
    std::string detailed;
};

static bool                                         error_desc_loaded = false;
static std::array<Description, ErrorCode::ENUM_END> error_descriptions;

/**
 * Loads description strings in a container
 * @param arr Container
 */
static void loadErrorStrings( std::array<Description, ErrorCode::ENUM_END> &arr ) {
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
    arr[ErrorCode::INCOMPLETE_UTF8_CODEPOINT_IN_INPUT_STREAM] = {
        "incomplete utf8 code point",
        R"(This error occurs if the input byte stream contains an incomplete UTF-8 codepoint as a result of abrupt read failure or premature EOF.)"
    };
    arr[ErrorCode::INCOMPLETE_UTF16_CODEPOINT_IN_INPUT_STREAM] = {
        "incomplete utf16 code point",
        R"(This error occurs if the input byte stream contains an incomplete UTF-16 codepoint as a result of abrupt read failure or premature EOF.)"
    };
    arr[ErrorCode::INCOMPLETE_UTF32_CODEPOINT_IN_INPUT_STREAM] = {
        "incomplete utf32 code point",
        R"(This error occurs if the input byte stream contains an incomplete UTF-32 codepoint as a result of abrupt read failure or premature EOF.)"
    };

    //TODO log debug
    error_desc_loaded = true;
}

/**
 * Gets the description string for an error code
 * @param err ::blogator::ErrorCode::* enum
 * @return Short description string
 */
const std::string &blogator::parser::specs::blogator::ErrorCode::str( int err ) {
    if( !error_desc_loaded )
        loadErrorStrings( error_descriptions );

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
const std::string &blogator::parser::specs::blogator::ErrorCode::detailed( int err ) {
    if( !error_desc_loaded )
        loadErrorStrings( error_descriptions );

    if( ( err = abs( err ) ) >= ENUM_END )
        return error_descriptions[ErrorCode::UNKNOWN].detailed;
    else
        return error_descriptions[err].detailed;
}
