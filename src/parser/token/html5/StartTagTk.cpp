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
    GenericTagTk( specs::html5::TokenType::END_TAG, std::move( text ), position )
{}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void StartTagTk::toStr( std::ostream &os ) const {
    os << "html5::StartTagTk=";
    GenericTagTk::toStr( os );
}
