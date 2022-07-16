#include "TokenType.h"

#include <unordered_map>
#include <set>
#include <sstream>

/**
 * Output stream operator
 * @param os Output stream
 * @param attr TokenType enum
 * @return Output stream
 */
std::ostream & blogator::parser::specs::markdown::operator <<( std::ostream &os, blogator::parser::specs::markdown::TokenType type ) {
    static const auto map = std::unordered_map<TokenType, std::string>( {
        { TokenType::UNKNOWN,            "UNKNOWN" },
        /* Blocks */
        { TokenType::HEADING,            "HEADING" },
        { TokenType::PARAGRAPH,          "PARAGRAPH" },
        { TokenType::HORIZONTAL_RULE,    "HORIZONTAL_RULE" },
        { TokenType::CODE_BLOCK,         "CODE_BLOCK" },
        { TokenType::BLOCKQUOTE,         "BLOCK_QUOTE" },
        { TokenType::LIST,               "LIST" },
        { TokenType::LIST_ITEM,          "LIST_ITEM" },
        { TokenType::TABLE,              "TABLE" },
        { TokenType::TABLE_ROW,          "TABLE_ROW" },
        { TokenType::TABLE_HEADING,      "TABLE_HEADING" },
        { TokenType::TABLE_CELL,         "TABLE_CELL" },
        { TokenType::DEFINITION_LIST_DT, "DEFINITION_LIST_DT" },
        { TokenType::DEFINITION_LIST_DD, "DEFINITION_LIST_DD" },
        { TokenType::BLOCK_END,          "BLOCK_END" },
        /* Inline */
        { TokenType::CHARACTER,          "CHARACTER" },
        { TokenType::LINE_BREAK,         "LINE_BREAK" },
        { TokenType::FORMAT_BEGIN,       "FORMAT_BEGIN" },
        { TokenType::FORMAT_END,         "FORMAT_END" },
        { TokenType::IMAGE,              "IMAGE" },
        { TokenType::HYPERLINK,          "HYPERLINK" },
        { TokenType::BLOCK_ID,           "BLOCK_ID" },
        { TokenType::FOOTNOTE_REF,       "FOOTNOTE_REF" },

        { TokenType::END_OF_FILE,        "EOF" },
    } );

    if( map.contains( type ) ) {
        os << map.at( type );
    } else {
        os << "enum string not found";
    }

    return os;
}

/**
 * Checks token type is a block
 * @param type specs::markdown::TokenType enum
 * @return Block state
 */
bool blogator::parser::specs::markdown::isBlockToken( TokenType type ) {
    static const auto BLOCKS = std::set<TokenType>( {
        TokenType::HEADING,
        TokenType::PARAGRAPH,
        TokenType::HORIZONTAL_RULE,
        TokenType::CODE_BLOCK,
        TokenType::BLOCKQUOTE,
        TokenType::LIST,
        TokenType::LIST_ITEM,
        TokenType::TABLE,
        TokenType::TABLE_ROW,
        TokenType::TABLE_HEADING,
        TokenType::TABLE_CELL,
        TokenType::DEFINITION_LIST_DT,
        TokenType::DEFINITION_LIST_DD,
        TokenType::BLOCK_END,
    } );

    return BLOCKS.contains( type );
}

/**
 * Checks token type is a container block
 * @param type specs::markdown::TokenType enum
 * @return Container block state
 */
bool blogator::parser::specs::markdown::isContainerBlock( TokenType type ) {
    static const auto BLOCKS = std::set<TokenType>( {
        TokenType::BLOCKQUOTE,
        TokenType::LIST,
        TokenType::LIST_ITEM,
    } );

    return BLOCKS.contains( type );
}

/**
 * Checks token type is leaf block
 * @param type specs::markdown::TokenType enum
 * @return Leaf block state
 */
bool blogator::parser::specs::markdown::isLeafBlock( TokenType type ) {
    static const auto BLOCKS = std::set<TokenType>( {
        TokenType::HEADING,
        TokenType::PARAGRAPH,
        TokenType::DEFINITION_LIST_DT,
        TokenType::DEFINITION_LIST_DD,
        TokenType::CODE_BLOCK,
    } );

    return BLOCKS.contains( type );
}

/**
 * Converts a parser::specs::markdown::TokenType enum to a string representation
 * @param attr TokenType enum
 * @return String representation
 */
std::string blogator::to_string( blogator::parser::specs::markdown::TokenType type ) {
    std::stringstream ss;
    ss << type;
    return ss.str();
}