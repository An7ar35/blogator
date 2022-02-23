#ifndef BLOGATOR_PARSER_DOM_DEFS_H
#define BLOGATOR_PARSER_DOM_DEFS_H

#include <vector>
#include <list>
#include <string>
#include <memory>

namespace blogator::parser::dom {
    namespace node {
        class Attr;
        class CDATASection;
        class Comment;
        class Document;
        class Node;
        class Element;
    }

    /* Definitions */
    typedef std::u32string                             DOMString_t;
    typedef std::u32string                             USVString_t;

    typedef std::unique_ptr<node::Node>                NodePtr_t;
    typedef std::list<node::Node *>                    NodeList_t;
    typedef std::vector<NodePtr_t>                     Nodes_t;
    typedef std::unique_ptr<node::Attr>                AttrPtr_t;
    typedef std::vector<AttrPtr_t>                     Attributes_t;
    typedef int                                        NamespaceMapID_t;

    typedef std::vector<node::Element *>               HTMLCollection_t;
    typedef std::vector<DOMString_t>                   DOMTokenList_t;

    template<typename T> using Sequence_t = std::vector<T>;
}

#endif //BLOGATOR_PARSER_DOM_DEFS_H