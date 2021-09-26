#ifndef BLOGATOR_PARSER_SPECS_HTML5_ERRORCODE_H
#define BLOGATOR_PARSER_SPECS_HTML5_ERRORCODE_H

#include <string>
#include <array>

namespace blogator::parser::specs::html5 {
    /**
     * Parsing error codes and descriptions
     * Lifted from: https://html.spec.whatwg.org/multipage/parsing.html#parse-errors
     */
    class ErrorCode {
      public:
        enum {
            NONE = 0,
            UNKNOWN,
            ABRUPT_CLOSING_OF_EMPTY_COMMENT,
            ABRUPT_DOCTYPE_PUBLIC_IDENTIFIER,
            ABRUPT_DOCTYPE_SYSTEM_IDENTIFIER,
            ABSENCE_OF_DIGITS_IN_NUMERIC_CHARACTER_REFERENCE,
            CDATA_IN_HTML_CONTENT,
            CHARACTER_REFERENCE_OUTSIDE_UNICODE_RANGE,
            CONTROL_CHARACTER_IN_INPUT_STREAM,
            CONTROL_CHARACTER_REFERENCE,
            END_TAG_WITH_ATTRIBUTES,
            DUPLICATE_ATTRIBUTE,
            END_TAG_WITH_TRAILING_SOLIDUS,
            EOF_BEFORE_TAG_NAME,
            EOF_IN_CDATA,
            EOF_IN_COMMENT,
            EOF_IN_DOCTYPE,
            EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT,
            EOF_IN_TAG,
            INCORRECTLY_CLOSED_COMMENT,
            INCORRECTLY_OPENED_COMMENT,
            INVALID_CHARACTER_SEQUENCE_AFTER_DOCTYPE_NAME,
            INVALID_FIRST_CHARACTER_OF_TAG_NAME,
            MISSING_ATTRIBUTE_VALUE,
            MISSING_DOCTYPE_NAME,
            MISSING_DOCTYPE_PUBLIC_IDENTIFIER,
            MISSING_DOCTYPE_SYSTEM_IDENTIFIER,
            MISSING_END_TAG_NAME,
            MISSING_QUOTE_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER,
            MISSING_QUOTE_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER,
            MISSING_SEMICOLON_AFTER_CHARACTER_REFERENCE,
            MISSING_WHITESPACE_AFTER_DOCTYPE_PUBLIC_KEYWORD,
            MISSING_WHITESPACE_AFTER_DOCTYPE_SYSTEM_KEYWORD,
            MISSING_WHITESPACE_BEFORE_DOCTYPE_NAME,
            MISSING_WHITESPACE_BETWEEN_ATTRIBUTES,
            MISSING_WHITESPACE_BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS,
            NESTED_COMMENT,
            NONCHARACTER_CHARACTER_REFERENCE,
            NONCHARACTER_IN_INPUT_STREAM,
            NON_VOID_HTML_ELEMENT_START_TAG_WITH_TRAILING_SOLIDUS,
            NULL_CHARACTER_REFERENCE,
            SURROGATE_CHARACTER_REFERENCE,
            SURROGATE_IN_INPUT_STREAM,
            UNEXPECTED_CHARACTER_AFTER_DOCTYPE_SYSTEM_IDENTIFIER,
            UNEXPECTED_CHARACTER_IN_ATTRIBUTE_NAME,
            UNEXPECTED_CHARACTER_IN_UNQUOTED_ATTRIBUTE_VALUE,
            UNEXPECTED_EQUALS_SIGN_BEFORE_ATTRIBUTE_NAME,
            UNEXPECTED_NULL_CHARACTER,
            UNEXPECTED_QUESTION_MARK_INSTEAD_OF_TAG_NAME,
            UNEXPECTED_SOLIDUS_IN_TAG,
            UNKNOWN_NAMED_CHARACTER_REFERENCE,

            ENUM_END
        };

        static const std::string & str( int err );
        static const std::string & detailed( int err );

      private:
        struct Description {
            std::string text;
            std::string detailed;
        };

        static bool                              _error_desc_loaded;
        static std::array<Description, ENUM_END> _descriptions;

        static void loadErrorStrings( std::array<Description, ENUM_END> &arr );
    };
}

#endif //BLOGATOR_PARSER_SPECS_HTML5_ERRORCODE_H