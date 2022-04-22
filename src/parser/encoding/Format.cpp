#include "Format.h"

#include <sstream>

/**
 * Output stream operator
 * @param os Output stream
 * @param fmt Format enum
 * @return Output stream
 */
std::ostream & blogator::parser::encoding::operator <<( std::ostream &os, blogator::parser::encoding::Format fmt ) {
    switch( fmt ) {
        case Format::UNKNOWN:  { os << "UNKNOWN";  } break;
        case Format::UTF8:     { os << "UTF-8";    } break;
        case Format::UTF16_BE: { os << "UTF-16BE"; } break;
        case Format::UTF16_LE: { os << "UTF-16LE"; } break;
        case Format::UTF32_BE: { os << "UTF-32BE"; } break;
        case Format::UTF32_LE: { os << "UTF-32LE"; } break;
        default:               { os << "enum string not found"; }
    }

    return os;
}

/**
 * Converts a blogator::parser::encoding::Format enum to a string representation
 * @param fmt Format enum
 * @return String representation
 */
std::string blogator::to_string( blogator::parser::encoding::Format fmt ) {
    std::stringstream ss;
    ss << fmt;
    return ss.str();
}

/**
 * Converts a blogator::parser::encoding::Format enum to a string representation
 * @param content_type Format enum
 * @return String representation
 */
std::u32string blogator::to_u32string( blogator::parser::encoding::Format fmt ) {
    auto u8str = to_string( fmt );
    return { u8str.cbegin(), u8str.cend() };
}
