#include "MarkdownToHtml.h"


using namespace blogator::parser::builder;

/**
 * Constructor
 * @param output Output for HTML-ized markdown text
 */
MarkdownToHtml::MarkdownToHtml( std::vector<uint32_t> & output ) :
    _error_count( 0 ),
    _output( output )
{}

void MarkdownToHtml::dispatch( std::unique_ptr<token::markdown::MarkdownTk> token ) { //TODO

}

size_t MarkdownToHtml::errors() const {
    return _error_count;
}
