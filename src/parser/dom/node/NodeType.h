#ifndef BLOGATOR_PARSER_DOM_NODE_NODETYPE_H
#define BLOGATOR_PARSER_DOM_NODE_NODETYPE_H

namespace blogator::parser::dom {
    enum class NodeType {
        UNDEFINED                   =  0,
        ELEMENT_NODE                =  1,
        ATTRIBUTE_NODE              =  2, //implemented w/ composition instead
        TEXT_NODE                   =  3,
        CDATA_SECTION_NODE          =  4,
      //ENTITY_REFERENCE_NODE       =  5, //not implemented/legacy
      //ENTITY_NODE                 =  6, //not implemented/legacy
      //PROCESSING_INSTRUCTION_NODE =  7, //not implemented
        COMMENT_NODE                =  8,
        DOCUMENT_NODE               =  9,
        DOCUMENT_TYPE_NODE          = 10, //DOCTYPE
        DOCUMENT_FRAGMENT_NODE      = 11,
      //NOTATION_NODE               = 12, //not implemented/legacy
    };
}

#endif //BLOGATOR_PARSER_DOM_NODE_NODETYPE_H