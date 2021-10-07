#include "StartTagTk.h"

using namespace blogator::parser::token::html5;

/**
 * Constructor
 * @param position Line:Col position of token in source text
 */
StartTagTk::StartTagTk( blogator::parser::TextPos position ) :
    GenericTagTk( specs::html5::TokenType::START_TAG, position )
{}

/**
 * Constructor
 * @param text UTF32 text
 * @param position Line:Col position of token in source text
 */
StartTagTk::StartTagTk( std::u32string text, blogator::parser::TextPos position ) :
    GenericTagTk( specs::html5::TokenType::START_TAG, std::move( text ), position )
{}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void StartTagTk::toStr( std::ostream &os ) const {
#ifdef TESTING
    os << R"(["StartTag", ")";
    unicode::utf8::convert( os, text() );
    os << "\", { ";
    for( auto it = attributes().cbegin(); it != attributes().cend(); ++it ){
        os << "\"";
        unicode::utf8::convert( os, it->name );
        os << "\":\"";
        unicode::utf8::convert( os, it->value );
        os << "\"";
        if( std::next( it ) != attributes().cend() ) {
            os << ", ";
        }
    }
    os << "}";
    if( selfclosing() ) {
        os << ",true";
    }
    os << "]";
#else
    os << "html5::StartTagTk=";
    GenericTagTk::toStr( os );
#endif
}
