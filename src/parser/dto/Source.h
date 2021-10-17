#ifndef BLOGATOR_PARSER_DTO_SOURCE_H
#define BLOGATOR_PARSER_DTO_SOURCE_H

#include <istream>
#include <filesystem>
#include "../encoding/Format.h"
#include "../encoding/Confidence.h"

namespace blogator::parser {
    class Source {
      public:
        Source( std::istream & in, std::filesystem::path path );
        Source( std::istream & in, std::filesystem::path path, encoding::Format_e format );

        std::istream & stream();
        [[nodiscard]] const std::filesystem::path & path() const;
        [[nodiscard]] encoding::Format_e format() const;
        [[nodiscard]] encoding::Confidence_e confidence() const;

        void setFormat( encoding::Format_e format );
        void setConfidence( encoding::Confidence_e confidence );

      private:
        std::istream              & _stream;
        const std::filesystem::path _path;
        encoding::Format_e          _format;
        encoding::Confidence_e      _confidence;
    };
}

#endif //BLOGATOR_PARSER_DTO_SOURCE_H