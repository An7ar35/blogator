#ifndef BLOGATOR_CONFIGURATION_PARSER_SPECS_INSERTIONMODE_H
#define BLOGATOR_CONFIGURATION_PARSER_SPECS_INSERTIONMODE_H

#include <string>
#include <ostream>

namespace blogator::configuration::parser::specs {
    enum class InsertionMode {
        ENUM_BEGIN = 0, //used for iterating enums

        BEFORE_KEY = ENUM_BEGIN,
        IN_KEY,
        AFTER_KEY,
        BEFORE_VALUE,
        IN_VALUE,
        AFTER_VALUE,
        END_OF_FILE,

        ENUM_END = END_OF_FILE, //used for iterating enums
    };

    std::ostream & operator <<( std::ostream & os, InsertionMode mode );
}

namespace blogator {
    std::string to_string( configuration::parser::specs::InsertionMode mode );
}

#endif //BLOGATOR_CONFIGURATION_PARSER_SPECS_INSERTIONMODE_H