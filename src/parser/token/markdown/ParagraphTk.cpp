#include "ParagraphTk.h"

#include "../../../unicode/utf8.h"

using namespace blogator::parser::token::markdown;

/**
 * Constructor
 * @param position Line:Col position of token in source text
 */
ParagraphTk::ParagraphTk( TextPos position ) :
    BlockBeginTk( specs::markdown::TokenType::PARAGRAPH, position )
{}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void ParagraphTk::toStr( std::ostream &os ) const {
    os << R"({ "type": ")" << this->type()
       << R"(", "id": ")";
    unicode::utf8::convert( os, this->id() );
    os << R"(", "position": ")" << this->position()
       << R"(" })";
}
