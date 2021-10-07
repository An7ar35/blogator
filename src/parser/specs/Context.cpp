#include "Context.h"

/**
 * Converts a context to a string representation
 * @param ctx Context
 * @return String
 */
std::string blogator::parser::specs::ctxToStr( blogator::parser::specs::Context ctx ) {
    switch( ctx ) {
        case Context::UNKNOWN:
            return "Unknown";
        case Context::BLOGATOR:
            return "Blogator";
        case Context::HTML5:
            return "HTML5";
        case Context::MARKDOWN:
            return "Markdown";
        default:
            //TODO log error
            return "missing - see log";
    }
}
