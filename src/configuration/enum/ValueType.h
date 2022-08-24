#ifndef BLOGATOR_CONFIGURATION_ENUM_VALUETYPE_H
#define BLOGATOR_CONFIGURATION_ENUM_VALUETYPE_H

#include <ostream>

namespace blogator::configuration {
    /**
     * Configuration value type in key-value pairs
     */
    enum class ValueType {
        NAME = 0,
        BOOLEAN,
        INTEGER,
        FLOAT,
        STRING,
    };

    std::ostream & operator <<( std::ostream & os, ValueType type );
}

namespace blogator {
    std::string to_string( blogator::configuration::ValueType type );
}

#endif //BLOGATOR_CONFIGURATION_ENUM_VALUETYPE_H