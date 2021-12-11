#ifndef BLOGATOR_LOGGER_FORMATTER_DEFAULTFILE_H
#define BLOGATOR_LOGGER_FORMATTER_DEFAULTFILE_H

#include "LogFormatter.h"

namespace blogator::logger::formatter {
    class DefaultFile : public LogFormatter {
      public:
        DefaultFile();
        ~DefaultFile() override = default;

        std::string format( const LogMsg &obj ) override;

      protected:
        void formatLogCounter( std::ostream &os, const LogMsg &obj ) override;
        void formatLogLevel( std::ostream &os, const LogMsg &obj ) override;
        void formatTimestamp( std::ostream &os, const LogMsg &obj ) override;
        void formatSource( std::ostream &os, const LogMsg &obj ) override;
        void formatMessageStr( std::ostream &os, const LogMsg &obj ) override;
    };
}

#endif //BLOGATOR_LOGGER_FORMATTER_DEFAULTFILE_H