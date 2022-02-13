#include "ContentType.h"

#include <sstream>

/**
 * Output stream operator
 * @param os Output stream
 * @param content_type ContentType enum
 * @return Output stream
 */
std::ostream & blogator::parser::specs::infra::operator <<( std::ostream &os, blogator::parser::specs::infra::ContentType content_type ) {
    switch( content_type ) {
        case ContentType::IMAGE_SVG_XML:         { os << "image/svg+xml";         } break;
        case ContentType::APPLICATION_XHTML_XML: { os << "application/xhtml+xml"; } break;
        case ContentType::APPLICATION_RSS_XML:   { os << "application/rss+xml";   } break;
        case ContentType::APPLICATION_XML:       [[fallthrough]];
        default:                                 { os << "application/xml";       } break;
    }

    return os;
}

/**
 * Converts a parser::specs::infra::ContentType enum to a string representation
 * @param content_type ContentType enum
 * @return String representation
 */
std::string blogator::to_string( blogator::parser::specs::infra::ContentType content_type ) {
    std::stringstream ss;
    ss << content_type;
    return ss.str();
}

/**
 * Converts a parser::specs::infra::ContentType enum to a string representation
 * @param content_type ContentType enum
 * @return String representation
 */
std::u32string blogator::to_u32string( blogator::parser::specs::infra::ContentType content_type ) {
    auto u8str = to_string( content_type );
    return { u8str.cbegin(), u8str.cend() };
}
