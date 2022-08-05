#include "HtmlBlockTk.h"

using namespace blogator::parser::token::markdown;

/**
 * Constructor
 * @param position Line:Col position of token in source text
 */
HtmlBlockTk::HtmlBlockTk( blogator::parser::TextPos position ) :
    BlockBeginTk( specs::markdown::TokenType::HTML, position ),
    _html_block_type( specs::markdown::HtmlBlockType::UNKNOWN )
{}

/**
 * Sets the HTML block type
 * @param type Type enum
 */
void HtmlBlockTk::setHtmlBlockType( blogator::parser::specs::markdown::HtmlBlockType type ) {
    _html_block_type = type;
}

/**
 * Gets the HTML block type
 * @return HTML block type enum
 */
blogator::parser::specs::markdown::HtmlBlockType HtmlBlockTk::htmlBlockType() const {
    return _html_block_type;
}


/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void HtmlBlockTk::toStr( std::ostream &os ) const {
    os << R"({ "type": ")" << this->type()
       << R"(", "html type": ")" << this->htmlBlockType()
       << R"(", "position": ")" << this->position()
       << R"(" })";
}