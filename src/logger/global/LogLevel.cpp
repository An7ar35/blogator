#include "LogLevel.h"

#include <ostream>
#include <sstream>

/**
 * Gets a string representation of a LogLevel
 * @param lvl LogLevel
 * @return String representation
 */
std::string blogator::logger::to_string( blogator::logger::LogLevel lvl ) {
    std::stringstream ss;
    ss << lvl;
    return ss.str();
}

/**
 * Outputs a representation to a stream
 * @param os Output stream
 * @param lvl Level to print out
 * @return Output stream
 */
std::ostream & blogator::logger::operator <<( std::ostream &os, const blogator::logger::LogLevel &lvl ) {
    switch(  lvl  ) {
        case LogLevel::EMERGENCY: { os << "EMERGENCY"; } break;
        case LogLevel::ALERT:     { os << "ALERT";     } break;
        case LogLevel::CRITICAL:  { os << "CRITICAL";  } break;
        case LogLevel::ERROR:     { os << "ERROR";     } break;
        case LogLevel::WARNING:   { os << "WARNING";   } break;
        case LogLevel::NOTICE:    { os << "NOTICE";    } break;
        case LogLevel::INFO:      { os << "INFO";      } break;
        case LogLevel::DEBUG:     { os << "DEBUG";     } break;
        case LogLevel::TRACE:     { os << "TRACE";     } break;
        default:                  { os << "UNKNOWN";   } break;
    }

    return os;
}
