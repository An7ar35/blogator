#include "DLTitleTk.h"

using namespace blogator::parser::token::markdown;

/**
 * Constructor
 * @param position Line:Col position of token in source text
 */
DLTitleTk::DLTitleTk( blogator::TextPos position ) :
    BlockBeginTk( specs::markdown::TokenType::DEFINITION_LIST_DT, position )
{}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void DLTitleTk::toStr( std::ostream &os ) const {
    os << R"({ "type": ")" << this->type()
       << R"(", "position": ")" << this->position()
       << R"(" })";
}
