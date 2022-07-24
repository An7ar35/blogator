#include "CodeBlockTk.h"

using namespace blogator::parser::token::markdown;

/**
 * Constructor
 * @param position Line:Col position of token in source text
 */
CodeBlockTk::CodeBlockTk( blogator::parser::TextPos position ) :
    BlockBeginTk( specs::markdown::TokenType::CODE_BLOCK, position )
{}

/**
 * Constructor
 * @param lang  Language tag for code block
 * @param position Line:Col position of token in source text
 */
CodeBlockTk::CodeBlockTk( std::u32string lang, blogator::parser::TextPos position ) :
    BlockBeginTk( specs::markdown::TokenType::CODE_BLOCK, std::move( lang ), position )
{}

/**
 * Gets the language tag for the code block
 * @return Language tag string
 */
const std::u32string & CodeBlockTk::lang() const {
    return this->text();
}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void CodeBlockTk::toStr( std::ostream &os ) const {
    os << R"({ "type": ")" << this->type()
       << R"(", "text": ")";
    unicode::utf8::convert( os, this->text() );
    os << R"(", "position": ")" << this->position()
       << R"(" })";
}