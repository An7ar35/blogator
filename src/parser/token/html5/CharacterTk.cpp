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

#ifdef TESTING

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void CharacterTk::toStr( std::ostream &os ) const {
    const auto & txt = text();

    os << R"(["Character",")";

    if( txt.size() == 1 && unicode::utf32::isascii( txt.at( 0 ) ) ) {
        auto hex = unicode::utf32::toxunicode( txt.at( 0 ) );
        unicode::utf8::convert( os, hex );
    } else {
        unicode::utf8::convert( os, txt );
    }

    os << "\"]";
}

#else

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void CharacterTk::toStr( std::ostream &os ) const {
    os << "html5::CharacterTk={ text: \"";
    unicode::utf8::convert( os, text() );
    os << "\", position: " << lineNum() << ":" << colPos() << " }";
}

#endif
