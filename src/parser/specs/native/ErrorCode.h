#ifndef BLOGATOR_PARSER_SPECS_NATIVE_ERRORCODE_H
#define BLOGATOR_PARSER_SPECS_NATIVE_ERRORCODE_H

#include <string>

namespace blogator::parser::specs::native {
    class ErrorCode {
      public:
        enum {
            NONE    = 0,
            UNKNOWN = 1,
            TOKEN_OF_UNKNOWN_TYPE,
            ENUM_END
        };

        static const std::string & str( int err );
        static const std::string & detailed( int err );
    };
}

#endif //BLOGATOR_PARSER_SPECS_NATIVE_ERRORCODE_H
