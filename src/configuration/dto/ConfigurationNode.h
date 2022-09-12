#ifndef BLOGATOR_CONFIGURATION_DTO_CONFIGURATIONNODE_H
#define BLOGATOR_CONFIGURATION_DTO_CONFIGURATIONNODE_H

#include <map>
#include <string>
#include <ostream>

#include "Value.h"

namespace blogator::configuration {
    /**
     * Configuration namespace/key node
     * @param children Child namespaces/key
     * @param value Value
     */
    struct ConfigurationNode {
        typedef std::map<std::u32string, ConfigurationNode> NodeMap_t;
        typedef NodeMap_t::iterator                         iterator;
        typedef NodeMap_t::const_iterator                   const_iterator;

        ConfigurationNode() = default;
        explicit ConfigurationNode( std::u32string suffix );
        ConfigurationNode( ConfigurationNode && node ) noexcept ;
        ConfigurationNode( const ConfigurationNode & node );

        friend std::ostream & operator <<( std::ostream & os, const ConfigurationNode & node );

        ConfigurationNode & operator =( ConfigurationNode && node ) noexcept ;
        ConfigurationNode & operator =( const ConfigurationNode & node );

        bool operator ==( const ConfigurationNode & rhs ) const;
        bool operator !=( const ConfigurationNode & rhs ) const;

        void clearSuffix();
        Value & getValue();
        Value & setValue( Value && val );
        Value & appendValue( Value && val );

        [[nodiscard]] bool isLeaf() const;
        [[nodiscard]] bool isKey() const;
        [[nodiscard]] ConfigurationNode copy() const;
        [[nodiscard]] size_t size() const;

        NodeMap_t      children;
        std::u32string suffix;
        bool           is_key { false };
        Value          value;
    };

    std::ostream & operator <<( std::ostream & os, const ConfigurationNode & node );
}

#endif //BLOGATOR_CONFIGURATION_DTO_CONFIGURATIONNODE_H
