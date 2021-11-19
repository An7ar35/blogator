#ifndef BLOGATOR_PARSER_DTO_SOURCE_H
#define BLOGATOR_PARSER_DTO_SOURCE_H

#include <istream>
#include <filesystem>

#include "TextPos.h"
#include "../encoding/Format.h"
#include "../encoding/Confidence.h"

namespace blogator::parser {
    /**
     * Container class for an input stream and properties related to it
     */
    class Source {
      public:
        Source( std::istream & in, std::filesystem::path path );
        Source( std::istream & in, std::filesystem::path path, encoding::Format format );

        std::istream & stream();
        TextPos & position();
        [[nodiscard]] const std::filesystem::path & path() const;
        [[nodiscard]] encoding::Format format() const;
        [[nodiscard]] encoding::Confidence confidence() const;

        void setFormat( encoding::Format format );
        void setConfidence( encoding::Confidence confidence );

      private:
        std::istream              & _stream;
        const std::filesystem::path _path;
        encoding::Format            _format;
        encoding::Confidence        _confidence;
        TextPos                     _position;
    };
}

#endif //BLOGATOR_PARSER_DTO_SOURCE_H