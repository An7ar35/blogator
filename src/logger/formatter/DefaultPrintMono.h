#ifndef BLOGATOR_LOGGER_FORMATTER_DEFAULTPRINTMONO_H
#define BLOGATOR_LOGGER_FORMATTER_DEFAULTPRINTMONO_H

#include "LogFormatter.h"

namespace blogator::logger::formatter {
    class DefaultPrintMono : public LogFormatter {
      public:
        DefaultPrintMono();

        std::string format( const LogMsg &obj ) override;

      protected:
        explicit DefaultPrintMono( LogFormatterType type );

        void formatLogCounter( std::ostream &os, const LogMsg &obj ) override;
        void formatLogLevel( std::ostream &os, const LogMsg &obj ) override;
        void formatTimestamp( std::ostream &os, const LogMsg &obj ) override;
        void formatSource( std::ostream &os, const LogMsg &obj ) override;
        void formatMessageStr( std::ostream &os, const LogMsg &obj ) override;
    };
}

#endif //BLOGATOR_LOGGER_FORMATTER_DEFAULTPRINTMONO_H