#include "HorizontalRuleTk.h"

#include "../../../unicode/utf8.h"

using namespace blogator::parser::token::markdown;

/**
 * Constructor
 * @param text Text
 * @param position Line:Col position of token in source text
 */
HorizontalRuleTk::HorizontalRuleTk( std::u32string text, TextPos position ) :
    BlockBeginTk( specs::markdown::TokenType::HORIZONTAL_RULE, std::move( text ), position )
{}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void HorizontalRuleTk::toStr( std::ostream &os ) const {
    os << R"({ "type": ")" << this->type()
       << R"(", "text": ")";
    unicode::utf8::convert( os, this->text() );
    os << R"(", "position": ")" << this->position()
       << R"(" })";
}