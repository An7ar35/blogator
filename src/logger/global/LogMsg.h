#ifndef BLOGATOR_LOGGER_GLOBAL_LOGMSG_H
#define BLOGATOR_LOGGER_GLOBAL_LOGMSG_H

#include <string>

#include "LogLevel.h"
#include "TimeStamp.h"

namespace blogator::logger {
    class LogMsg {
      public:
        LogMsg( uint64_t id, LogLevel lvl, std::string src_file, int line_num, std::string message );

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