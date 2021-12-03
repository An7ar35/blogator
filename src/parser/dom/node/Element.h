#ifndef BLOGATOR_PARSER_DOM_NODE_ELEMENT_H
#define BLOGATOR_PARSER_DOM_NODE_ELEMENT_H

#include "Node.h"

namespace blogator::parser::dom::node {
    class Element : public Node {
      public:
        //TODO

      private:
        Nodes_t  _children;
    };
}

#endif //BLOGATOR_PARSER_DOM_NODE_ELEMENT_H