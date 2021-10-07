#include "CharacterTk.h"

using namespace blogator::parser::token::html5;

/**
 * Constructor
 * @param position Line:Col position of token in source text
 */
CharacterTk::CharacterTk( TextPos position ) :
    HTML5Tk( specs::html5::TokenType::CHARACTER, position )
{}

/**
 * Constructor
 * @param text UTF32 text
 * @param position Line:Col position of token in source text
 */
CharacterTk::CharacterTk( std::u32string text, TextPos position ) :
    HTML5Tk( specs::html5::TokenType::CHARACTER, std::move( text ), position )
{}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void CharacterTk::toStr( std::ostream &os ) const {
#ifdef TESTING
    os << R"(["Character", ")";
//    if( text().size() == 1 ) {
//        os << unicode::toxunicode( text().at( 0 ) );
//    } else {
        unicode::utf8::convert( os, text() );
//    }
    os << "\"]";
#else
    os << "html5::CharacterTk={ text: \"";
    unicode::utf8::convert( os, text() );
    os << "\", position: " << lineNum() << ":" << colPos() << " }";
#endif
}