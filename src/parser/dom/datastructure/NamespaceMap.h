#ifndef BLOGATOR_PARSER_DOM_DATASTRUCTURE_NAMESPACEMAP_H
#define BLOGATOR_PARSER_DOM_DATASTRUCTURE_NAMESPACEMAP_H

#include <map>
#include <vector>
#include <thread>

#include "../defs.h"
#include "../../specs/infra/Namespace.h"

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
        typedef NamespaceMapID_t id_t;

        NamespaceMap();

        id_t setNamespace( specs::infra::Namespace ns );
        id_t setNamespace( const DOMString_t &ns, DOMString_t prefix = DOMString_t() );

        [[nodiscard]] specs::infra::Namespace getNamespaceEnum( id_t id ) const;
        [[nodiscard]] const DOMString_t & getNamespaceURI( id_t id ) const;
        [[nodiscard]] const DOMString_t & getNamespacePrefix( id_t id ) const;
        [[nodiscard]] id_t getID( specs::infra::Namespace ns ) const;
        [[nodiscard]] id_t getID( const DOMString_t &ns ) const;
        [[nodiscard]] size_t size() const;
        [[nodiscard]] bool empty() const;

        static constexpr int NONE    =  0;
        static constexpr int INVALID = -1;

      private:
        struct NS {
            DOMString_t             uri;
            DOMString_t             prefix;
            specs::infra::Namespace ns_enum;
        };

        mutable std::mutex                      _mutex;
        std::vector<NS>                         _namespaces;
        std::map<DOMString_t,             id_t> _str_mapping;
        std::map<specs::infra::Namespace, id_t> _enum_mapping;
    };
}

#endif //BLOGATOR_PARSER_DOM_DATASTRUCTURE_NAMESPACEMAP_H