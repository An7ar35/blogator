#include "TableHeadingTk.h"

#include "../../../unicode/unicode.h"

using namespace blogator::parser::token::markdown;

/**
 * Constructor
 * @param position Line:Col position of token in source text
 */
TableHeadingTk::TableHeadingTk( TextPos position ) :
    BlockBeginTk( specs::markdown::TokenType::TABLE_HEADING, position ),
    _alignment( Alignment::NONE )
{}

/**
 * Sets table 'heading' alignment for the column
 * @param alignment Horizontal alignment
 */
void TableHeadingTk::setAlignment( TableHeadingTk::Alignment alignment ) {
    this->_alignment = alignment;
}

/**
 * Gets the heading column's alignment
 * @return Alignment
 */
TableHeadingTk::Alignment blogator::parser::token::markdown::TableHeadingTk::alignment() const {
    return _alignment;
}

/**
 * Resolves alignment
 * @param def Alignment definition (e.g.: "?-?")
 * @return Alignment
 */
TableHeadingTk::Alignment TableHeadingTk::resolveAlignment( const std::u32string & def ) {
    return TableHeadingTk::resolveAlignment( def.front(), def.back() );
}

/**
 * Resolves alignment
 * @param def Alignment definition (e.g.: "?-?")
 * @return Alignment
 */
TableHeadingTk::Alignment TableHeadingTk::resolveAlignment( const std::vector<char32_t> & def ) {
    return TableHeadingTk::resolveAlignment( def.front(), def.back() );
}

/**
 * Prints out a string representation of the token
 * @param os Output stream
 */
void TableHeadingTk::toStr( std::ostream &os ) const {
    os << R"({ "type": ")" << this->type()
       << R"(", "alignment": ")" << this->alignment()
       << R"(", "position": ")" << this->position()
       << R"(" })";
}

/**
 * Resolves alignment
 * @param first First character in definition
 * @param last Last character in definition
 * @return Resolved alignment
 */
inline TableHeadingTk::Alignment TableHeadingTk::resolveAlignment( char32_t first, char32_t last ) {
    if( first == unicode::COLON ) { //":..?"
        return ( last == unicode::COLON ? Alignment::CENTER : Alignment::LEFT );
    } else if( last == unicode::COLON ) { //"..:"
        return Alignment::RIGHT;
    } else {
        return Alignment::NONE;
    }
}

/**
 * Output stream operator
 * @param os Output stream
 * @param alignment TableHeadingTk::Alignment enum
 * @return Output stream
 */
std::ostream & blogator::parser::token::markdown::operator <<( std::ostream &os, TableHeadingTk::Alignment alignment ) {
    switch( alignment ) {
        case TableHeadingTk::Alignment::NONE  : { os << "NONE";   } break;
        case TableHeadingTk::Alignment::LEFT  : { os << "LEFT";   } break;
        case TableHeadingTk::Alignment::CENTER: { os << "CENTER"; } break;
        case TableHeadingTk::Alignment::RIGHT : { os << "RIGHT";  } break;
    }

    return os;
}