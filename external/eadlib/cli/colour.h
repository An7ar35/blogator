/**
    \defgroup       CLI Command Line Colours
    \ingroup        CLI
    \defgroup       Colours
    @brief          Method to format text using ANSI escape codes destined for the terminal
    @author         E. A. Davison
    @copyright      E. A. Davison 2019
    @license        GNUv2 Public License
**/
#ifndef EADLIB_CLI_COLOURS_H
#define EADLIB_CLI_COLOURS_H

#include <string>
#include <iostream>
#include <sstream>

namespace eadlib::cli {
    enum class FGColour   { BLACK = 30, RED = 31, GREEN = 32, YELLOW = 33, BLUE = 34, MAGENTA = 35, CYAN = 36, WHITE = 37 };
    enum class BGColour   { BLACK = 40, RED = 41, GREEN = 42, YELLOW = 43, BLUE = 44, MAGENTA = 45, CYAN = 46, WHITE = 47 };
    enum class Decoration { BOLD = 1, FAINT = 2, ITALIC = 3, UNDERLINE = 4 };

    template<FGColour colour> void format_( std::stringstream &ss ) {}
    template<FGColour colour, Decoration head, Decoration ...tail> void format_( std::stringstream &ss ) {
        ss << ';' << static_cast<int>( head );
        format_<colour, tail...>( ss );
    }

    /**
     * Formats text using ANSI terminal escape codes
     * @tparam colour Foreground (text) colour
     * @tparam args   Decorations
     * @param str     String to format
     * @return Formatted string
     */
    template<FGColour colour, Decoration ...args> std::string format( const std::string & str ) {
        std::stringstream ss;
        ss << "\33[" << static_cast<int>( colour );
        format_<colour, args...>( ss );
        ss << "m" << str << "\033[0m";
        return ss.str();
    }

    template<BGColour colour> void format_( std::stringstream &ss ) {}
    template<BGColour colour, Decoration head, Decoration ...tail> void format_( std::stringstream &ss ) {
        ss << ';' << static_cast<int>( head );
        format_<colour, tail...>( ss );
    }

    /**
     * Formats text using ANSI terminal escape codes
     * @tparam colour Background colour
     * @tparam args   Decorations
     * @param str     String to format
     * @return Formatted string
     */
    template<BGColour colour, Decoration ...args> std::string format( const std::string &str ) {
        std::stringstream ss;
        ss << "\33[" << static_cast<int>( colour );
        format_<colour, args...>( ss );
        ss << "m" << str << "\033[0m";
        return ss.str();
    }
}

#endif //EADLIB_CLI_COLOURS_H
