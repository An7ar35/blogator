#include "EndOfFileTk.h"

using namespace blogator::parser::token::markdown;

/**
 * Constructor
 * @param position Line:Col position of token in source text
 */
EndOfFileTk::EndOfFileTk( blogator::parser::TextPos position ) :
    MarkdownTk( specs::markdown::TokenType::END_OF_FILE, position )
{}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void EndOfFileTk::toStr( std::ostream &os ) const {
    os << R"({ "type": ")" << this->type()
       << R"(", "position": ")" << this->position()
       << R"(" })";
}