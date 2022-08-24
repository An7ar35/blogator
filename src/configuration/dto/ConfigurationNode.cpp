#include "ConfigurationNode.h"

#include "Value.h"
#include "../../unicode/utf8.h"

using namespace blogator::configuration;

/**
 * Move-constructor
 * @param node Node to move
 */
ConfigurationNode::ConfigurationNode( ConfigurationNode &&node ) noexcept :
    suffix( std::move( node.suffix ) ),
    value( std::move( node.value ) ),
    children( std::move( node.children ) )
{}

/**
 * Constructor
 * @param suffix Node suffix string
 */
ConfigurationNode::ConfigurationNode( std::u32string suffix ) :
    suffix( std::move( suffix ) )
{}

/**
 * Copy-constructor
 * @param node Node to copy
 */
ConfigurationNode::ConfigurationNode( const ConfigurationNode & node ) :
    suffix( node.suffix ),
    value( node.value ? std::make_unique<Value>( *node.value ) : nullptr )
{
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
    suffix = node.suffix;

    if( node.value ) {
        this->value = std::move( node.value );
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
    suffix = std::move( node.suffix );

    if( node.value ) {
        this->value = std::make_unique<Value>( *node.value );
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
    if( this->hasValue() && rhs.hasValue() ) {
       return  this->suffix   == rhs.suffix
           && *this->value    == *rhs.value
           &&  this->children == rhs.children;
    }

    if( !this->hasValue() && !rhs.hasValue() ) {
        return this->suffix   == rhs.suffix
            && this->children == rhs.children;
    }

    return false;
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
 * Checks if node has a value set
 * @return Value state
 */
bool ConfigurationNode::hasValue() const {
    return value.operator bool();
}

/**
 * Gets the number of keys in the subtree
 * @return Number of keys with values in subtree
 */
size_t ConfigurationNode::size() const {
    size_t size = ( hasValue() ? 1 : 0 );

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
    if( node.hasValue() ) {
        os << blogator::unicode::utf8::convert( node.suffix ) << " = " << *node.value;
    } else {
        os << blogator::unicode::utf8::convert( node.suffix );
    }

    return os;
}