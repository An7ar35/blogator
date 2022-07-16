#include "ListTk.h"

#include "../../../unicode/unicode.h"
#include "../../specs/markdown/ListType.h"

using namespace blogator::parser::token::markdown;

/**
 * Constructor
 * @param list_type List type
 * @param position Line:Col position of token in source text
 */
ListTk::ListTk( specs::markdown::ListType list_type, TextPos position ) :
    BlockBeginTk( specs::markdown::TokenType::LIST, position ),
    _list_type( list_type ),
    _tight( true )
{}

/**
 * Set the list's tight separation flag
 * @param flag 'Tight' flag
 */
void ListTk::setTightFlag( bool flag ) {
    _tight = flag;
}

/**
 * Gets the list type
 * @return List type enum
 */
blogator::parser::specs::markdown::ListType ListTk::listType() const {
    return _list_type;
}

/**
 * Gets the list's tight format flag
 * @return Tight flag
 */
bool ListTk::tight() const {
    return _tight;
}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void ListTk::toStr( std::ostream &os ) const {
    os << R"({ "type": ")" << this->type()
       << R"(", "list": ")" << this->listType()
       << R"(", "tight": ")" << ( this->tight() ? "true" : "false" )
       << R"(", "position": ")" << this->position()
       << R"(" })";
}
