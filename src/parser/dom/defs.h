#ifndef BLOGATOR_PARSER_DOM_DEFS_H
#define BLOGATOR_PARSER_DOM_DEFS_H

#include <vector>
#include <list>
#include <string>
#include <memory>

namespace blogator::parser::dom {
    namespace node {
        class Attr;
        class Node;
        class Element;
    }

    typedef std::u32string                   DOMString_t;
    typedef std::u32string                   USVString_t;
    typedef std::unique_ptr<node::Node> NodePtr_t;
    typedef std::unique_ptr<node::Attr> AttrPtr_t;
    typedef std::list<node::Node *>     NodeList_t;
    typedef std::vector<NodePtr_t>           Nodes_t;
    typedef std::vector<node::Attr *>   Attributes_t;
    typedef std::vector<std::unique_ptr<node::Element>>  HTMLCollection_t;
}

#endif //BLOGATOR_PARSER_DOM_DEFS_H