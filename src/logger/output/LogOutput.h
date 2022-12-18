#ifndef BLOGATOR_LOGGER_OUTPUT_LOGOUTPUT_H
#define BLOGATOR_LOGGER_OUTPUT_LOGOUTPUT_H

#include <string>

#include "../global/LogLevel.h"
#include "../../exception/logger_exception.h"

namespace blogator::logger::output {
    /**
     * Output classes
     */
    enum class LogOutputType {
        SYSLOG,
        TERMINAL,
        FILE,
        STREAM,
        CUSTOM,
    };

    /**
     * Abstract class for outputs
     */
    class LogOutput {
      public:
        LogOutput( LogOutputType type, std::string name );
        virtual ~LogOutput() = default;

        virtual bool operator ==( const LogOutput &rhs ) const final;
        virtual bool operator !=( const LogOutput &rhs ) const final;

        /**
         * Opens the output (called when added to the Logger)
         * @throws exception::logger_exception when `open()` call fails on the LogOutput
         */
        virtual void open() = 0;

        /**
         * Closes the output (cleanup)
         * @throws exception::logger_exception when `close()` call fails on the LogOutput
         */
        virtual void close() = 0;

        /**
         * Writes a log message to the output
         * @param lvl Loglevel of the message
         * @param msg Log event string line
         */
        virtual void write( [[maybe_unused]] LogLevel lvl, const std::string &msg ) = 0;

        [[nodiscard]] virtual LogOutputType type() const final;
        [[nodiscard]] virtual const std::string & name() const final;

      private:
        LogOutputType  _type;
        std::string    _name;
    };
}

#endif //BLOGATOR_LOGGER_OUTPUT_LOGOUTPUT_H
