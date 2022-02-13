#include "Namespace.h"

#include <ostream>
#include <sstream>

using namespace blogator::parser::specs::infra;

/**
 * Output stream operator
 * @param os Output stream
 * @param ns Namespace enum
 * @return Output stream
 */
std::ostream & blogator::parser::specs::infra::operator <<( std::ostream &os, specs::infra::Namespace ns ) {
    switch( ns ) {
        case Namespace::UNKNOWN: { os << "UNKNOWN"; } break;
        case Namespace::HTML5:   { os << "HTML5";   } break;
        case Namespace::MATHML:  { os << "MathML";  } break;
        case Namespace::SVG:     { os << "SVG";     } break;
        case Namespace::XLINK:   { os << "XLINK";   } break;
        case Namespace::XML:     { os << "XML";     } break;
        case Namespace::XMLNS:   { os << "XMLNS";   } break;
        case Namespace::OTHER:   { os << "OTHER";   } break;
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
        case Namespace::HTML5:   { return U"http://www.w3.org/1999/xhtml"; }
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
        case Namespace::HTML5:   { return U""; }
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
