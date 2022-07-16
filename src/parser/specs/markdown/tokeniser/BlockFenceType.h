#ifndef BLOGATOR_PARSER_SPECS_MARKDOWN_BLOCKFENCETYPE_H
#define BLOGATOR_PARSER_SPECS_MARKDOWN_BLOCKFENCETYPE_H

#include <ostream>
#include <string>

namespace blogator::parser::specs::markdown {
    enum class BlockFenceType {
        NONE = 0,
        TRIPLE_TILDE,
        TRIPLE_GRAVE_ACCENT,
    };

    std::ostream & operator <<( std::ostream &os, BlockFenceType type );
}

namespace blogator {
    std::string to_string( blogator::parser::specs::markdown::BlockFenceType type );
}

#endif //BLOGATOR_PARSER_SPECS_MARKDOWN_BLOCKFENCETYPE_H