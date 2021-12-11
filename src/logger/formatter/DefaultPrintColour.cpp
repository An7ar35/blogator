#include "DefaultPrintColour.h"

using namespace blogator::logger::formatter;

const char * DefaultPrintColour::RESET  = "\033[0m";
const char * DefaultPrintColour::BLACK  = "\033[30m";
const char * DefaultPrintColour::RED    = "\033[31m";
const char * DefaultPrintColour::GREEN  = "\033[32m";
const char * DefaultPrintColour::YELLOW = "\033[33m";
const char * DefaultPrintColour::BLUE   = "\033[34m";
const char * DefaultPrintColour::BROWN  = "\033[0;33m";
const char * DefaultPrintColour::PURPLE = "\033[35m";
const char * DefaultPrintColour::CYAN   = "\033[36m";
const char * DefaultPrintColour::GREY   = "\033[0;37m";
const char * DefaultPrintColour::WHITE  = "\033[37m";
const char * DefaultPrintColour::BOLD   = "\033[1m";

/**
 * Constructor
 */
DefaultPrintColour::DefaultPrintColour() :
    DefaultPrintMono( LogFormatterType::DEFAULT_PRINT_COLOUR )
{}

/**
 * Format log message object
 * @param obj Log message object
 * @return Formatted string
 */
std::string DefaultPrintColour::format( const blogator::logger::LogMsg &obj ) {
    std::stringstream ss;

    ss << BOLD << BLUE;
    DefaultPrintMono::formatLogCounter( ss, obj );
    ss << RESET;
    DefaultPrintMono::formatTimestamp( ss, obj );
    this->formatLogLevel( ss, obj );
    ss << CYAN;
    DefaultPrintMono::formatMessageStr( ss, obj );
    ss << RESET;

    return ss.str();
}

/**
 * Format log level description
 * @param os Output stream
 * @param obj Log message object
 */
void DefaultPrintColour::formatLogLevel( std::ostream &os, const LogMsg &obj ) {
    switch(  obj.level()  ) {
        case LogLevel::EMERGENCY: { os << RED    << "|EMERGENCY| " << RESET; } break;
        case LogLevel::ALERT:     { os << RED    << "|--ALERT--| " << RESET; } break;
        case LogLevel::CRITICAL:  { os << RED    << "|-CRITICAL| " << RESET; } break;
        case LogLevel::ERROR:     { os << PURPLE << "|--ERROR--| " << RESET; } break;
        case LogLevel::WARNING:   { os << YELLOW << "|-WARNING-| " << RESET; } break;
        case LogLevel::NOTICE:    { os           << "|--NOTICE-| ";          } break;
        case LogLevel::INFO:      { os           << "|---INFO--| ";          } break;
        case LogLevel::DEBUG:     { os << GREEN  << "|--DEBUG--| " << RESET; } break;
        case LogLevel::TRACE:     { os << GREY   << "|--TRACE--| " << RESET; } break;
        default:                  { os << BROWN  << "|-UNKNOWN-| " << RESET; } break;
    }
}