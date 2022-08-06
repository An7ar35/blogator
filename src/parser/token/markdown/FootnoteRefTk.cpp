#include "FootnoteRefTk.h"

#include "../../../unicode/utf8.h"

/**
 * Constructor
 * @param ref Footnote number
 * @param position Line:Col position of token in source text
 */
blogator::parser::token::markdown::FootnoteRefTk::FootnoteRefTk( std::u32string ref, TextPos position ) :
    MarkdownTk( specs::markdown::TokenType::FOOTNOTE_REF, std::move( ref ), position )
{}

/**
 * Gets the reference string
 * @return Footnote reference string
 */
const std::u32string & blogator::parser::token::markdown::FootnoteRefTk::ref() const {
    return this->text();
}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void blogator::parser::token::markdown::FootnoteRefTk::toStr( std::ostream &os ) const {
    os << R"({ "type": ")" << this->type()
       << R"(", "ref": ")";
    unicode::utf8::convert( os, this->text() );
    os << R"(", "position": ")" << this->position()
       << R"(" })";
}