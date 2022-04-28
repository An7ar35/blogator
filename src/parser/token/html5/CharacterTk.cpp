#include "CharacterTk.h"

using namespace blogator::parser::token::html5;

/**
 * Constructor
 * @param position Line:Col position of token in source text
 */
CharacterTk::CharacterTk( TextPos position ) :
    HTML5Tk( specs::infra::TokenType::CHARACTER, position )
{}

/**
 * Constructor
 * @param text UTF32 text
 * @param position Line:Col position of token in source text
 */
CharacterTk::CharacterTk( std::u32string text, TextPos position ) :
    HTML5Tk( specs::infra::TokenType::CHARACTER, std::move( text ), position )
{}

#ifdef TESTING

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void CharacterTk::toStr( std::ostream &os ) const {
    os << R"(["Character",")";
    unicode::normalize( os, this->text() );
    os << "\"]";
}

#endif
