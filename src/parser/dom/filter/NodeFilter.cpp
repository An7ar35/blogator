#include "NodeFilter.h"

#include <utility>

#include "../node/Node.h"

using namespace blogator::parser::dom;

/**
 * Constructor
 * @param what_to_show What to show bit mask
 */
NodeFilter::NodeFilter( unsigned long what_to_show ) :
    _what_to_show( what_to_show )
{}

/**
 * Constructor
 * @param what_to_show What to show bit mask
 * @param acceptNode_fn Callback function for node content filtering
 */
blogator::parser::dom::NodeFilter::NodeFilter( unsigned long what_to_show, std::function<Result( const node::Node & )> acceptNode_fn ) :
    _what_to_show( what_to_show ),
    _callback( std::move( acceptNode_fn ) )
{}

/**
 * Gets the filter's what to show mask
 * @return  What-to-show mask
 */
unsigned long NodeFilter::whatToShow() const {
    return _what_to_show;
}

/**
 * Check a node against filter
 * @param node Node to check
 * @return Filtering result
 */
NodeFilter::Result NodeFilter::acceptNode( const node::Node &node ) const {
    if( _callback ) {
        if( this->validate( node ) ) {
            return _callback( node );
        } else {
            return Result::FILTER_SKIP;
        }
    } else {
        return ( this->validate( node ) ? Result::FILTER_ACCEPT : Result::FILTER_SKIP );
    }
}

/**
 * Check a node against filter mask
 * @param node Node to check
 * @return Mask-based filtering result
 */
bool NodeFilter::validate( const node::Node &node ) const {
    switch( node.nodeType() ) {
        case NodeType::ELEMENT_NODE:           { return ( _what_to_show & SHOW_ELEMENT );           };
        case NodeType::ATTRIBUTE_NODE:         { return ( _what_to_show & SHOW_ATTRIBUTE );         };
        case NodeType::TEXT_NODE:              { return ( _what_to_show & SHOW_TEXT );              };
        case NodeType::CDATA_SECTION_NODE:     { return ( _what_to_show & SHOW_CDATA_SECTION );     };
        case NodeType::COMMENT_NODE:           { return ( _what_to_show & SHOW_COMMENT );           };
        case NodeType::DOCUMENT_NODE:          { return ( _what_to_show & SHOW_DOCUMENT );          };
        case NodeType::DOCUMENT_TYPE_NODE:     { return ( _what_to_show & SHOW_DOCUMENT_TYPE );     };
        case NodeType::DOCUMENT_FRAGMENT_NODE: { return ( _what_to_show & SHOW_DOCUMENT_FRAGMENT ); };
        case NodeType::UNDEFINED:              [[fallthrough]];
        default:                               { return false; };
    }
}