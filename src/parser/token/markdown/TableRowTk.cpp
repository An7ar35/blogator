#include "TableRowTk.h"

using namespace blogator::parser::token::markdown;

/**
 * Constructor
 * @param position Line:Col position of token in source text
 */
TableRowTk::TableRowTk( blogator::parser::TextPos position ) :
    BlockBeginTk( specs::markdown::TokenType::TABLE_ROW, position )
{}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void TableRowTk::toStr( std::ostream &os ) const {
    os << R"({ "type": ")" << this->type()
       << R"(", "position": ")" << this->position()
       << R"(" })";
}
