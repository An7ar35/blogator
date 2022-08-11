#include "ListItemTk.h"

#include <unordered_map>
#include "../../../unicode/utf8.h"

using namespace blogator::parser::token::markdown;

/**
 * Constructor
 * @param position Line:Col position of token in source text
 */
ListItemTk::ListItemTk( TextPos position ) :
    BlockBeginTk( specs::markdown::TokenType::LIST_ITEM, position ),
    _modality( Modality::NONE )
{}

/**
 * Constructor (Task item)
 * @param ticked Flag for task item modality
 * @param position Line:Col position of token in source text
 */
ListItemTk::ListItemTk( bool ticked, TextPos position ) :
    BlockBeginTk( specs::markdown::TokenType::LIST_ITEM, position ),
    _modality( ( ticked ? Modality::CHECKED : Modality::UNCHECKED ) )
{}

/**
 * Constructor (Generic with ID)
 * @param ref_id Reference ID string
 * @param position Line:Col position of token in source text
 */
ListItemTk::ListItemTk( std::u32string ref_id, blogator::TextPos position ) :
    BlockBeginTk( specs::markdown::TokenType::LIST_ITEM, std::move( ref_id ), position ),
    _modality( Modality::NONE )
{}

/**
 * Gets the prefix type
 * @return List item prefix type
 */
ListItemTk::Modality ListItemTk::modality() const {
    return _modality;
}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void ListItemTk::toStr( std::ostream &os ) const {
    os << R"({ "type": ")" << this->type()
       << R"(", "ref": ")";
    unicode::utf8::convert( os, this->refID() );
    os << R"(", "modality": ")" << this->modality()
       << R"(", "position": ")" << this->position()
       << R"(" })";
}

/**
 * Gets the list item's ID string
 * @return ID string
 */
const std::u32string &ListItemTk::refID() const {
    return this->text();
}


/**
 * Output stream operator
 * @param os Output stream
 * @param type ListItemTk::Type enum
 * @return Output stream
 */
std::ostream & blogator::parser::token::markdown::operator <<( std::ostream &os, ListItemTk::Modality modality ) {
    static const auto MAP = std::unordered_map<blogator::parser::token::markdown::ListItemTk::Modality, std::string>( {
        { ListItemTk::Modality::NONE,      "NONE"      },
        { ListItemTk::Modality::CHECKED,   "CHECKED"   },
        { ListItemTk::Modality::UNCHECKED, "UNCHECKED" },
    } );

    auto it = MAP.find( modality );

    os << ( it != MAP.cend() ? it->second : "ListItemTk::Modality has not string description!" );

    return os;
}
