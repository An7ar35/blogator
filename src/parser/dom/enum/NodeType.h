#ifndef BLOGATOR_PARSER_DOM_ENUM_NODETYPE_H
#define BLOGATOR_PARSER_DOM_ENUM_NODETYPE_H

#include <ostream>
#include <string>

namespace blogator::parser::dom {
    enum class NodeType {
        UNDEFINED                   =  0,
        ELEMENT_NODE                =  1,
        ATTRIBUTE_NODE              =  2,
        TEXT_NODE                   =  3,
        CDATA_SECTION_NODE          =  4,
      //ENTITY_REFERENCE_NODE       =  5, //not implemented/legacy
      //ENTITY_NODE                 =  6, //not implemented/legacy
      //PROCESSING_INSTRUCTION_NODE =  7, //not implemented
        COMMENT_NODE                =  8,
        DOCUMENT_NODE               =  9,
        DOCUMENT_TYPE_NODE          = 10,
        DOCUMENT_FRAGMENT_NODE      = 11,
      //NOTATION_NODE               = 12, //not implemented/legacy
    };

    std::ostream & operator <<( std::ostream &os, NodeType type );
}

namespace blogator {
    std::string to_string( blogator::parser::dom::NodeType type );
}

#endif //BLOGATOR_PARSER_DOM_ENUM_NODETYPE_H