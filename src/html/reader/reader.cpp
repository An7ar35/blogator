#include "reader.h"
#include <iostream>
#include <fstream>
#include <regex>

/**
 * Gets any content between 2 strings in a string
 * @param from Starting string
 * @param to   Finishing string
 * @param str Line of html text
 * @return Content found
 */
std::string blogator::html::reader::getContentBetween( const std::string &from,
                                                       const std::string &to,
                                                       const std::string &str )
{
    std::string content;
    auto i_begin = str.find( from );

    if( i_begin != std::string::npos ) {
        auto range_begin = i_begin + from.length();
        auto i_end       = str.find( to, range_begin );
        if( i_end != std::string::npos ) {
            auto range_end = i_end - ( i_begin + from.length() );
            content = str.substr( range_begin, range_end );
        }
    }

    return content;
}

/**
 * Gets any content between 2 strings inside an HTML DTO
 * @param from Starting string
 * @param to   Finishing string
 * @param html HTML DTO consisting of multiple lines
 * @return Content lines found as a HTML DTO
 */
blogator::dto::HTML blogator::html::reader::getContentBetween( const std::string &from,
                                                               const std::string &to,
                                                               const blogator::dto::HTML &html )
{
    auto content = dto::HTML();

    if( html._lines.empty() )
        return content;

    auto line_it = html._lines.cbegin();
    auto i_begin = std::string::npos;

    while( line_it != html._lines.cend() && ( i_begin = line_it->find( from ) ) == std::string::npos ) {
        ++line_it;
    }

    if( line_it == html._lines.cend() )
        return content;

    auto i_end = line_it->find( to );

    if( i_end == std::string::npos || i_end < i_begin) //grab the rest of the line
        content._lines.emplace_back( line_it->substr( i_begin + from.length() ) );
    else
        content._lines.emplace_back( line_it->substr( i_begin + from.length(), i_end - ( i_begin + from.length() ) ) );

    while( ( ++line_it ) != html._lines.cend() && ( i_end = line_it->find( to ) ) == std::string::npos ) {
        content._lines.emplace_back( *line_it );
    }

    if( line_it != html._lines.cend() && i_end != std::string::npos )
        content._lines.emplace_back( line_it->substr( 0, i_end ) );

    return content;
}

/**
 * Gets all the content between any repeating range in a string
 * @param from Starting string
 * @param to   Finishing string
 * @param str Html text string
 * @return Set of content found
 */
std::vector<std::string> blogator::html::reader::getContentsBetween( const std::string &from,
                                                                     const std::string &to,
                                                                     const std::string &str )
{
    std::vector<std::string> content;
    auto i_begin = str.find( from );

    while( i_begin != std::string::npos ) {
        auto range_begin = i_begin + from.length();
        auto i_end = str.find( to, range_begin );

        if( i_end != std::string::npos ) {
            auto range_end = i_end - ( i_begin + from.length() );
            content.emplace_back( str.substr( range_begin, range_end ) );
        }

        i_begin = str.find( from, i_end );
    }

    return content;
}

/**
 * Gets the positions of the start and finish of a summary or summaries in a line
 * @param line_number Current line number
 * @param line        Line
 * @param positions   Container for all positions found
 */
void blogator::html::reader::getSummaryPositions( const size_t & line_number,
                                                  const std::string & line,
                                                  std::deque<dto::InsertPosition> &positions )
{
    const static auto tag = std::make_pair<std::string, std::string>( "<span class=\"summary\">", "</span>" );

    bool end_of_line = { false };

    while( !end_of_line ) {
        if( positions.size() % 2 == 0 ) { //need to find opening tag
            auto summary_begin = ( positions.empty() || positions.back().line != line_number )
                                 ? line.find( tag.first )
                                 : line.find( tag.first, positions.back().col );

            if( summary_begin != std::string::npos ) {
                positions.emplace_back( dto::InsertPosition( line_number, ( summary_begin + tag.first.length() ) ) );
            } else {
                end_of_line = true;
            }

        } else { //need to find a closing tag
            auto summary_end = ( positions.back().line != line_number )
                               ? line.find( tag.second )
                               : line.find( tag.second, positions.back().col );

            if( summary_end != std::string::npos ) {
                positions.emplace_back( dto::InsertPosition( line_number, summary_end ) );
            } else {
                end_of_line = true;
            }
        }
    }
}

/**
 * Gets the first set of tags found in a string
 * @param str Html text
 * @return Set of tags found
 */
std::vector<std::string> blogator::html::reader::getTags( const std::string &str ) {
    static const std::string span_tag_open = "<span class=\"tag\">";
    static const std::string span_tag_close = "</span>";

    auto tags = std::vector<std::string>();

    auto i_begin = str.find( span_tag_open );

    while( i_begin != std::string::npos ) {
        auto i_end = str.find( span_tag_close, i_begin + span_tag_open.length() );
        auto next = i_begin + span_tag_open.length();

        while( next < i_end ) {
            i_begin = next;
            next = str.find( ',', i_begin + 1 );

            if( next == std::string::npos ) {
                tags.emplace_back( str.substr( i_begin, i_end - i_begin ) );
            } else {
                tags.emplace_back( str.substr( i_begin, next - i_begin ) );
                next = str.find_first_not_of( " ,\t\n\v\f\r", next );
            }
        }

        i_begin = str.find( span_tag_open, i_end );
    }

    return tags;
}

/**
 * Gets all insertion points for a list of blocks (css classes) in an HTML document
 * @param html   HTML DTO
 * @param blocks List of css classes used to flag insertion points in the HTML
 * @return Consecutive insert points with their respective css classes
 */
blogator::dto::ConsecutiveWritePositions_t
    blogator::html::reader::getConsecutiveWritePositions( const blogator::dto::HTML &html,
                                                          blogator::dto::Template::BlockInsertClasses_t &blocks )
{
    auto   write_pos   = dto::ConsecutiveWritePositions_t();
    size_t line_number = 0;

    for( const auto &line : html._lines ) {
        for( auto &css_class : blocks ) {
            auto s  = R"(^(.*<\s*[a-zA-Z\d]+\s*class\s*=\s*"[a-zA-Z-_\s]*)" + css_class.first + R"((?:\s[a-zA-Z-_\s]*"|")>\s*)(?:</[a-zA-Z\d]+>))";
            auto rx = std::regex( s );
            auto it = std::sregex_iterator( line.begin(), line.end(), rx );

            if( it != std::sregex_iterator() )
                css_class.second = true; //class is used

            while( it != std::sregex_iterator() ) {
                write_pos.insert(
                    std::make_pair( dto::InsertPosition( line_number, it->str( 1 ).length() ),
                                    css_class.first
                    )
                );
                ++it;
            }
        }

        ++line_number;
    }

    return write_pos;
}

/**
 * Gets the indent  (blank spaces) on a line
 * @param line String line
 * @return Indent string
 */
std::string blogator::html::reader::getIndent( const std::string &line ) {
    if( line.empty() )
        return "";

    const auto char_it = find_if( line.begin(),
                                  line.end(),
                                  []( char c ) { return !isspace( c ); } );

    return line.substr( 0, char_it - line.begin() );
}