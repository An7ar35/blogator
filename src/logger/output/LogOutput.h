#ifndef BLOGATOR_LOGGER_OUTPUT_LOGOUTPUT_H
#define BLOGATOR_LOGGER_OUTPUT_LOGOUTPUT_H

#include <string>

#include "../global/LogLevel.h"

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

        virtual void open() = 0;
        virtual void close() = 0;
        virtual void write( LogLevel lvl, const std::string &msg ) = 0;

        [[nodiscard]] virtual LogOutputType type() const final;
        [[nodiscard]] virtual const std::string & name() const final;

      private:
        LogOutputType  _type;
        std::string    _name;
    };
}

#endif //BLOGATOR_LOGGER_OUTPUT_LOGOUTPUT_H
