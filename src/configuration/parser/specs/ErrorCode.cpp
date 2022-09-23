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
            R"(This error occurs when the number of curly brackets opened does not match the number closed (opened: +1, closed: -1).)"
        };
        arr[ ErrorCode::EOF_IN_VALUE ] = {
            "EOF in value",
            R"(This error occurs when EOF is encountered without a semi-colon terminator on a key-value declaration.)"
        };
        arr[ ErrorCode::UNSPECIFIED_KEY ] = {
            "unspecified key",+
            R"(This error occurs when no key has been given.)"
        };
        arr[ ErrorCode::ROOT_IN_SCOPED_KEY ] = {
            "root in scoped key",
            R"(This error occurs when a root key (i.e.: with a ':' prefix) is declared inside a scoped namespace.)"
        };
        arr[ ErrorCode::INVALID_CONFIG_FORMAT ] = {
            "invalid configuration format",
            R"(This error occurs when the configuration format is incorrect (generic error).)"
        };
        arr[ ErrorCode::INVALID_NAMESPACE_FORMAT ] = {
            "invalid namespace format",
            R"(This error occurs when the format of a namespace is incorrect.)"
        };
        arr[ ErrorCode::INVALID_KEY_FORMAT ] = {
            "invalid key format",
            R"(This error occurs when the format of a key or value name is incorrect.)"
        };
        arr[ ErrorCode::INVALID_VALUE_FORMAT ] = {
            "invalid value format",
            R"(This error occurs when the format of a value is incorrect.)"
        };
        arr[ ErrorCode::UNRESOLVED_NAMED_VALUE ] = {
            "unresolved named value",
            R"(This error occurs when a cached named value could not be resolved to an existing namespaced key.)"
        };
        arr[ ErrorCode::MISSING_ASSIGNMENT_OPERATOR ] = {
            "missing assignment operator",
            R"(This error occurs when a scope is opened in a key declaration instead of after the assignment operator '='. )"
        };
        arr[ ErrorCode::MISSING_TERMINATOR ] = {
            "missing terminator",
            R"(This error occurs when a key-value is missing its ';' terminator at the end.)"
        };
        arr[ ErrorCode::MISSING_LIST_DECLARATION ] = {
            "missing list declaration",
            R"(This error occurs when a list is detected but the list declaration ("[...]") is missing.)"
        };
        arr[ ErrorCode::MISSING_LIST_SEPARATOR ] = {
            "missing list separator",
            R"(This error occurs when a sequence of values in a list is missing comma separator(s) between.)"
        };
        arr[ ErrorCode::UNCLOSED_BRACKET ] = {
            "unclosed bracket",
            R"(This error occurs when a bracket was opened but not closed.)"
        };
        arr[ ErrorCode::UNOPENED_BRACKET ] = {
            "unopened bracket",
            R"(This error occurs when a bracket is being closed without an opened matching.)"
        };
        arr[ ErrorCode::VALUE_OVERRIDE ] = {
            "value override",
            R"(This error occurs when a already existing key with a atomic value is being overridden.)"
        };
        arr[ ErrorCode::EOF_IN_KEY ] = {
            "EOF in key",
            R"(This error occurs when EOF is encountered before the current KV and any opened scopes conclude.)"
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

