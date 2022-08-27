#ifndef BLOGATOR_CONFIGURATION_DTO_CONFIGURATION_H
#define BLOGATOR_CONFIGURATION_DTO_CONFIGURATION_H

#include <string>
#include <memory>
#include <map>
#include <vector>

#include "Value.h"
#include "ConfigurationNode.h"

namespace blogator::configuration {
    /**
     * Configuration container
     *
     * Key_t format (findAll/remove)\n
     * ----------------------------------\n
     * · ""         : does nothing\n
     * · "*"        : everything\n
     * · "ns:*"     : all key/children within a namespace\n
     * · "key"      : root key/namespace\n
     * · "ns:key"   : namespaced key\n
     * · "ns:*:key" : matching keys in any namespace inside the namespace 'ns'\n
     * · "*:key"    : 'key' child inside any namespace in root\n
     * · "*:ns:*    : everything in 'ns' child of any namespaces in root\n
     */
    class Configuration {
      public:
        typedef std::vector<std::u32string> Key_t;

        Configuration() = default;

        bool operator ==( const Configuration & rhs ) const;
        bool operator !=( const Configuration & rhs ) const;

        friend std::ostream & operator <<( std::ostream & os, const Configuration & cfg );

        bool add( const Key_t & ns_key, bool val );
        bool add( const Key_t & ns_key, int64_t val );
        bool add( const Key_t & ns_key, double val );
        bool add( const Key_t & ns_key, const char32_t * val, bool is_name = false );
        bool add( const Key_t & ns_key, const std::u32string & val, bool is_name = false );

        ValueStore * find( const Key_t & ns_key );
        Configuration findAll( const Key_t & ns_key );
        size_t remove( const Key_t & ns_key );
        Configuration copy( const Key_t & ns );

        [[nodiscard]] size_t size() const;
        [[nodiscard]] bool empty() const;

      private:
        ConfigurationNode _root;

      private:
        explicit Configuration( ConfigurationNode  root );

        std::pair<ConfigurationNode &, bool> createNode( const Key_t & key );
        ConfigurationNode * findNode( const Key_t & key );
        size_t remove( ConfigurationNode * parent, Key_t::const_iterator begin, Key_t::const_iterator end );
        void accumulate( Configuration & cfg, ConfigurationNode * src_parent, const Key_t & ns_root, Key_t::const_iterator begin, Key_t::const_iterator end );
        void printAll( std::vector<std::string> & prefix, const ConfigurationNode * node, std::ostream & out ) const;
    };

    std::ostream & operator <<( std::ostream & os, const Configuration & cfg );
}

#endif //BLOGATOR_CONFIGURATION_DTO_CONFIGURATION_H