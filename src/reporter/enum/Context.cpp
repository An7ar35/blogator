#include "Context.h"

#include "../../logger/Logger.h"

/**
 * Output stream operator
 * @param os Output stream
 * @param ctx Context enum
 * @return Output stream
 */
std::ostream & blogator::reporter::operator <<( std::ostream &os, blogator::reporter::Context ctx ) {
    switch( ctx ) {
        case Context::UNKNOWN:  { os << "Unknown";  } break;
        case Context::ENCODING: { os << "Encoding"; } break;
        case Context::NATIVE:   { os << "Blogator"; } break;
        case Context::HTML5:    { os << "HTML";     } break;
        case Context::MARKDOWN: { os << "Markdown"; } break;
        default:
            LOG_ERROR( "[blogator::reporter::ctxToStr( ", static_cast<int>( ctx ), " )] "
                       "Missing string representation for enum." );

            os << "enum string not found";
    }

    return os;
}

/**
 * Converts a reporter::Context enum to a string representation
 * @param ctx Context enum
 * @return String representation
 */
std::string blogator::to_string( blogator::reporter::Context ctx ) {
    std::stringstream ss;
    ss << ctx;
    return ss.str();
}
