#ifndef BLOGATOR_PARSER_SPECS_NATIVE_INSERTIONMODE_H
#define BLOGATOR_PARSER_SPECS_NATIVE_INSERTIONMODE_H

#include <string>
#include <ostream>

namespace blogator::parser::specs::native {
    enum class InsertionMode {
        INITIAL = 0,
        PRE_NAMESPACE,
        IN_NAMESPACE, //{{..}}
        POST_NAMESPACE,

        ENUM_END = POST_NAMESPACE, //used for iterating enums
    };

    std::ostream & operator <<( std::ostream &os, InsertionMode mode );
}

namespace blogator {
    std::string to_string( blogator::parser::specs::native::InsertionMode mode );
}

#endif //BLOGATOR_PARSER_SPECS_NATIVE_INSERTIONMODE_H
