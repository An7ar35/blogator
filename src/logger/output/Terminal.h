#ifndef BLOGATOR_LOGGER_OUTPUT_TERMINAL_H
#define BLOGATOR_LOGGER_OUTPUT_TERMINAL_H

#include <ostream>

#include "LogOutput.h"

namespace blogator::logger::output {
    class Terminal : public LogOutput {
      public:
        explicit Terminal( std::string name );
        ~Terminal() override = default;

        void open() override;
        void close() override;
        void write( LogLevel lvl, const std::string &msg ) override;
    };
}

#endif //BLOGATOR_LOGGER_OUTPUT_TERMINAL_H