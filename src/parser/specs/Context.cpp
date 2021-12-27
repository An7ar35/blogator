#include "Context.h"

#include "../../logger/Logger.h"

/**
 * Output stream operator
 * @param os Output stream
 * @param ctx Context enum
 * @return Output stream
 */
std::ostream & blogator::parser::specs::operator <<( std::ostream &os, blogator::parser::specs::Context ctx ) {
    switch( ctx ) {
        case Context::UNKNOWN:  { os << "Unknown";  } break;
        case Context::BLOGATOR: { os << "Blogator"; } break;
        case Context::HTML5:    { os << "HTML5";    } break;
        case Context::MARKDOWN: { os << "Markdown"; } break;
        default:
            LOG_ERROR( "[blogator::parser::specs::ctxToStr( ", static_cast<int>( ctx ), " )] "
                       "Missing string representation for enum." );

            os << "enum string not found";
    }

    return os;
}

/**
 * Converts a parser::specs::Context enum to a string representation
 * @param ctx Context enum
 * @return String representation
 */
std::string blogator::to_string( blogator::parser::specs::Context ctx ) {
    std::stringstream ss;
    ss << ctx;
    return ss.str();
}
