#include "Blogator.h"

using namespace blogator::parser;

/**
 * Constructor
 * @param ns Context namespace
 * @param element Element type
 * @param line_number Line position of token
 * @param col_number Char position of token on line
 */
token::Blogator::Blogator( specs::Context ns, specs::blogator::Element element, size_t line_number, size_t col_number ) :
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
token::Blogator::Blogator( specs::Context ns, specs::blogator::Element element, std::string text, size_t line_number, size_t col_number ) :
    Token( ns, std::move( text ), line_number, col_number ),
    _element( element )
{}

/**
 * Gets the Blogator element type
 * @return Blogator element
 */
specs::blogator::Element token::Blogator::getElement() const {
    return _element;
}
