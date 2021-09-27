#include "HTML5Tk.h"

using namespace blogator::parser::token;

/**
 * Constructor
 * @param type HTML5 token type
 * @param position Line:Col position of token in source text
 */
html5::HTML5Tk::HTML5Tk( specs::html5::TokenType type, TextPos position ) :
    Token( specs::Context::HTML5, position ),
    _type( type )
{}

/**
 * Constructor
 * @param type HTML5 token type
 * @param text UTF32 text
 * @param position Line:Col position of token in source text
 */
html5::HTML5Tk::HTML5Tk( specs::html5::TokenType type, std::u32string text, TextPos position ) :
    Token( specs::Context::HTML5, std::move( text ), position ),
    _type( type )
{}

/**
 * Gets the token type
 * @return Token type
 */
blogator::parser::specs::html5::TokenType html5::HTML5Tk::type() const {
    return _type;
}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void html5::HTML5Tk::toStr( std::ostream &os ) const {
    os << "html5::GenericTk={ type: " << static_cast<int>( _type ) << ", text: \"";
    unicode::utf8::convert( os, text() );
    os << "\", position: " << lineNum() << ":" << colPos() << " }";
}
