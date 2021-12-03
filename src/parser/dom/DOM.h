#ifndef BLOGATOR_PARSER_DOM_DOM_H
#define BLOGATOR_PARSER_DOM_DOM_H

#include <string>
#include <list>
#include <unordered_map>

#include "node/Node.h"

namespace blogator::parser::dom {
    class DOM {
      public:

        NodeList_t getElementsByTagName( const std::u32string & tag_name );

      private:

    };
}

#endif //BLOGATOR_PARSER_DOM_DOM_H