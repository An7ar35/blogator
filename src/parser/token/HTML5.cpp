#include "HTML5.h"

#include <utility>

using namespace blogator::parser;

/**
 * Constructor
 * @param ns Context namespace
 * @param element Element type
 * @param line_number Line position of token
 * @param col_number Char position of token on line
 */
token::HTML5::HTML5( specs::Context ns, specs::html5::Element element, size_t line_number, size_t col_number ) :
    Token( ns, line_number, col_number ),
    _element( element )
{}

/**
 * Constructor
 * @param ns Context namespace
 * @param element Element type
 * @param text Text
 * @param line_number Line position of token
 * @param col_number Char position of token on line
 */
token::HTML5::HTML5( specs::Context ns, specs::html5::Element element, std::string text, size_t line_number, size_t col_number ) :
    Token( ns, std::move( text ), line_number, col_number ),
    _element( element )
{}

/**
 * Gets the HTML element type
 * @return HTML element
 */
specs::html5::Element token::HTML5::getElement() const {
    return _element;
}