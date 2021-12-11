#ifndef BLOGATOR_LOGGER_OUTPUT_SYSLOG_H
#define BLOGATOR_LOGGER_OUTPUT_SYSLOG_H

#include "LogOutput.h"

namespace blogator::logger::output {
    class SysLog : public LogOutput {
      public:
        explicit SysLog( std::string name );
        ~SysLog() override = default;

        void open() override;
        void close() override;
        void write( LogLevel lvl, const std::string &msg ) override;
    };
}

#endif //BLOGATOR_LOGGER_OUTPUT_SYSLOG_H