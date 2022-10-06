#ifndef BLOGATOR_STRING_STRING_H
#define BLOGATOR_STRING_STRING_H

#include <vector>
#include <string>
#include <type_traits>
#include <functional>

namespace blogator::string {
    template<typename CharType> std::vector<std::basic_string<CharType>> split( std::basic_string<CharType> str, CharType separator, bool discard_empty = false );
    template<typename CharType> typename std::basic_string<CharType>::iterator & skipWhitespace( const std::basic_string<CharType> &str, typename std::basic_string<CharType>::iterator &it );
    template<typename CharType> typename std::basic_string<CharType>::const_iterator & skipWhitespace( const std::basic_string<CharType> &str, typename std::basic_string<CharType>::const_iterator &it );
    template<typename CharType> typename std::basic_string<CharType> remove( std::basic_string<CharType> str, std::function<bool( CharType )> predicate_fn );
}

// =================================== TEMPLATE IMPLEMENTATIONS ================================= //

/**
 * Splits a string into substrings
 * @tparam CharType String character type
 * @param str       String to split
 * @param separator Split separator character
 * @param discard_empty Discard empty sections (default=false)
 * @return List of sub-strings
 */
template<typename CharType> std::vector<std::basic_string<CharType>> blogator::string::split(
    std::basic_string<CharType> str,
    CharType                    separator,
    bool                        discard_empty )
{
    auto v = std::vector<std::basic_string<CharType>>();

    if( str.empty() )
        return v;

    typename std::basic_string<CharType>::size_type prev = 0;
    typename std::basic_string<CharType>::size_type next = str.find( separator );

    while( next != std::basic_string<CharType>::npos ) {
        const auto length = next - prev;

        if( length != 0 || !discard_empty ) {
            v.emplace_back( str.substr( prev, length ) );
        }

        prev = next + 1;
        next = str.find( separator, prev );
    }

    const auto length = str.size() - prev;

    if( length != 0 || !discard_empty ) {
        v.emplace_back( str.substr( prev ) );
    }

    return v;
}

/**
 * Skips over any whitespaces
 * @tparam CharType Character type
 * @param str       String
 * @param it        String iterator to use
 * @return String iterator placed to either the end or the first non-whitespace character found
 */
template<typename CharType> typename std::basic_string<CharType>::iterator & blogator::string::skipWhitespace(
    const std::basic_string<CharType> &str,
    typename std::basic_string<CharType>::iterator &it )
{
    if constexpr( std::is_same_v<CharType, char32_t> ) {
        while( it != str.cend() && iswspace( *it ) )
            ++it;
    } else {
        while( it != str.cend() && isspace( *it ) )
            ++it;
    }

    return it;
}

/**
 * Skips over any whitespaces
 * @tparam CharType Character type
 * @param str       String
 * @param it        Const string iterator to use
 * @return Const string iterator placed to either the end or the first non-whitespace character found
 */
template<typename CharType> typename std::basic_string<CharType>::const_iterator & blogator::string::skipWhitespace(
    const std::basic_string<CharType> &str,
    typename std::basic_string<CharType>::const_iterator &it )
{
    if constexpr( std::is_same_v<CharType, char32_t> ) {
        while( it != str.cend() && iswspace( *it ) )
            ++it;
    } else {
        while( it != str.cend() && isspace( *it ) )
            ++it;
    }

    return it;
}

/**
 * Removes any characters matching the predicate
 * @tparam CharType Character type
 * @param str String
 * @param predicate_fn Predicate function that return true on a character to remove
 * @return String with characters removed
 * @throws std::bad_alloc if the function needs to allocate storage and fails
 */
template<typename CharType> std::basic_string<CharType> blogator::string::remove( //TODO unit test
    std::basic_string<CharType> str,
    std::function<bool( CharType )> predicate_fn )
{
    auto it1 = str.begin();
    auto it2 = it1;

    while( it2 != str.end() ) {
        if( !predicate_fn ) {
            *it1 = *it2;
            ++it1;
        }

        ++it2;
    }

    try {
        return str.substr( 0, std::distance( str.begin(), it1 ) );
    } catch( const std::out_of_range & e ) {
        return {};
    }
}

#endif //BLOGATOR_STRING_STRING_H
