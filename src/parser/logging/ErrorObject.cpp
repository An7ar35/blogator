#include "ErrorObject.h"

#include <utility>

using namespace blogator::parser::logging;

/**
 * Constructor
 * @param src Source filepath
 * @param ctx Context
 * @param err_code Error code
 * @param position Position in source file
 */
ErrorObject::ErrorObject( std::filesystem::path src, specs::Context ctx, int err_code, TextPos position ) :
    _src_file( std::move( src ) ),
    _context( ctx ),
    _code( err_code ),
    _position( position )
{}

/**
 * Gets the source filepath string
 * @return Filepath string
 */
std::string ErrorObject::filepath() const {
    return _src_file.string();
}

/**
 * Gets the context of the error
 * @return Context
 */
std::string ErrorObject::context() const {
    return specs::ctxToStr( _context );
}

/**
 * Gets the short error description
 * @return Short error description string
 */
std::string ErrorObject::error() const {
    switch( _context ) {
        case specs::Context::BLOGATOR:
            return "BLOGATOR"; //TODO
        case specs::Context::HTML5:
            return specs::html5::ErrorCode::str( _code );
        case specs::Context::MARKDOWN:
            return "MARKDOWN"; //TODO
        default:
            return "Unknown Context";
    }
}

/**
 * Gets the long error description
 * @return Long error description string
 */
std::string ErrorObject::detailed() const {
    switch( _context ) {
        case specs::Context::BLOGATOR:
            return "BLOGATOR"; //TODO
        case specs::Context::HTML5:
            return specs::html5::ErrorCode::detailed( _code );
        case specs::Context::MARKDOWN:
            return "MARKDOWN"; //TODO
        default:
            return "Unknown Context";
    }
}

/**
 * Gets the position as a string
 * @return Position string
 */
std::string ErrorObject::position() const {
    std::stringstream ss;
    ss << _position.line << ":" << _position.col;
    return ss.str();
}

/**
 * Prints the source filepath
 * @param os Output stream
 * @return Output stream
 */
std::ostream &ErrorObject::filepath( std::ostream &os ) const {
    os << _src_file.string();
    return os;
}

/**
 * Prints the context
 * @param os Output stream
 * @return Output stream
 */
std::ostream &ErrorObject::context( std::ostream &os ) const {
    os << specs::ctxToStr( _context );
    return os;
}

/**
 * Prints the short error description
 * @param os Output stream
 * @return Output stream
 */
std::ostream &ErrorObject::error( std::ostream &os ) const {
    switch( _context ) {
        case specs::Context::BLOGATOR:
            os << "BLOGATOR"; //TODO
            break;

        case specs::Context::HTML5:
            os << specs::html5::ErrorCode::str( _code );
            break;

        case specs::Context::MARKDOWN:
            os << "MARKDOWN"; //TODO
            break;

        default:
            os << "Unknown Context";
            break;
    }

    return os;
}

/**
 * Prints the long error description
 * @param os Output stream
 * @return Output stream
 */
std::ostream &ErrorObject::detailed( std::ostream &os ) const {
    switch( _context ) {
        case specs::Context::BLOGATOR:
            os << "BLOGATOR"; //TODO
            break;

        case specs::Context::HTML5:
            os << specs::html5::ErrorCode::detailed( _code );
            break;

        case specs::Context::MARKDOWN:
            os << "MARKDOWN"; //TODO
            break;

        default:
            os << "Unknown Context";
            break;
    }

    return os;
}

/**
 * Prints the position
 * @param os Output stream
 * @return Output stream
 */
std::ostream &ErrorObject::position( std::ostream &os ) const {
    os << _position.line << ":" << _position.col;
    return os;
}