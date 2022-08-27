#include "ConfigurationNode.h"

#include "../../unicode/utf8.h"

using namespace blogator::configuration;

/**
 * Constructor
 * @param suffix Node suffix string
 */
ConfigurationNode::ConfigurationNode( std::u32string suffix ) :
    suffix( std::move( suffix ) )
{}

/**
 * Move-constructor
 * @param node Node to move
 */
ConfigurationNode::ConfigurationNode( ConfigurationNode &&node ) noexcept :
    suffix( std::move( node.suffix ) ),
    is_key( node.is_key ),
    values( std::move( node.values ) ),
    children( std::move( node.children ) )
{}

/**
 * Copy-constructor
 * @param node Node to copy
 */
ConfigurationNode::ConfigurationNode( const ConfigurationNode & node ) :
    suffix( node.suffix ),
    is_key( node.is_key )
{
    for( auto &val_ptr: node.values() ) {
        if( val_ptr ) {
            this->values.emplace_back( std::make_unique<Value>( *val_ptr ) );
        } else {
            this->values.emplace_back( nullptr );
        }
    }

    for( const auto & child : node.children ) {
        this->children.emplace( child.first, child.second.copy() );
    }
}

/**
 * Move-operator
 * @param node Node to move
 * @return Moved node
 */
ConfigurationNode & ConfigurationNode::operator =( ConfigurationNode &&node ) noexcept {
    suffix = std::move( node.suffix );
    is_key = node.is_key;

    for( auto &val_ptr: node.values() ) {
        if( val_ptr ) {
            this->values.emplace_back( std::move( val_ptr ) );
        } else {
            this->values.emplace_back( nullptr );
        }
    }

    this->children = std::move( node.children );

    return *this;
}

/**
 * Copy-operator
 * @param node Node to copy
 * @return Copied node
 */
ConfigurationNode & ConfigurationNode::operator =( const ConfigurationNode & node ) {
    suffix = node.suffix;
    is_key = node.is_key;

    for( const auto &val_ptr: node.values() ) {
        if( val_ptr ) {
            this->values.emplace_back( std::make_unique<Value>( *val_ptr ) );
        } else {
            this->values.emplace_back( nullptr );
        }
    }

    for( const auto & child : node.children ) {
        this->children.emplace( child.first, child.second.copy() );
    }

    return *this;
}

/**
 * Equality operator
 * @param rhs ConfigurationNode to compare with
 * @return Equality state of subtree
 */
bool ConfigurationNode::operator ==( const ConfigurationNode &rhs ) const {
    return this->suffix   == rhs.suffix
        && this->is_key   == rhs.is_key
        && this->values   == rhs.values
        && this->children == rhs.children;
}

/**
 * Inequality operator
 * @param rhs ConfigurationNode to compare with
 * @return Inequality state of subtree
 */
bool ConfigurationNode::operator !=( const ConfigurationNode &rhs ) const {
    return !( *this == rhs );
}

/**
 * Clears the suffix for the node
 */
void ConfigurationNode::clearSuffix() {
    suffix.clear();
}

/**
 * Gets a pointer to the value store
 * @return Pointer to value store or nullptr when node is not a key
 */
ValueStore * ConfigurationNode::value() {
    return ( isKey() ? &values : nullptr );
}

/**
 * Add a value to the value store
 * @param val Value pointer
 */
void ConfigurationNode::addValue( std::unique_ptr<Value> &&val ) {
    values.emplace_back( std::move( val ) );

    if( !is_key ) {
        is_key = true;
    }
}

/**
 * Copy node recursively
 * @return Copy of node
 */
ConfigurationNode ConfigurationNode::copy() const {
    return { *this };
}

/**
 * Checks if node is a leaf (i.e.: a key)
 * @return Leaf state
 */
bool ConfigurationNode::isLeaf() const {
    return children.empty();
}

/**
 * Checks the key flag for the node
 * @return Key-value state
 */
bool ConfigurationNode::isKey() const {
    return is_key;
}

/**
 * Gets the number of keys in the subtree
 * @return Number of keys with values in subtree
 */
size_t ConfigurationNode::size() const {
    size_t size = ( isKey() ? 1 : 0 );

    for( const auto & [k, v] : children ) {
        size += v.size();
    }

    return size;
}

/**
 * Output stream operator
 * @param os Output stream
 * @param node configuration:ConfigurationNode to print out
 * @return Output stream
 */
std::ostream & blogator::configuration::operator <<( std::ostream &os, const ConfigurationNode &node ) {
    if( node.isKey() ) {
        os << blogator::unicode::utf8::convert( node.suffix ) << " = " << node.values;
    } else {
        os << blogator::unicode::utf8::convert( node.suffix );
    }

    return os;
}