#include "StartTagTk.h"

#include <algorithm>

using namespace blogator::parser::token::html5;

/**
 * Constructor
 * @param position Line:Col position of token in source text
 */
StartTagTk::StartTagTk( blogator::parser::TextPos position ) :
    GenericTagTk( specs::infra::TokenType::START_TAG, position )
{}

/**
 * Constructor
 * @param text UTF32 text
 * @param position Line:Col position of token in source text
 */
StartTagTk::StartTagTk( std::u32string text, blogator::parser::TextPos position ) :
    GenericTagTk( specs::infra::TokenType::START_TAG, std::move( text ), position )
{}

#ifdef TESTING

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void StartTagTk::toStr( std::ostream &os ) const {
    os << R"(["StartTag", ")";
    unicode::normalize( os, this->text() );
    os << "\", { ";
    for( auto it = attributes().cbegin(); it != attributes().cend(); ++it ) {
        os << "\"";
        unicode::normalize( os, it->name );
        os << "\":\"";
        unicode::normalize( os, it->value );
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
}

#endif
