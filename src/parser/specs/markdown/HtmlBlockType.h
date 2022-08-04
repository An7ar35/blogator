#ifndef BLOGATOR_PARSER_SPECS_MARKDOWN_HTMLBLOCKTYPE_H
#define BLOGATOR_PARSER_SPECS_MARKDOWN_HTMLBLOCKTYPE_H

#include <ostream>

namespace blogator::parser::specs::markdown {
    enum class HtmlBlockType {
        UNKNOWN = 0,
        TYPE_1  = 1,
        TYPE_2,
        TYPE_3,
        TYPE_4,
        TYPE_5,
        TYPE_6,
        TYPE_7,
    };

    std::ostream & operator <<( std::ostream &os, HtmlBlockType type );

    bool isHtmlBlockType1( const std::u32string & tag_name );
    bool isHtmlBlockType6( const std::u32string & tag_name );
}

#endif //BLOGATOR_PARSER_SPECS_MARKDOWN_HTMLBLOCKTYPE_H