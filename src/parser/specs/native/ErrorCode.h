#ifndef BLOGATOR_PARSER_SPECS_NATIVE_ERRORCODE_H
#define BLOGATOR_PARSER_SPECS_NATIVE_ERRORCODE_H

#include <string>

namespace blogator::parser::specs::native {
    class ErrorCode {
      public:
        enum {
            NONE    = 0,
            UNKNOWN = 1,
            INVALID_UTF8_CODEPOINT_START_BYTE,
            INVALID_UTF16_SURROGATE_PAIR,
            INCOMPLETE_UTF8_CODEPOINT_IN_INPUT_STREAM,
            INCOMPLETE_UTF16_HIGH_SURROGATE_IN_INPUT_STREAM,
            INCOMPLETE_UTF16_CODEPOINT_IN_INPUT_STREAM,
            INCOMPLETE_UTF32_CODEPOINT_IN_INPUT_STREAM,

            ENUM_END
        };

        static const std::string & str( int err );
        static const std::string & detailed( int err );
    };
}

#endif //BLOGATOR_PARSER_SPECS_NATIVE_ERRORCODE_H
