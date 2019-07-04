#include "reader.h"
#include <iostream>
#include <regex>

/**
 * Gets any content between 2 strings on a line
 * @param from Starting string
 * @param to   Finishing string
 * @param line Line of html text
 * @return Content found
 */
std::string blogator::html::reader::getContentBetween( const std::string &from,
                                                       const std::string &to,
                                                       const std::string &line )
{
    std::string content;
    auto i_begin = line.find( from );

    if( i_begin != std::string::npos ) {
        auto range_begin = i_begin + from.length();
        auto i_end       = line.find( to, range_begin );
        if( i_end != std::string::npos ) {
            auto range_end = i_end - ( i_begin + from.length() );
            content = line.substr( range_begin, range_end );
        }
    }

    return content;
}

/**
 * Gets the first set of tags found in the line
 * @param line Line of html text
 * @return Set of tags found
 */
std::vector<std::string> blogator::html::reader::getTags( const std::string &line ) {
    static const std::string span_tag_open = "<span class=\"tag\">";
    static const std::string span_tag_close = "</span>";

    auto tags = std::vector<std::string>();

    auto i_begin = line.find( span_tag_open );

    while( i_begin != std::string::npos ) {
        auto i_end = line.find( span_tag_close, i_begin + span_tag_open.length() );
        auto next = i_begin + span_tag_open.length();

        while( next < i_end ) {
            i_begin = next;
            next = line.find( ',', i_begin + 1 );

            if( next == std::string::npos ) {
                tags.emplace_back( line.substr( i_begin, i_end - i_begin ) );
            } else {
                tags.emplace_back( line.substr( i_begin, next - i_begin ) );
                next = line.find_first_not_of( " ,\t\n\v\f\r", next );
            }
        }

        i_begin = line.find( span_tag_open, i_end );
    }

    return tags;
}

/**
 * Finds the line index of a tag's first occurrence
 * @param tag Tag to find
 * @param html HTML DTO
 * @return Line index of the tag
 * @throws std::invalid_argument when the tag is empty (i.e.: "")
 * @throws std::out_of_range when the tag cannot be found on any lines
 */
size_t blogator::html::reader::findLineOfTag( const std::string &tag, const blogator::dto::HTML &html ) {
    if( tag.empty() )
        throw std::invalid_argument( "No tag was given." );

    for( auto i = 0; i < html._lines.size(); ++i ) {
        auto pos = html._lines.at( i ).find( tag );
        if( pos != std::string::npos )
            return i;
    }

    throw std::out_of_range( "Tag \"" + tag + "\" could not be found in dto::HTML." );
}

/**
 * Finds insert positions in an HTML structure
 * @param html HTML DTO to search in
 * @param divs Map of div class names to find
 * @return Reference to the modified DivWritePositions_t
 */
blogator::dto::Template::DivWritePositions_t &
    blogator::html::reader::findInsertPositions( const blogator::dto::HTML &html,
                                                 blogator::dto::Template::DivWritePositions_t &divs )
{
    size_t line_number = 0;

    for( const auto &line : html._lines ) {
        for( auto &div : divs ) {
            auto s  = R"(^(.*<\s*[a-zA-Z\d]+\s*class\s*=\s*"[a-zA-Z-_\s]*)" + div.first + R"((?:\s[a-zA-Z-_\s]*"|")>\s*)(?:</[a-zA-Z\d]+>))";
            auto rx = std::regex( s );
            auto it = std::sregex_iterator( line.begin(), line.end(), rx );

            while( it != std::sregex_iterator() ) {
                div.second.emplace_back( line_number, it->str( 1 ).length() );
                ++it;
            }
        }

        ++line_number;
    }

    return divs;
}

/**
 * Gets the indent  (blank spaces) on a line
 * @param line String line
 * @return Indent string
 */
std::string blogator::html::reader::getIndent( const std::string & line ) {
    if( line.empty() )
        return "";

    const auto char_it = find_if( line.begin(),
                                  line.end(),
                                  []( char c ) { return !isspace( c ); } );

    return line.substr( 0, char_it - line.begin() );
}