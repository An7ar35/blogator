#include "Configuration.h"

#include <stack>
#include <vector>

#include "../../unicode/utf8.h"

using namespace blogator::configuration;

/**
 * [PRIVATE] Constructor
 * @param root Root node
 */
Configuration::Configuration( ConfigurationNode root ) :
    _root( std::move( root ) )
{
    _root.suffix.clear(); //root doesn't have a suffix
}

/**
 * Equality operator
 * @param rhs Configuration to compare
 * @return Equality state
 */
bool Configuration::operator ==( const Configuration &rhs ) const {
    return this->_root == rhs._root;
}

/**
 * Inequality operator
 * @param rhs Configuration to compare
 * @return Inequality state
 */
bool Configuration::operator !=( const Configuration &rhs ) const {
    return !( *this == rhs );
}

/**
 * Adds a namespaced key-value pair
 * @param ns_key Namespaced key
 * @param val Value
 * @return Key-value insertion state
 */
bool Configuration::add( const Configuration::Key_t & ns_key, bool val ) {
    auto [ node, created ] = createNode( ns_key );
    node.value = std::make_unique<Value>( val );
    return created;
}

/**
 * Adds a namespaced key-value pair
 * @param ns_key Namespaced key
 * @param val Value
 * @return Key-value insertion state
 */
bool Configuration::add( const Configuration::Key_t & ns_key, int64_t val ) {
    auto [ node, created ] = createNode( ns_key );
    node.value = std::make_unique<Value>( val );
    return created;
}

/**
 * Adds a namespaced key-value pair
 * @param ns_key Namespaced key
 * @param val Value
 * @return Key-value insertion state
 */
bool Configuration::add( const Configuration::Key_t & ns_key, double val ) {
    auto [ node, created ] = createNode( ns_key );
    node.value = std::make_unique<Value>( val );
    return created;
}

/**
 * Adds a namespaced key-value pair
 * @param ns_key Namespaced key
 * @param val Value
 * @param is_name Flag to signal that the string is a named reference
 * @return Key-value insertion state
 */
bool Configuration::add( const Configuration::Key_t & ns_key, const char32_t * val, bool is_name ) {
    return add( ns_key, std::u32string( val ), is_name );
}

/**
 * Adds a namespaced key-value pair
 * @param ns_key Namespaced key
 * @param val Value
 * @param is_name Flag to signal that the string is a named reference
 * @return Key-value insertion state
 */
bool Configuration::add( const Configuration::Key_t & ns_key, const std::u32string & val, bool is_name ) {
    auto [ node, created ] = createNode( ns_key );
    node.value = std::make_unique<Value>( val, is_name );
    return created;
}

/**
 * Find the value for a namespaced key
 * @param ns_key Namespaced key
 * @return Pointer to value associated with key (or nullptr when namespaced key or value does not exist)
 */
Value * Configuration::find( const Configuration::Key_t & ns_key ) {
    auto * node = findNode( ns_key );
    return ( node == nullptr ? nullptr : node->value.get() );
}

/**
 * Find namespaced values
 * @param ns_key Namespaced key
 * @return Configuration object containing all namespaced key-value pairs found
 */
Configuration Configuration::findAll( const Configuration::Key_t &ns_key ) {
    auto cfg = Configuration();
    accumulate( cfg, &_root, {}, ns_key.cbegin(), ns_key.cend() );
    return cfg;
}

/**
 * Removes a namespaced key-value pair
 * @param ns_key Namespaced key ('*': any ns/key)
 * @return Number of keys deleted
 */
size_t Configuration::remove( const Configuration::Key_t & ns_key ) {
    return remove( &_root, ns_key.cbegin(), ns_key.cend() );
}

/**
 * Gets the configuration within a namespace
 * @param ns_key Namespace (root in the returned Configuration)
 * @return Configuration in namespace
 */
Configuration Configuration::copy( const Configuration::Key_t & ns ) {
    const auto * root = findNode( ns );
    return ( root == nullptr ? Configuration() : Configuration( *root ) );
}

/**
 * Gets the number of keys in the configuration
 * @return Key count
 */
size_t Configuration::size() const {
    return _root.size();
}

/**
 * Gets the empty state of the configuration store
 * @return Empty state
 */
bool Configuration::empty() const {
    return ( _root.size() == 0 );
}

/**
 * [PRIVATE] Gets the node (or creates it if not found) for a given namespaced key
 * @param key Namespaced key
 * @return Key node and insertion state in a pair
 */
std::pair<ConfigurationNode &, bool> Configuration::createNode( const Configuration::Key_t &key ) {
    if( key.empty() ) {
        return { _root, false };
    }

    const size_t suffix_i = ( key.size() - 1 );

    //iterate through prefixing namespaces and add when absent (i.e.: `[ns0][ns1]...[nsN-1]`)
    ConfigurationNode * curr_node = &_root;

    for( size_t prefix_i = 0; prefix_i < suffix_i; ++prefix_i ) {
        auto [it, inserted] = curr_node->children.try_emplace( key.at( prefix_i ), ConfigurationNode( key.at( prefix_i ) ) );

        curr_node = &it->second;
    }

    //add key to last parent namespace map (i.e.: `ns0:ns1:...:nsN-1:[KEY]`)
    auto [it, inserted] = curr_node->children.try_emplace( key.at( suffix_i ), ConfigurationNode( key.at( suffix_i ) ) );

    return { it->second, inserted };
}

/**
 * [PRIVATE] Gets a namespaced node
 * @param ns_key Namespaced key
 * @return Pointer to node or nullptr when ns entry does not exist
 */
ConfigurationNode * Configuration::findNode( const Configuration::Key_t & ns_key ) {
    ConfigurationNode * curr_node = &_root;

    for( const auto & name : ns_key ) {
        auto it = curr_node->children.find( name );

        if( it == curr_node->children.end() ) {
            return nullptr; //EARLY RETURN
        }

        curr_node = &it->second;
    }

    return curr_node;
}

/**
 * [PRIVATE] Remove node(s)
 * @param parent Current parent node
 * @param begin Current namespace iterator
 * @param end End namespace iterator
 * @return Number of nodes removed
 */
size_t Configuration::remove( ConfigurationNode * parent, Key_t::const_iterator begin, Key_t::const_iterator end ) {
    size_t rm_count = 0;

    if( begin == end ) {
        return rm_count;  //EARLY RETURN;
    }

    if( std::next( begin ) == end ) { //i.e.: last
        if( *begin == U"*" ) {
            rm_count = parent->size();
            parent->value.reset();
            parent->children.clear();

        } else {
            auto it = parent->children.find( *begin );

            if( it != parent->children.end() ) {
                rm_count = it->second.size();
                parent->children.erase( it );
            }
        }

    } else if( *begin == U"*" ) { //i.e.: any namespace
        for( auto & [k, v] : parent->children ) {
            rm_count += remove( &v, std::next( begin ), end );
        }

    } else {
        auto it = parent->children.find( *begin );

        if( it != parent->children.end() ) {
            return remove( &it->second, std::next( begin ), end ); //EARLY RETURN;
        }
    }

    return rm_count;
}

/**
 * Recursively accumulate keys and namespaces into a Configuration object
 * @param cfg Target Configuration object
 * @param src_parent Source root of subtree
 * @param root_ns Source root namespace prefix
 * @param begin Current namespace iterator
 * @param end End namespace iterator
 */
void Configuration::accumulate( Configuration & cfg, ConfigurationNode * src_parent, const Key_t & root_ns, Key_t::const_iterator begin, Key_t::const_iterator end ) {
    if( begin == end ) {
        return;  //EARLY RETURN;
    }

    if( std::next( begin ) == end ) { //i.e.: last
        if( *begin == U"*" ) {
            auto [ target_parent, created ] = cfg.createNode( root_ns );

            for( const auto & [k, v] : src_parent->children ) {
                target_parent.children.emplace( k, v );
            }

            if( src_parent->hasValue() ) {
                target_parent.value = std::make_unique<Value>( *src_parent->value );
            }

        } else {
            auto it = src_parent->children.find( *begin );

            if( it != src_parent->children.end() ) {
                auto [ cfg_node, created ] = cfg.createNode( root_ns );
                cfg_node.children.emplace( it->first, it->second );
            }
        }

    } else if( *begin == U"*" ) { //i.e.: any namespace
        for( auto & [k, v] : src_parent->children ) {
            auto ns_key = root_ns;
            ns_key.emplace_back( k );
            accumulate( cfg, &v, ns_key, std::next( begin ), end );
        }

    } else {
        auto it = src_parent->children.find( *begin );

        if( it != src_parent->children.end() ) {
            auto ns_key = root_ns;
            ns_key.emplace_back( it->first );
            accumulate( cfg, &it->second, ns_key, std::next( begin ), end );
        }
    }
}

/**
 * Prints all nodes in the subtree
 * @param prefix Prefix(es) of subtree
 * @param node Root node of subtree
 * @param out Output stream
 */
void Configuration::printAll( std::vector<std::string> & prefix, const ConfigurationNode * node, std::ostream & out ) const {
    if( node->hasValue() ) {
        out << ":";

        for( const auto &p: prefix ) {
            out << p << ":";
        }

        out << *node << "\n";
    }

    if( !node->isLeaf() ) {
        if( node != &_root ) { //<- deals with 'root' edge case
            prefix.emplace_back( blogator::unicode::utf8::convert( node->suffix ) );
        }

        for( const auto & [k, v] : node->children ) {
            printAll( prefix, &v, out );
        }

        if( node != &_root ) {
            prefix.pop_back();
        }
    }
}

/**
 * Output stream operator
 * @param os Output stream
 * @param cfg Configuration object to print
 * @return Output stream
 */
std::ostream & blogator::configuration::operator <<( std::ostream &os, const Configuration &cfg ) {
    auto suffix = std::vector<std::string>();
    cfg.printAll( suffix, &cfg._root, os );
    return os;
}