#ifndef BLOGATOR_STRING_STRING_H
#define BLOGATOR_STRING_STRING_H

#include <vector>
#include <string>
#include <type_traits>

namespace blogator::string {
    template<typename CharType> std::vector<std::basic_string<CharType>> split( std::basic_string<CharType> str, CharType separator, bool discard_empty = false );
    template<typename CharType> typename std::basic_string<CharType>::iterator & skipWhitespace( const std::basic_string<CharType> &str, typename std::basic_string<CharType>::iterator &it );
    template<typename CharType> typename std::basic_string<CharType>::const_iterator & skipWhitespace( const std::basic_string<CharType> &str, typename std::basic_string<CharType>::const_iterator &it );
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

#endif //BLOGATOR_STRING_STRING_H
