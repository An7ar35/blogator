#include "TableCellTk.h"

using namespace blogator::parser::token::markdown;

/**
 * Constructor
 * @param position Line:Col position of token in source text
 */
TableCellTk::TableCellTk( TextPos position ) :
    BlockBeginTk( specs::markdown::TokenType::TABLE_CELL, position )
{}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void TableCellTk::toStr( std::ostream &os ) const {
    os << R"({ "type": ")" << this->type()
       << R"(", "position": ")" << this->position()
       << R"(" })";
}
