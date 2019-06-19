#include "html.h"
#include <iostream>

/**
 * Gets the text between heading (h1) tags
 * @param line Line of html text
 * @return Heading found
 */
std::string blogator::html::readHeading( const std::string &line ) {
    static const std::string h1_tag_open = "<h1>";

    std::string heading;
    auto i_begin = line.find( h1_tag_open );

    if( i_begin!= std::string::npos ) {
        auto i_end = line.find( "</h1>", i_begin + h1_tag_open.length() );
        heading = line.substr( i_begin + h1_tag_open.length(), i_end - h1_tag_open.length() );
    }

    return heading;
}

/**
 * Gets the first date found in the line
 * @param line Line of html text
 * @return Date found
 */
std::string blogator::html::readDate( const std::string &line ) {
    static const std::string time_tag_open = "<time datetime=\"";

    std::string date;
    auto        i_begin = line.find( time_tag_open );

    if( i_begin != std::string::npos ) {
        auto i_end = line.find( '\"', i_begin + time_tag_open.length() );
        date = line.substr( i_begin + time_tag_open.length(), i_end - time_tag_open.length() );
    }

    return date;
}

/**
 * Gets the first set of tags found in the line
 * @param line Line of html text
 * @return Set of tags found
 */
std::vector<std::string> blogator::html::readTags( const std::string &line ) {
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
size_t blogator::html::findLineOfTag( const std::string &tag, const blogator::dto::HTML &html ) {
    if( tag.empty() )
        throw std::invalid_argument( "No tag was given." );

    for( auto i = 0; i < html._lines.size(); ++i ) {
        auto pos = html._lines.at( i ).find( tag );
        if( pos != std::string::npos )
            return i;
    }

    throw std::out_of_range( "Tag \"" + tag + "\" could not be found in dto::HTML." );
}
