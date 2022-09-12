#ifndef BLOGATOR_CONFIGURATION_PARSER_DTO_VALUECACHE_H
#define BLOGATOR_CONFIGURATION_PARSER_DTO_VALUECACHE_H

#include <deque>

#include "../../dto/Value.h"
#include "../../../dto/TextPos.h"

namespace blogator::configuration::parser {
    /**
     * @param position root value position in source text
     * @param root Root array or atomic value
     * @param hierarchy Value hierarchy (helps track nested arrays)
     * @param count Cached Value counter
     */
    struct ValueCache {
        ValueCache() = default;
        ValueCache( Value && root, TextPos position );

        bool convertCurrentContextToArray();
        bool createArray();
        bool closeArray();
        void addValueToCurrentContext( Value && value, const TextPos &pos );

        [[nodiscard]] Value::Type_e rootType() const;
        [[nodiscard]] bool initialised() const;

        TextPos                                   position;
        std::unique_ptr<Value>                    root;
        std::deque<std::reference_wrapper<Value>> hierarchy;
    };
}

#endif //BLOGATOR_CONFIGURATION_PARSER_DTO_VALUECACHE_H