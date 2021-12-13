#ifndef BLOGATOR_LOGGER_GLOBAL_LOGMSG_H
#define BLOGATOR_LOGGER_GLOBAL_LOGMSG_H

#include <string>

#include "LogLevel.h"
#include "TimeStamp.h"

namespace blogator::logger {
    class LogMsg {
      public:
        LogMsg( uint64_t id, LogLevel lvl, std::string src_file, int line_num, std::string message );

        friend std::ostream & operator <<( std::ostream &os, const LogMsg & msg ) {
            os << "{ id: " << msg._id
               << ", source: " << msg._src_file << ":" << msg._src_line
               << ", level: " << msg._level
               << ", timestamp: " << msg._ts
               << ", message: " << msg._message
               << " }";
            return os;
        }

        bool operator ==( const LogMsg &rhs ) const;
        bool operator !=( const LogMsg &rhs ) const;

        [[nodiscard]] bool isEquivalent( const LogMsg &rhs ) const;

        [[nodiscard]] uint64_t id() const;
        [[nodiscard]] LogLevel level() const;
        [[nodiscard]] const std::string & source() const;
        [[nodiscard]] int line() const;
        [[nodiscard]] const TimeStamp & timestamp() const;
        [[nodiscard]] const std::string & message() const;

      private:
        uint64_t    _id;
        LogLevel    _level;
        std::string _src_file;
        int         _src_line;
        TimeStamp   _ts;
        std::string _message;
    };
}

#endif //BLOGATOR_LOGGER_GLOBAL_LOGMSG_H