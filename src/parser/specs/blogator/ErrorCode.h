#ifndef BLOGATOR_PARSER_SPECS_BLOGATOR_ERRORCODE_H
#define BLOGATOR_PARSER_SPECS_BLOGATOR_ERRORCODE_H

#include <string>

namespace blogator::parser::specs::blogator {
    class ErrorCode {
      public:
        enum {
            NONE    = 0,
            UNKNOWN = 1,
            INVALID_UTF8_START_BYTE,
            INCOMPLETE_UTF8_CODEPOINT_IN_INPUT_STREAM,
            INCOMPLETE_UTF32_CODEPOINT_IN_INPUT_STREAM,



            ENUM_END
        };

        static const std::string & str( int err );
        static const std::string & detailed( int err );
    };
}

#endif //BLOGATOR_PARSER_SPECS_BLOGATOR_ERRORCODE_H
