#ifndef BLOGATOR_CONFIGURATION_PARSER_DTO_NAMEDVALUES_H
#define BLOGATOR_CONFIGURATION_PARSER_DTO_NAMEDVALUES_H

#include "../../../dto/TextPos.h"
#include "../../dto/Configuration.h"

namespace blogator::configuration::parser {
    /**
     * NamedValue container
     * @param nsk Namespaced Key for the named value
     * @param value_refs Collection of all pending values inside the Configuration for the named value
     */
    struct NamedValues {
        /**
         * Configuration value reference wrapper
         * @param src_pos Position of 'named value' in the source text (for logging purposes)
         * @param val_ref Reference to the value object in the Configuration for the key
         */
        struct ValueRef {
            bool operator ==( const ValueRef & rhs ) const;

            TextPos                       pos;
            std::reference_wrapper<Value> ref;
        };

        Configuration::Key_t   nsk;
        std::vector<ValueRef>  values;
    };
}

#endif //BLOGATOR_CONFIGURATION_PARSER_DTO_NAMEDVALUES_H
