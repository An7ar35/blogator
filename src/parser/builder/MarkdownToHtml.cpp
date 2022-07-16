#include "MarkdownToHtml.h"

#include "../dto/U32Text.h"
#include "../token/markdown/MarkdownTk.h"
#include "../../exception/failed_expectation.h"

using namespace blogator::parser::builder;

/**
 * Constructor
 */
MarkdownToHtml::MarkdownToHtml() :
    _error_count( 0 ),
    _current_insert_mode( InsertionMode_e::INITIAL ),
    _original_insert_mode( InsertionMode_e::INITIAL )
{}

/**
 * Initialises the builder to an initial state
 * @param path Source file path
 */
void MarkdownToHtml::init( std::filesystem::path path ) {
    _src_path             = std::move( path );
    _output               = std::make_unique<std::vector<char32_t>>();
    _current_insert_mode  = InsertionMode_e::INITIAL;
    _original_insert_mode = InsertionMode_e::INITIAL;
}

/**
 * Process token
 * @param token Markdown token
 * @throw blogator::exception::failed_expectation when builder has not been initialised
 */
void MarkdownToHtml::dispatch( std::unique_ptr<token::markdown::MarkdownTk> token ) { //TODO

}

/**
 * Extracts the html output
 * @return HTML text output
 */
std::unique_ptr<blogator::parser::U32Text> MarkdownToHtml::reset() {
    if( _output ) {
        auto u32text = std::make_unique<U32Text>( _src_path, *_output );
        _error_count = 0;
        _src_path.clear();
        _output.reset();
        return std::move( u32text );

    } else {
        throw FAILED_EXPECTATION_EXCEPTION(
            "[parser::builder::MarkdownToHtml::reset()] Builder had not been initiated."
        );
    }
}

/**
 * Gets the number of parsing error raised
 * @return Parsing error count
 */
size_t MarkdownToHtml::errors() const {
    return _error_count;
}
