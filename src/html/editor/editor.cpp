#include "editor.h"

#include <iostream>
#include <sstream>

/**
 * Removes any complete hyperlinks (format = '<a' ... '>' ... '</a>')
 * @param html HTML DTO
 * @return Number of hyperlinks found
 */
size_t blogator::html::editor::removeHyperlinks( blogator::dto::HTML & html ) {
    size_t href_removed = 0;

    for( auto &line : html._lines )
        href_removed += html::editor::removeHyperlinks( line );

    return href_removed;
}

/**
 * Removes any complete hyperlinks (format = '<a' ... '>' ... '</a>')
 * @param line Line string
 * @return Number of hyperlinks found
 */
size_t blogator::html::editor::removeHyperlinks( std::string &line ) {
    std::string start_tag = "<a";
    std::string mid_tag   = ">";
    std::string end_tag   = "</a>";

    size_t                 href_count = 0;
    std::stringstream      new_line;

    std::string::size_type begin = 0;
    std::string::size_type end   = line.find( start_tag );

    while( end != std::string::npos ) {
        std::stringstream      temp_ss;
        std::string::size_type temp_begin = begin;
        temp_ss << line.substr( temp_begin, end - temp_begin ); //before '<a'

        temp_begin = line.find( mid_tag, end );
        if( temp_begin != std::string::npos ) {
            end = line.find( end_tag, temp_begin );
            temp_ss << line.substr( temp_begin + mid_tag.length(), end - ( temp_begin + mid_tag.length() ) ); // between '>' and '</a>'
        }

        if( end != std::string::npos ) { //reached the closing tag
            temp_begin = end + end_tag.length();
            end = line.find( start_tag, end );

            begin = temp_begin;
            new_line << temp_ss.str();

            ++href_count;
        }
    }

    new_line << line.substr( begin );
    if( href_count > 0 )
        line = new_line.str();

    return href_count;
}
