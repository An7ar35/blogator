#ifndef BLOGATOR_PARSER_SPECS_HTML5_ERRORCODE_H
#define BLOGATOR_PARSER_SPECS_HTML5_ERRORCODE_H

#include <string>

namespace blogator::parser::specs::infra {
    /**
     * Parsing error codes and descriptions
     * Lifted from: https://html.spec.whatwg.org/multipage/parsing.html#parse-errors
     */
    class ErrorCode {
      public:
        enum {
            NONE    = 0,
            UNKNOWN = 1,

            /* from the standard (used mostly in tokenizer) */
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

            /* extras (used exclusively in the tree constructor )*/
            INVALID_DOCTYPE_TOKEN_IN_FOREIGN_CONTENT,
            INVALID_STARTTAG_TOKEN_IN_FOREIGN_CONTENT,
            INVALID_ENDTAG_TOKEN_IN_FOREIGN_CONTENT,
            MISMATCHING_ENDTAG_IN_FOREIGN_CONTENT,
            MISSING_DOCTYPE,
            MALFORMED_DOCTYPE,
            MALFORMED_TAG_IMAGE,
            MALFORMED_TAG_BR,
            UNEXPECTED_TOKEN_DISCARDED,
            UNEXPECTED_TOKEN_FORWARDED,
            UNEXPECTED_CHARACTER,
            UNEXPECTED_DOCTYPE,
            UNEXPECTED_STARTTAG,
            UNEXPECTED_ENDTAG,
            UNEXPECTED_ENDTAG_SPECIAL_ELEMENT,
            UNEXPECTED_EOF,
            UNEXPECTED_EOF_IN_TEMPLATE,
            ORPHANED_STARTTAG,
            ORPHANED_ENDTAG,
            ORPHANED_ENDTAG_IN_SCOPE,
            UNCLOSED_TAG,
            INVALID_NESTING_OF_ELEMENT,
            INVALID_NESTING_OF_FORMATTING_ELEMENT,
            INVALID_END_TAG_IN_CURRENT_INSERTION_MODE,
            INVALID_ELEMENT_IN_FRAGMENT_CASE,
            ADOPTION_AGENCY_ALGO_4_4,
            ADOPTION_AGENCY_ALGO_4_5,
            ADOPTION_AGENCY_ALGO_4_6,
            FOSTER_PARENTING_TOKEN,
            FOSTER_PARENTING_TEXT,

            ENUM_END
        };

        static const std::string & str( int err );
        static const std::string & detailed( int err );
    };
}

#endif //BLOGATOR_PARSER_SPECS_HTML5_ERRORCODE_H
