#ifndef BLOGATOR_REPORTER_CONTEXT_H
#define BLOGATOR_REPORTER_CONTEXT_H

#include <string>
#include <ostream>

namespace blogator::reporter {
    enum class Context {
        UNKNOWN = 0,
        ENCODING,
        CONFIGURATION,
        NATIVE,
        HTML5,
        MARKDOWN,

        ENUM_END = MARKDOWN //used for iterating enum
    };

    std::ostream & operator <<( std::ostream &os, Context ctx );
}

namespace blogator {
    std::string to_string( blogator::reporter::Context ctx );
}

#endif //BLOGATOR_REPORTER_CONTEXT_H