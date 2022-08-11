#include "DLDefinitionTk.h"

using namespace blogator::parser::token::markdown;

/**
 * Constructor
 * @param position Line:Col position of token in source text
 */
DLDefinitionTk::DLDefinitionTk( blogator::TextPos position ) :
    BlockBeginTk( specs::markdown::TokenType::DEFINITION_LIST_DD, position )
{}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void DLDefinitionTk::toStr( std::ostream &os ) const {
    os << R"({ "type": ")" << this->type()
       << R"(", "position": ")" << this->position()
       << R"(" })";
}