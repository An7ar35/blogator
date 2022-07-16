#include "TableTk.h"

using namespace blogator::parser::token::markdown;

/**
 * Constructor
 * @param position Line:Col position of token in source text
 */
TableTk::TableTk( TextPos position ) :
    BlockBeginTk( specs::markdown::TokenType::TABLE, position )
{}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void TableTk::toStr( std::ostream &os ) const {
    os << R"({ "type": ")" << this->type()
       << R"(", "position": ")" << this->position()
       << R"(" })";
}