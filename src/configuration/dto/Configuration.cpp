#include "Configuration.h"

#include <stack>
#include <vector>
#include <stdexcept>

#include "../../unicode/ascii.h"
#include "../../unicode/utf8.h"
#include "../../string/helpers.h"
#include "../../logger/Logger.h"

using namespace blogator::configuration;

/**
 * [PRIVATE] Constructor
 * @param root Root node
 */
Configuration::Configuration( ConfigurationNode root ) :
    _root( std::move( root ) )
{
    _root.clearSuffix(); //root doesn't have a suffix
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
 * @param value Value object to set the value as
 * @return Reference to the inserted value
 */
Value & Configuration::add( const Configuration::Key_t &ns_key, Value && value ) { //TODO unit test
    auto [ node, created ] = createNode( ns_key );
    return node.setValue( std::move( value ) );
}

/**
 * Adds a namespaced key-value pair with no value set
 * @param ns_key Namespaced key
 * @param flags Value flags indicating property of the value added (default: 0x00)
 * @return Reference to the inserted value
 */
Value & Configuration::add( const Configuration::Key_t & ns_key, Flags_t flags ) {
    auto [ node, created ] = createNode( ns_key );

    return ( ( flags & APPEND_VALUE_FLAG ) || ( node.isKey() && node.value.type() == Value::Type_e::ARRAY )
             ? node.appendValue( Value( flags ) )
             : node.setValue( Value( flags ) ) );
}

/**
 * Adds a namespaced key-value pair
 * @param ns_key Namespaced key
 * @param val Value
 * @param flags Value flags indicating property of the value added (default: 0x00)
 * @return Reference to the inserted value
 */
Value & Configuration::add( const Configuration::Key_t & ns_key, bool val, Flags_t flags ) {
    auto [ node, created ] = createNode( ns_key );

    return ( ( flags & APPEND_VALUE_FLAG ) || ( node.isKey() && node.value.type() == Value::Type_e::ARRAY )
             ? node.appendValue( Value( val, flags ) )
             : node.setValue( Value( val, flags ) ) );
}

/**
 * Adds a namespaced key-value pair
 * @param ns_key Namespaced key
 * @param val Value
 * @param flags Value flags indicating property of the value added (default: 0x00)
 * @return Reference to the inserted value
 */
Value & Configuration::add( const Configuration::Key_t & ns_key, int64_t val, Flags_t flags ) {
    auto [ node, created ] = createNode( ns_key );

    return ( ( flags & APPEND_VALUE_FLAG ) || ( node.isKey() && node.value.type() == Value::Type_e::ARRAY )
             ? node.appendValue( Value( val, flags ) )
             : node.setValue( Value( val, flags ) ) );
}

/**
 * Adds a namespaced key-value pair
 * @param ns_key Namespaced key
 * @param val Value
 * @param flags Value flags indicating property of the value added (default: 0x00)
 * @return Reference to the inserted value
 */
Value & Configuration::add( const Configuration::Key_t & ns_key, double val, Flags_t flags ) {
    auto [ node, created ] = createNode( ns_key );

    return ( ( flags & APPEND_VALUE_FLAG ) || ( node.isKey() && node.value.type() == Value::Type_e::ARRAY )
             ? node.appendValue( Value( val, flags ) )
             : node.setValue( Value( val, flags ) ) );
}

/**
 * Adds a namespaced key-value pair
 * @param ns_key Namespaced key
 * @param val Value
 * @param flags Value flags indicating property of the value added (default: 0x00)
 * @return Reference to the inserted value
 */
Value & Configuration::add( const Configuration::Key_t & ns_key, const char32_t * val, Flags_t flags ) {
    return add( ns_key, std::u32string( val ), flags );
}

/**
 * Adds a namespaced key-value pair
 * @param ns_key Namespaced key
 * @param val Value
 * @param flags Value flags indicating property of the value added (default: 0x00)
 * @return Reference to the inserted value
 */
Value & Configuration::add( const Configuration::Key_t & ns_key, const std::u32string & val, Flags_t flags ) {
    auto [ node, created ] = createNode( ns_key );

    return ( ( flags & APPEND_VALUE_FLAG ) || ( node.isKey() && node.value.type() == Value::Type_e::ARRAY )
             ? node.appendValue( Value( val, flags ) )
             : node.setValue( Value( val, flags ) ) );
}

/**
 * Find the value for a namespaced key
 * @param ns_key Namespaced key
 * @return Pointer to the value associated with key (or nullptr when namespaced key-value does not exist - i.e. just a plain namespace)
 */
Value * Configuration::find( const Configuration::Key_t & ns_key ) {
    auto * node = findNode( ns_key );
    return ( node != nullptr && node->isKey() ? &node->getValue() : nullptr );
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
 * Checks if namespaced key with a value already exists
 * @param ns_key Namespaced key
 * @return Key-Value existence state
 */
bool Configuration::exists( const Configuration::Key_t & ns_key ) const {
    auto * node = const_cast<Configuration *>( this )->findNode( ns_key );
    return ( node != nullptr && node->isKey() );
}

/**
 * Prints key-value pairs to a container
 * @param quoted_types Set of Value types whose value are to be double quoted (default: none)
 * @return Collection of UTF-8 encoded Key-Value pairs
 */
std::vector<std::pair<std::string, std::string>> Configuration::printToCollection( const std::set<Value::Type_e> & quoted_types ) const {
    auto suffix     = std::vector<std::string>();
    auto collection = std::vector<std::pair<std::string, std::string>>();
    this->printAll( suffix, &_root, collection, quoted_types );
    return std::move( collection );
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
std::pair<ConfigurationNode &, bool> Configuration::createNode( const Configuration::Key_t & key ) {
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
            parent->value = Value();
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
 * [PRIVATE] Recursively accumulate keys and namespaces into a Configuration object
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

            target_parent.is_key = src_parent->is_key;
            target_parent.value  = src_parent->value;

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
 * [PRIVATE] Prints all nodes in the subtree
 * @param prefix Prefix(es) of subtree
 * @param node Root node of subtree
 * @param out Output stream
 */
void Configuration::printAll( std::vector<std::string> & prefix, const ConfigurationNode * node, std::ostream & out ) const {
    if( node->value.type() != Value::Type_e::VOID ) {
        out << ":";

        for( const auto & p: prefix ) {
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
 * [PRIVATE] Prints all nodes in the subtree to a collection of Key-Value pairs
 * @param prefix Prefix(es) of subtree
 * @param node Root node of subtree
 * @param out Output KV collection
 * @param quoted_types Set of Value types whose value are to be double quoted (default: none)
 */
void Configuration::printAll( std::vector<std::string> & prefix, const ConfigurationNode * node, std::vector<std::pair<std::string, std::string>> & out, const std::set<Value::Type_e> & quoted_types ) const {
    if( node->isKey() ) {
        std::stringstream k_ss;
        std::stringstream v_ss;

        k_ss << ":";

        for( const auto & p: prefix ) {
            k_ss << p << ":";
        }

        k_ss << blogator::unicode::utf8::convert( node->suffix );

        Value::print( v_ss, node->value, quoted_types );

        out.emplace_back( std::make_pair( k_ss.str(), v_ss.str() ) );
    }

    if( !node->isLeaf() ) {
        if( node != &_root ) { //<- deals with 'root' edge case
            prefix.emplace_back( blogator::unicode::utf8::convert( node->suffix ) );
        }

        for( const auto & [k, v] : node->children ) {
            printAll( prefix, &v, out, quoted_types );
        }

        if( node != &_root ) {
            prefix.pop_back();
        }
    }
}


/**
 * Converts a namespace key formatted string into a namespace key structure (e.g.: "ns:key" -> ["ns"]["key"])
 * @param ns_key Namespaced key string
 * @return Configuration Key
 * @throws std::invalid_argument when the namespaced key string is invalid
 */
Configuration::Key_t Configuration::convert( std::u32string ns_key ) {
    for( auto & c : ns_key ) {
        if( unicode::ascii::isupper( c ) ) {
            c = unicode::ascii::tolower( c );
        }
    }

    auto nsk_struct = string::split<char32_t>( ns_key, U':', true );

    if( !Configuration::validate( nsk_struct ) ) {
        LOG_ERROR(
            "[configuration::Configuration::convert( \"", unicode::utf8::convert( ns_key ), "\" )] "
            "Invalid namespaced key format"
        );

        throw std::invalid_argument( "invalid char in namespaced key string" );
    }

    return std::move( nsk_struct );
}

/**
 * Converts a namespace key structure into a formatted namespaced key string (e.g.: ["ns"]["key"] -> "ns:key")
 * @param ns_key Namespaced key structure (unchecked)
 * @return Namespace key string
 */
std::u32string Configuration::convert( const Configuration::Key_t &ns_key ) {
    auto str = std::u32string();

    str.append( 1, U':' );

    for( size_t i = 0; i < ns_key.size(); ++i ) {
        str.append( ns_key.at( i ) );

        if( i != ( ns_key.size() - 1 ) ) {
            str.append( 1, U':' );
        }
    }

    return std::move( str );
}

/**
 * Checks string is formatted correctly to be a namespace or key part
 * @param str String to check
 * @return Validation state
 */
bool Configuration::validate( const std::u32string &str ) {
    auto it = str.cbegin();

    if( it != str.cend() && *it == unicode::UNDERSCORE ) {
        while( it != str.cend() && *(it++) == unicode::UNDERSCORE );
    }

    if( it != str.cend() && unicode::ascii::islower( *it ) ) {
        while( it++ != str.cend() && ( unicode::ascii::islower( *it ) || unicode::ascii::isdigit( *it ) || *it == unicode::UNDERSCORE ) );

        return ( it == str.cend() ); //EARLY RETURN
    }

    return false;
}

/**
 * Checks namespaced key parts are correctly formatted
 * @param ns_key Namespaced key
 * @return Validation state
 */
bool Configuration::validate( const Configuration::Key_t &ns_key ) {
    for( const auto & part : ns_key ) {
        if( !Configuration::validate( part ) ) {
            return false; //EARLY RETURN
        }
    }

    return true;
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

/**
 * Output stream operator
 * @param os Output stream
 * @param key Configuration::Key_t object
 * @return Output stream
 */
std::ostream & blogator::configuration::operator <<( std::ostream &os, const Configuration::Key_t & key ) {
    os << ":";

    for( size_t i = 0; i < key.size(); ++i ) {
        os << unicode::utf8::convert( key.at( i ) );

        if( i < ( key.size() - 1 ) ) {
            os << ":";
        }
    }

    return os;
}

/**
 * Converts a configuration::Configuration object to a string representation
 * @param type Configuration object
 * @return String representation
 */
std::string blogator::to_string( const Configuration &cfg ) {
    std::stringstream ss;
    ss << cfg;
    return ss.str();
}

/**
 * Converts a configuration::Configuration::Key_t object to a string representation
 * @param type Configuration::Key_t object
 * @return String representation
 */
std::string blogator::to_string( const Configuration::Key_t &key ) {
    std::stringstream ss;
    ss << key;
    return ss.str();
}
