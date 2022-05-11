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
 * Constructor
 * @param alt_text Image alt text
 * @param position Line:Col position of token in source text
 */
ImageTk::ImageTk( std::u32string alt_text, TextPos position ) :
    MarkdownTk( specs::markdown::TokenType::IMAGE, std::move( alt_text ), position )
{}

/**
 * Constructor
 * @param alt_text Image alt text
 * @param src Image source
 * @param position Line:Col position of token in source text
 */
ImageTk::ImageTk( std::u32string alt_text, std::u32string src, TextPos position ) :
    MarkdownTk( specs::markdown::TokenType::IMAGE, std::move( alt_text ), position ),
    _src( std::move( src ) )
{}

/**
 * Sets the image's source
 * @param src Source
 */
void ImageTk::setSource( const std::u32string &src ) {
    _src = src;
}

/**
 * Gets the image's alt-text
 * @return Alt-text
 */
const std::u32string & ImageTk::altTxt() const {
    return this->text();
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
void  ImageTk::toStr( std::ostream &os ) const {
    os << R"({ "type": ")" << this->type() << R"(", "alt": ")";
    unicode::utf8::convert( os, this->altTxt() );
    os << R"(", "src": ")";
    unicode::utf8::convert( os, this->src() );
    os << R"(" })";
}