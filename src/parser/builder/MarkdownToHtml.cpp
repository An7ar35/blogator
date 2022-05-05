#include "MarkdownToHtml.h"

/**
 * Constructor
 * @param output Output for HTML-ized markdown text
 */
blogator::parser::builder::MarkdownToHtml::MarkdownToHtml( std::vector<uint32_t> & output ) :
    _error_count( 0 ),
    _output( output )
{}

void blogator::parser::builder::MarkdownToHtml::dispatch( std::unique_ptr<token::markdown::MarkdownTk> token ) { //TODO

}

size_t blogator::parser::builder::MarkdownToHtml::errors() const {
    return _error_count;
}
