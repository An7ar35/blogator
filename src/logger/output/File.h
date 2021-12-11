#ifndef BLOGATOR_LOGGER_OUTPUT_FILE_H
#define BLOGATOR_LOGGER_OUTPUT_FILE_H

#include <filesystem>
#include <fstream>

#include "LogOutput.h"

namespace blogator::logger::output {
    class File : public LogOutput {
      public:
        explicit File( const std::filesystem::path& file_path );
        ~File() override;

        void open() override;
        void close() override;
        void write( LogLevel lvl, const std::string &msg ) override;

      private:
        std::filesystem::path          _path;
        std::unique_ptr<std::ofstream> _out;
    };
}

#endif //BLOGATOR_LOGGER_OUTPUT_FILE_H