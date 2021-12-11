#ifndef BLOGATOR_LOGGER_OUTPUT_STREAM_H
#define BLOGATOR_LOGGER_OUTPUT_STREAM_H

#include <ostream>

#include "LogOutput.h"

namespace blogator::logger::output {
    class Stream : public LogOutput {
      public:
        Stream( std::string name, std::ostream &stream );
        ~Stream() override = default;

        void open() override;
        void close() override;
        void write( LogLevel lvl, const std::string &msg ) override;

      private:
        std::ostream & _out;
    };
}

#endif //BLOGATOR_LOGGER_OUTPUT_STREAM_H