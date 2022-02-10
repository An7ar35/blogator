#include "NamespaceMap.h"

#include "../../../logger/Logger.h"
#include "../../../unicode/unicode.h"

using namespace blogator::parser::dom;

/**
 * Sets a namespace
 * @param ns specs::html5::Namespace enum
 * @return Namespace ID (or INVALID on error)
 */
NamespaceMap::id_t NamespaceMap::setNamespace( blogator::parser::specs::html5::Namespace ns ) {
    std::lock_guard<std::mutex> guard( _mutex );

    if( ns == specs::html5::Namespace::UNKNOWN || ns ==specs::html5::Namespace::OTHER ) {
        LOG_ERROR(
            "[parser::dom::NamespaceMap::setNamespace( ", blogator::to_string( ns ), " )] "
            "Invalid argument. Use DOMString_t version of setter instead."
        );

        return INVALID; //EARLY RETURN
    }

    if( _enum_mapping.contains( ns ) ) {
        return _enum_mapping.at( ns ); //EARLY RETURN

    } else {
        const auto i               = _namespaces.size();
        const auto [ it, success ] = _enum_mapping.emplace( std::pair<specs::html5::Namespace, id_t>( ns, i ) );

        if( success ) {
            _namespaces.emplace_back( NS { specs::html5::to_namespaceURI( ns ), ns } );
            return static_cast<id_t>( i ); //BRANCHED RETURN (1/2)

        } else {
            LOG_CRITICAL(
                "[parser::dom::NamespaceMap::setNamespace( ", blogator::to_string( ns ), " )] "
                "Failed to add enum mapping."
            );

            return INVALID;  //BRANCHED RETURN (2/2)
        }
    }
}

/**
 * Sets a namespace
 * @param ns Namespace URI string
 * @return Namespace ID (or INVALID on error)
 */
NamespaceMap::id_t NamespaceMap::setNamespace( const DOMString_t &ns ) {
    std::lock_guard<std::mutex> guard( _mutex );

    if( _str_mapping.contains( ns ) ) {
        return _str_mapping.at( ns ); //EARLY RETURN

    } else {
        const auto i               = _namespaces.size();
        const auto [ it, success ] = _str_mapping.emplace( std::pair<DOMString_t , id_t>( ns, i ) );

        if( success ) {
            _namespaces.emplace_back( NS { ns, specs::html5::Namespace::OTHER } );
            return static_cast<id_t>( i ); //BRANCHED RETURN (1/2)

        } else {
            LOG_CRITICAL(
                "[parser::dom::NamespaceMap::setNamespace( ", blogator::unicode::utf8::convert( ns ), " )] "
                "Failed to add string mapping."
            );

            return INVALID;  //BRANCHED RETURN (2/2)
        }
    }
}

/**
 * Gets an namespace's blogator enum
 * @param id Namespace ID
 * @return specs::html5::Namespace enum
 * @throws std::out_of_range when mapping inconsistency is detected
 */
blogator::parser::specs::html5::Namespace NamespaceMap::getNamespaceEnum( NamespaceMap::id_t id ) const {
    std::lock_guard<std::mutex> guard( _mutex );

    if( id < 0 || id >= _namespaces.size() ) {
        LOG_ERROR(
            "[parser::dom::NamespaceMap::getNamespaceEnum( ", id, " )] "
            "Namespace mapping may be corrupted."
        );

        throw std::out_of_range( "NamespaceMap inconsistency detected - see log" );
    }

    return _namespaces.at( id ).ns_enum;
}

/**
 * Gets a namespace's URI
 * @param id Namespace ID
 * @return Namespace URI
 * @throws std::out_of_range when mapping inconsistency is detected
 */
const DOMString_t & NamespaceMap::getNamespaceURI( id_t id ) const {
    std::lock_guard<std::mutex> guard( _mutex );

    if( id < 0 || id >= _namespaces.size() ) {
        LOG_ERROR(
            "[parser::dom::NamespaceMap::getNamespaceURI( ", id, " )] "
            "Namespace mapping may be corrupted."
        );

        throw std::out_of_range( "NamespaceMap inconsistency detected - see log" );
    }

    return _namespaces.at( id ).uri;
}

/**
 * Gets the number of namespaces declared in the map
 * @return Namespace count
 */
size_t NamespaceMap::size() const {
    std::lock_guard<std::mutex> guard( _mutex );
    return _namespaces.size();
}

/**
 * Gets the empty state of the map
 * @return Empty state
 */
bool NamespaceMap::empty() const {
    std::lock_guard<std::mutex> guard( _mutex );
    return _namespaces.empty();
}
