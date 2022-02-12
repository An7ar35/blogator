#ifndef BLOGATOR_PARSER_DOM_DATASTRUCTURE_NAMESPACEMAP_H
#define BLOGATOR_PARSER_DOM_DATASTRUCTURE_NAMESPACEMAP_H

#include <map>
#include <vector>
#include <thread>

#include "../defs.h"
#include "../../specs/html5/Namespace.h"

namespace blogator::parser::dom {
    /**
     * DOM Namespace mapping (thread-safe)
     *
     * This is meant to hold and share all namespace URI strings between DOM documents
     * so that each individual node that require namespace designation can fetch based
     * on an ID (integer) instead of having multiple instances of strings spread throughout.
     */
    class NamespaceMap {
      public:
        typedef int id_t;

        id_t setNamespace( specs::html5::Namespace ns );
        id_t setNamespace( const DOMString_t &ns, DOMString_t prefix = DOMString_t() );

        [[nodiscard]] specs::html5::Namespace getNamespaceEnum( id_t id ) const;
        [[nodiscard]] const DOMString_t & getNamespaceURI( id_t id ) const;
        [[nodiscard]] const DOMString_t & getNamespacePrefix( id_t id ) const;
        [[nodiscard]] size_t size() const;
        [[nodiscard]] bool empty() const;

        static constexpr int INVALID = -1;
        static constexpr int NONE    = -1;

      private:
        struct NS {
            DOMString_t             uri;
            DOMString_t             prefix;
            specs::html5::Namespace ns_enum;
        };

        mutable std::mutex                      _mutex;
        std::vector<NS>                         _namespaces;
        std::map<DOMString_t,             id_t> _str_mapping;
        std::map<specs::html5::Namespace, id_t> _enum_mapping;
    };
}

#endif //BLOGATOR_PARSER_DOM_DATASTRUCTURE_NAMESPACEMAP_H