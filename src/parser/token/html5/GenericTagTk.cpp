#include "GenericTagTk.h"

using namespace blogator::parser::token::html5;

/**
 * Constructor
 * @param type HTML5 token type
 * @param position Line:Col position of token in source text
 */
GenericTagTk::GenericTagTk( specs::html5::TokenType type, blogator::parser::TextPos position ) :
    HTML5Tk( type, position ),
    _self_closing( false )
{}

/**
 * Constructor
 * @param type HTML5 token type
 * @param text UTF32 text
 * @param position Line:Col position of token in source text
 */
GenericTagTk::GenericTagTk( specs::html5::TokenType type, std::u32string text, blogator::parser::TextPos position ) :
    HTML5Tk( type, std::move( text ), position ),
    _self_closing( false )
{}

/**
 * Gets the name of the tag
 * @return Name string
 */
std::u32string GenericTagTk::name() const {
    return this->text();
}

/**
 * Gets the self-closing flag state
 * @return Self-closing state
 */
bool GenericTagTk::selfclosing() const {
    return _self_closing;
}

/**
 * Gets the list of attributes
 * @return List of attributes
 */
const Attributes_t &GenericTagTk::attributes() const {
    return _attributes;
}

/**
 * Adds an attribute to the list
 * @param name Attribute name
 * @param value Attribute value
 */
void GenericTagTk::addAttribute( std::u32string name, std::u32string value ) {
    _attributes.emplace_back( Attribute { .name = std::move( name ), .value = std::move( value ) } );
}

/**
 * Sets the self-closing flag
 * @param flag Flag state (default=true)
 */
void GenericTagTk::setSelfClosing( bool flag ) {
    _self_closing = flag;
}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void GenericTagTk::toStr( std::ostream &os ) const {
    os << "{ text: \"";
    unicode::utf8::convert( os, text() );
    os << "\", self-closing: " << selfclosing() << ", attributes: [ ";
    for( const auto & attr : attributes() ) {
        unicode::utf8::convert( os, attr.name );
        os << "=\"";
        unicode::utf8::convert( os, attr.value );
        os << "\" ";
    }
    os << "], position: " << lineNum() << ":" << colPos() << " }";
}
