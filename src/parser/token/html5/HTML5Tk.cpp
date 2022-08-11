#include "HTML5Tk.h"

#include "../../../unicode/utf8.h"

using namespace blogator::parser::token;

/**
 * Constructor
 * @param type HTML5 token type
 * @param position Line:Col position of token in source text
 */
html5::HTML5Tk::HTML5Tk( specs::infra::TokenType type, TextPos position ) :
    Token( reporter::Context::HTML5, position ),
    _type( type )
{}

/**
 * Constructor
 * @param type HTML5 token type
 * @param text UTF32 text
 * @param position Line:Col position of token in source text
 */
html5::HTML5Tk::HTML5Tk( specs::infra::TokenType type, std::u32string text, TextPos position ) :
    Token( reporter::Context::HTML5, std::move( text ), position ),
    _type( type )
{}

/**
 * Gets the token type
 * @return Token type
 */
blogator::parser::specs::infra::TokenType html5::HTML5Tk::type() const {
    return _type;
}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void html5::HTML5Tk::toStr( std::ostream &os ) const {
    os << "HTML5Tk={ type: " << this->type() << ", text: \"";
    unicode::utf8::convert( os, this->text() );
    os << "\", position: " << this->position() << " }";
}