#ifndef BLOGATOR_CONFIGURATION_DTO_CONFIGURATION_H
#define BLOGATOR_CONFIGURATION_DTO_CONFIGURATION_H

#include <string>
#include <memory>
#include <map>
#include <vector>

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
        typedef uint8_t                     Flags_t;

        static constexpr uint8_t ARRAY_VALUE_FLAG  = Value::IS_ARRAY_VALUE;
        static constexpr uint8_t NAMED_VALUE_FLAG  = Value::IS_NAMED_VALUE;
        static constexpr uint8_t APPEND_VALUE_FLAG = 0b100;

        Configuration() = default;

        bool operator ==( const Configuration & rhs ) const;
        bool operator !=( const Configuration & rhs ) const;

        friend std::ostream & operator <<( std::ostream & os, const Configuration & cfg );

        Value & add( const Key_t & ns_key, Value && value );
        Value & add( const Key_t & ns_key, Flags_t flags = 0 );
        Value & add( const Key_t & ns_key, bool val, Flags_t flags = 0 );
        Value & add( const Key_t & ns_key, int64_t val, Flags_t flags = 0 );
        Value & add( const Key_t & ns_key, double val, Flags_t flags = 0 );
        Value & add( const Key_t & ns_key, const char32_t * val, Flags_t flags = 0 );
        Value & add( const Key_t & ns_key, const std::u32string & val, Flags_t flags = 0 );

        Value * find( const Key_t & ns_key );
        Configuration findAll( const Key_t & ns_key );
        size_t remove( const Key_t & ns_key );
        Configuration copy( const Key_t & ns );

        [[nodiscard]] bool exists( const Key_t & ns_key ) const;
        [[nodiscard]] std::vector<std::pair<std::string, std::string>> printToCollection( const std::set<Value::Type_e> & quoted_types = {} ) const;
        [[nodiscard]] size_t size() const;
        [[nodiscard]] bool empty() const;

        static Key_t convert( std::u32string ns_key );
        static std::u32string convert( const Key_t & ns_key );
        static bool validate( const std::u32string & str );
        static bool validate( const Key_t & ns_key );

      private:
        ConfigurationNode _root;

      private:
        explicit Configuration( ConfigurationNode  root );

        std::pair<ConfigurationNode &, bool> createNode( const Key_t & key );
        ConfigurationNode * findNode( const Key_t & key );
        size_t remove( ConfigurationNode * parent, Key_t::const_iterator begin, Key_t::const_iterator end );
        void accumulate( Configuration & cfg, ConfigurationNode * src_parent, const Key_t & ns_root, Key_t::const_iterator begin, Key_t::const_iterator end );
        void printAll( std::vector<std::string> & prefix, const ConfigurationNode * node, std::ostream & out ) const;
        void printAll( std::vector<std::string> & prefix, const ConfigurationNode * node, std::vector<std::pair<std::string, std::string>> & out, const std::set<Value::Type_e> & quoted_types = {} ) const;
    };

    std::ostream & operator <<( std::ostream & os, const Configuration & cfg );
    std::ostream & operator <<( std::ostream & os, const Configuration::Key_t & key );
}

namespace blogator {
    std::string to_string( const blogator::configuration::Configuration & cfg );
    std::string to_string( const blogator::configuration::Configuration::Key_t & key );
}

#endif //BLOGATOR_CONFIGURATION_DTO_CONFIGURATION_H