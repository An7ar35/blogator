#include "ImageTk.h"

#include "../../../unicode/utf8.h"

using namespace blogator::parser::token::markdown;

/**
 * Constructor
 * @param position Line:Col position of token in source text
 */
ImageTk::ImageTk( TextPos position ) :
    MarkdownTk( specs::markdown::TokenType::IMAGE, position )
{}

/**
 * Sets the image's source
 * @param src Source
 */
void ImageTk::setSource( const std::u32string &src ) {
    _src = src;
}

/**
 * Gets the image's source
 * @return Image source
 */
const std::u32string & ImageTk::src() const {
    return _src;
}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void ImageTk::toStr( std::ostream &os ) const {
    os << R"({ "type": ")" << this->type() << R"(", "src": ")";
    unicode::utf8::convert( os, this->src() );
    os << R"(", "position": ")" << this->position()
       << R"(" })";
}