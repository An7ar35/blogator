#include "EndOfFileTk.h"

using namespace blogator::parser::token::html5;

/**
 * Constructor
 * @param position Line:Col position of token in source text
 */
EndOfFileTk::EndOfFileTk( blogator::parser::TextPos position ) :
    HTML5Tk( specs::html5::TokenType::END_OF_FILE, position )
{}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void EndOfFileTk::toStr( std::ostream &os ) const {
    os << "html5::EndOfFileTk={ position: " << lineNum() << ":" << colPos() << " }";
}
