#include "HtmlBlockTk.h"

using namespace blogator::parser::token::markdown;

/**
 * Constructor
 * @param position Line:Col position of token in source text
 */
HtmlBlockTk::HtmlBlockTk( blogator::parser::TextPos position ) :
    BlockBeginTk( specs::markdown::TokenType::HTML, position )
{}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void HtmlBlockTk::toStr( std::ostream &os ) const {
    os << R"({ "type": ")" << this->type()
       << R"(", "position": ")" << this->position()
       << R"(" })";
}