#include "Namespace.h"

#include "../../../sys/terminate.h"

#include <ostream>
#include <sstream>
#include <unordered_map>

using namespace blogator::parser::specs::infra;

/**
 * [LOCAL] URI-to-Namespace map loader
 * @return Map
 */
static std::unordered_map<std::u32string, Namespace> loadMap() noexcept {
    try {
        auto map = std::unordered_map<std::u32string, Namespace>();

        for( auto i = static_cast<int>( Namespace::URI_DEFS_BEGIN ); i <= static_cast<int>( Namespace::URI_DEFS_END ); ++i ) {
            map.emplace(
                std::make_pair<std::u32string, Namespace>( to_namespaceURI( static_cast<Namespace>( i ) ),
                                                           static_cast<Namespace>( i ) )
            );
        }

        return map;

    } catch( ... ) {
        TERMINATE( "[parser::specs::infra::Namespace::] Failed to load map data (loadMap)." );
        std::terminate();
    }
}

static const std::unordered_map<std::u32string, Namespace> NAMESPACE_URI_MAP = loadMap();

/**
 * Output stream operator
 * @param os Output stream
 * @param ns Namespace enum
 * @return Output stream
 */
std::ostream & blogator::parser::specs::infra::operator <<( std::ostream &os, specs::infra::Namespace ns ) {
    switch( ns ) {
        case Namespace::UNKNOWN: { os << "UNKNOWN";               } break;
        case Namespace::NONE:    { os << "NONE";                  } break;
        case Namespace::DEPR:    { os << "DEPRECIATED HTML";      } break;
        case Namespace::HTML5:   { os << "HTML5";                 } break;
        case Namespace::HTML:    { os << "HTML";                  } break;
        case Namespace::MATHML:  { os << "MathML";                } break;
        case Namespace::SVG:     { os << "SVG";                   } break;
        case Namespace::XLINK:   { os << "XLINK";                 } break;
        case Namespace::XML:     { os << "XML";                   } break;
        case Namespace::XMLNS:   { os << "XMLNS";                 } break;
        case Namespace::OTHER:   { os << "OTHER";                 } break;
        default:                 { os << "enum string not found"; }
    }

    return os;
}

/**
 * Converts a parser::specs::infra::Namespace enum to a namespace string representation
 * @param ns Namespace enum
 * @return Standard string representation
 */
std::u32string blogator::parser::specs::infra::to_namespaceURI( Namespace ns ) {
    //ref: https://infra.spec.whatwg.org/#namespaces
    switch( ns ) {
        case Namespace::UNKNOWN: { return U"UNKNOWN"; }
        case Namespace::NONE:    { return U""; }
        case Namespace::DEPR:    [[fallthrough]];
        case Namespace::HTML5:   [[fallthrough]];
        case Namespace::HTML:    { return U"http://www.w3.org/1999/xhtml"; }
        case Namespace::MATHML:  { return U"http://www.w3.org/1998/Math/MathML"; }
        case Namespace::SVG:     { return U"http://www.w3.org/2000/svg"; }
        case Namespace::XLINK:   { return U"http://www.w3.org/1999/xlink"; }
        case Namespace::XML:     { return U"http://www.w3.org/XML/1998/namespace"; }
        case Namespace::XMLNS:   { return U"http://www.w3.org/2000/xmlns/"; }
        case Namespace::OTHER:   { return U""; }
        default:                 { return U"enum string not found"; }
    }
}

/**
 * Gets the namespace prefix for a given namespace
 * @param ns Namespace enum
 * @return Namespace prefix string
 */
std::u32string blogator::parser::specs::infra::to_prefix( Namespace ns ) {
    switch( ns ) {
        case Namespace::UNKNOWN: { return U""; }
        case Namespace::NONE:    { return U""; }
        case Namespace::DEPR:    { return U""; }
        case Namespace::HTML5:   { return U""; }
        case Namespace::HTML:    { return U""; }
        case Namespace::MATHML:  { return U"math"; }
        case Namespace::SVG:     { return U"svg"; }
        case Namespace::XLINK:   { return U"xlink"; }
        case Namespace::XML:     { return U"xml"; }
        case Namespace::XMLNS:   { return U"xmlns"; }
        case Namespace::OTHER:   [[fallthrough]];
        default:                 { return U""; }
    }
}

/**
 * Gets the enum matching a given URI
 * @param uri Namespace URI string
 * @return Matching enum (or 'UNKNOWN' when no match is found)
 */
Namespace blogator::parser::specs::infra::to_namespace( const std::u32string &uri ) {
    auto it = NAMESPACE_URI_MAP.find( uri );

    return ( it == NAMESPACE_URI_MAP.end() ? Namespace::UNKNOWN : it->second );
}

/**
 * Converts a parser::specs::infra::Namespace enum to a string representation
 * @param ns Namespace enum
 * @return String representation
 */
std::string blogator::to_string( blogator::parser::specs::infra::Namespace ns ) {
    std::stringstream ss;
    ss << ns;
    return ss.str();
}

/**
 * Converts a parser::specs::infra::Namespace enum to a string representation
 * @param ns Namespace enum
 * @return String representation
 */
std::u32string blogator::to_u32string( blogator::parser::specs::infra::Namespace ns ) {
    auto u8str = blogator::to_string( ns );
    return { u8str.cbegin(), u8str.cend() };
}
