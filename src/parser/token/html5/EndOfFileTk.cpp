#include "EndOfFileTk.h"

using namespace blogator::parser::token::html5;

/**
 * Constructor
 * @param position Line:Col position of token in source text
 */
EndOfFileTk::EndOfFileTk( blogator::TextPos position ) :
    HTML5Tk( specs::infra::TokenType::END_OF_FILE, position )
{}

#ifdef TESTING

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void EndOfFileTk::toStr( std::ostream &os ) const {
    os << "[\"EOF\"]";
}

#endif