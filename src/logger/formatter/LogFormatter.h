#ifndef BLOGATOR_LOGGER_FORMATTER_LOGFORMATTER_H
#define BLOGATOR_LOGGER_FORMATTER_LOGFORMATTER_H

#include <string>

#include "../global/LogMsg.h"

namespace blogator::logger::formatter {
    enum class LogFormatterType {
        DEFAULT_FILE,
        DEFAULT_PRINT_MONO,
        DEFAULT_PRINT_COLOUR,
        CUSTOM,
    };

    class LogFormatter {
      public:
        explicit LogFormatter( LogFormatterType type );
        virtual ~LogFormatter() = default;

        /**
         * Formats a LogMsg object into a string
         * @param obj LogMsg object
         * @return Formatted string
         */
        virtual std::string format( const LogMsg &obj ) = 0;

        [[nodiscard]] virtual LogFormatterType type() const final;

      protected:
        virtual void formatLogCounter( std::ostream &os, const LogMsg &obj ) = 0;
        virtual void formatLogLevel( std::ostream &os, const LogMsg &obj ) = 0;
        virtual void formatTimestamp( std::ostream &os, const LogMsg &obj ) = 0;
        virtual void formatSource( std::ostream &os, const LogMsg &obj ) = 0;
        virtual void formatMessageStr( std::ostream &os, const LogMsg &obj ) = 0;

      private:
        LogFormatterType _type;
    };
}

#endif //BLOGATOR_LOGGER_FORMATTER_LOGFORMATTER_H