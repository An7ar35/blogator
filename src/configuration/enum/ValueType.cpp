#include "ValueType.h"

#include <sstream>

/**
 * Output stream operator
 * @param os Output stream
 * @param type ValueType enum
 * @return Output stream
 */
std::ostream & blogator::configuration::operator <<( std::ostream &os, blogator::configuration::ValueType type ) {
    switch( type ) {
        case ValueType::NAME:    { os << "NAME";                  } break;
        case ValueType::BOOLEAN: { os << "BOOLEAN";               } break;
        case ValueType::INTEGER: { os << "INTEGER";               } break;
        case ValueType::FLOAT:   { os << "FLOAT";                 } break;
        case ValueType::STRING:  { os << "STRING";                } break;
        default:                 { os << "enum string not found"; } break;
    }

    return os;
}

/**
 * Converts a configuration::ValueType enum to a string representation
 * @param type ValueType enum
 * @return String representation
 */
std::string blogator::to_string( blogator::configuration::ValueType type ) {
    std::stringstream ss;
    ss << type;
    return ss.str();
}