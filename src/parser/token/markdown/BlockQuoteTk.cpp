#include "BlockQuoteTk.h"

#include "../../../unicode/ascii.h"
#include "../../../unicode/utf8.h"

using namespace blogator::parser::token::markdown;

/**
 * Constructor
 * @param nesting Nesting level for blockquote
 * @param position Line:Col position of token in source text
 */
BlockQuoteTk::BlockQuoteTk( size_t nesting, blogator::TextPos position ) :
    BlockBeginTk( specs::markdown::TokenType::BLOCKQUOTE, std::u32string( 1, blogator::unicode::GREATER_THAN_SIGN ), position ),
    _nesting( nesting )
{}

/**
 * Gets the blockquote's nesting level (i.e.: number of '>')
 * @return Nesting level
 */
size_t BlockQuoteTk::nesting() const {
    return _nesting;
}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void BlockQuoteTk::toStr( std::ostream &os ) const {
    os << R"({ "type": ")" << this->type()
       << R"(", "nesting": )" << this->nesting()
       << R"(, "text": ")";
    unicode::utf8::convert( os, this->text() );
    os << R"(", "position": ")" << this->position()
       << R"(" })";
}