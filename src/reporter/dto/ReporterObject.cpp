#include "ReporterObject.h"

#include <utility>

#include "../../parser/specs/native/ErrorCode.h"
#include "../../parser/specs/infra/ErrorCode.h"
#include "../../parser/specs/markdown/ErrorCode.h"

using namespace blogator::reporter;
using namespace blogator::parser;

/**
 * Constructor
 */
ReporterObject::ReporterObject() :
    _src_file( "" ),
    _context( Context::UNKNOWN ),
    _code( 0 ),
    _position( { 0, 0 } )
{}

/**
 * Constructor
 * @param src Source filepath
 * @param ctx Context
 * @param err_code Error code
 * @param position Position in source file
 */
ReporterObject::ReporterObject( std::filesystem::path src, Context ctx, int err_code, TextPos position ) :
    _src_file( std::move( src ) ),
    _context( ctx ),
    _code( err_code ),
    _position( position )
{}

/**
 * Constructor
 * @param src Source filepath
 * @param ctx Context
 * @param err_code Error code
 * @param txt Optional text to append to the error message
 * @param position Position in source file
 */
ReporterObject::ReporterObject( std::filesystem::path src, Context ctx, int err_code, TextPos position, std::string txt ) :
    _src_file( std::move( src ) ),
    _context( ctx ),
    _code( err_code ),
    _position( position ),
    _text( std::move( txt ) )
{}

/**
 * Output stream operator
 * @param os Output stream
 * @param err ErrorObject
 * @return Output stream
 */
std::ostream & blogator::reporter::operator <<( std::ostream &os, const ReporterObject &err ) {
    os << "{ path: " << err.filepath()
       << ", position: " << err.position()
       << ", context: " << err.context()
       << ", description: \"" << err.error() << "\""
       << " }";
    return os;
}

/**
 * Checks if error is the same as another
 * @param rhs Error object to compare to
 * @return Equivalence state (filepath is ignored)
 */
bool ReporterObject::operator ==( const ReporterObject &rhs ) const {
    return context()  == rhs.context()
        && errcode()  == rhs.errcode()
        && position() == rhs.position();
}

/**
 * Checks if error is not the same as another
 * @param rhs Error object to compare to
 * @return Non-equivalence state (filepath is ignored)
 */
bool ReporterObject::operator !=( const ReporterObject &rhs ) const {
    return !( *this == rhs );
}

/**
 * Gets the source filepath string
 * @return Filepath string
 */
std::string ReporterObject::filepath() const {
    return _src_file.string();
}

/**
 * Gets the context of the error
 * @return Context
 */
std::string ReporterObject::context() const {
    return blogator::to_string( _context );
}

/**
 * Gets the short error description
 * @return Short error description string
 */
std::string ReporterObject::error() const {
    switch( _context ) {
        case Context::NATIVE:
            return ( _text.empty()
                     ? specs::native::ErrorCode::str( _code )
                     : specs::native::ErrorCode::str( _code ) + ": " + _text );
        case Context::HTML5:
            return ( _text.empty()
                     ? specs::infra::ErrorCode::str( _code )
                     : specs::infra::ErrorCode::str( _code ) + ": " + _text );
        case Context::MARKDOWN:
            return ( _text.empty()
                     ? specs::markdown::ErrorCode::str( _code )
                     : specs::markdown::ErrorCode::str( _code ) + ": " + _text );
        default:
            return "Unknown Context";
    }
}

/**
 * Gets the long error description
 * @return Long error description string
 */
std::string ReporterObject::detailed() const {
    switch( _context ) {
        case Context::NATIVE:
            return specs::native::ErrorCode::detailed( _code );
        case Context::HTML5:
            return specs::infra::ErrorCode::detailed( _code );
        case Context::MARKDOWN:
            return specs::markdown::ErrorCode::detailed( _code );
        default:
            return "Unknown Context";
    }
}

/**
 * Gets the position as a string
 * @return Position string
 */
std::string ReporterObject::position() const {
    std::stringstream ss;
    ss << _position.line << ":" << _position.col;
    return ss.str();
}

/**
 * Gets the position
 * @return TextPos
 */
blogator::TextPos ReporterObject::textpos() const {
    return _position;
}

/**
 * Prints the source filepath
 * @param os Output stream
 * @return Output stream
 */
std::ostream & ReporterObject::filepath( std::ostream &os ) const {
    os << _src_file.string();
    return os;
}

/**
 * Prints the context
 * @param os Output stream
 * @return Output stream
 */
std::ostream & ReporterObject::context( std::ostream &os ) const {
    os << _context;
    return os;
}

/**
 * Prints the short error description
 * @param os Output stream
 * @return Output stream
 */
std::ostream & ReporterObject::error( std::ostream &os ) const {
    switch( _context ) {
        case Context::NATIVE:
            os << specs::native::ErrorCode::str( _code );
            break;

        case Context::HTML5:
            os << specs::infra::ErrorCode::str( _code );
            break;

        case Context::MARKDOWN:
            os << specs::markdown::ErrorCode::str( _code );
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
std::ostream & ReporterObject::detailed( std::ostream &os ) const {
    switch( _context ) {
        case Context::NATIVE:
            os << specs::native::ErrorCode::detailed( _code );
            break;

        case Context::HTML5:
            os << specs::infra::ErrorCode::detailed( _code );
            break;

        case Context::MARKDOWN:
            os << specs::markdown::ErrorCode::detailed( _code );
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
std::ostream & ReporterObject::position( std::ostream &os ) const {
    os << _position.line << ":" << _position.col;
    return os;
}


/**
 * Gets the context code
 * @return Context
 */
blogator::reporter::Context ReporterObject::ctxcode() const {
    return _context;
}

/**
 * Gets the error code within the context
 * @return Contextual error code
 */
int ReporterObject::errcode() const {
    return _code;
}