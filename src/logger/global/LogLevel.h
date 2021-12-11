#ifndef BLOGATOR_LOGGER_GLOBAL_LOGLEVEL_H
#define BLOGATOR_LOGGER_GLOBAL_LOGLEVEL_H

#include <string>

namespace blogator::logger {
    enum class LogLevel : int {
        EMERGENCY = 0,
        ALERT     = 1,
        CRITICAL  = 2,
        ERROR     = 3,
        WARNING   = 4,
        NOTICE    = 5,
        INFO      = 6,
        DEBUG     = 7,
        TRACE     = 8,
    };

    std::string to_string( LogLevel lvl );
    std::ostream & operator <<( std::ostream &os, const LogLevel &lvl );
}

#endif //BLOGATOR_LOGGER_GLOBAL_LOGLEVEL_H