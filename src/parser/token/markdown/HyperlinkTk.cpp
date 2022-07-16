#include "HyperlinkTk.h"

#include "../../../unicode/unicode.h"

using namespace blogator::parser::token::markdown;

/**
 * Constructor
 * @param position Line:Col position of token in source text
 */
HyperlinkTk::HyperlinkTk( TextPos position ) :
    MarkdownTk( specs::markdown::TokenType::HYPERLINK, position )
{}

/**
 * Sets the hyperlink url
 * @param address URL
 */
void HyperlinkTk::setURL( const std::u32string & url ) {
    this->setText( url );
}

/**
 * Gets the URL
 * @return URL string
 */
const std::u32string & HyperlinkTk::url() const {
    return this->text();
}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void HyperlinkTk::toStr( std::ostream &os ) const {
    os << R"({ "type": ")" << this->type() << R"(", "url": ")";
    unicode::normalize( os, this->text() );
    os << R"(", "position": ")" << this->position()
       << R"(" })";
}
