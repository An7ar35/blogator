#include "Block.h"

#include "../../token/markdown/MarkdownTk.h"
#include "../../token/markdown/ListTk.h"

using namespace blogator::parser::tokeniser::markdown;

/**
 * Constructor
 * @param t Block type
 * @param pos Text position
 */
Block::Block( specs::markdown::TokenType t, parser::TextPos pos ) :
    type( t ), position( pos ), token( nullptr )
{}

/**
 * Constructor
 * @param tk Pointer to token in queue
 * @param pos Text position
 */
Block::Block( token::markdown::MarkdownTk * tk ) :
    type( tk->type() ), position( tk->position() ), token( tk )
{}

/**
 * Equality operator
 * @param rhs Block to check against
 * @return Equality state
 */
bool Block::operator ==( const Block &rhs ) const {
    return this->type     == rhs.type
        && this->position == rhs.position
        && this->token    == rhs.token;
}

/**
 * Gets the column position of the block
 * @return Block column position
 */
size_t Block::column() const {
    return position.col;
}

/**
 * Gets the ListType sub-type of the block (if any)
 * @return ListType enum
 */
blogator::parser::specs::markdown::ListType Block::listType() const {
    if( type == specs::markdown::TokenType::LIST && token != nullptr ) {
        return dynamic_cast<const token::markdown::ListTk *>( token )->listType();
    }

    return specs::markdown::ListType::NONE;
}