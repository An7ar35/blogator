#include "CommentTk.h"

using namespace blogator::parser::token::html5;

/**
 * Constructor
 * @param position Line:Col position of token in source text
 */
CommentTk::CommentTk( blogator::TextPos position ) :
    HTML5Tk( specs::infra::TokenType::COMMENT, position )
{}

/**
 * Constructor
 * @param text UTF32 text
 * @param position Line:Col position of token in source text
 */
CommentTk::CommentTk( std::u32string text, blogator::TextPos position ) :
    HTML5Tk( specs::infra::TokenType::COMMENT, std::move( text ), position )
{}

#ifdef TESTING

#include "../../../unicode/unicode.h"

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void CommentTk::toStr( std::ostream &os ) const {
    os << R"(["Comment", ")";
    unicode::normalize( os, this->text() );
    os << "\"]";
}

#endif
