#ifndef BLOGATOR_ENCODING_DTO_SOURCE_H
#define BLOGATOR_ENCODING_DTO_SOURCE_H

#include <istream>
#include <filesystem>

#include "../../dto/TextPos.h"
#include "../specs/Format.h"
#include "../specs/Confidence.h"

namespace blogator::encoding {
    /**
     * Container class for an input stream and properties related to it
     */
    class Source {
      public:
        Source( std::istream & in, std::filesystem::path path );
        Source( std::istream & in, std::filesystem::path path, encoding::specs::Format format );

        std::istream & stream();
        TextPos & position();
        [[nodiscard]] const std::filesystem::path & path() const;
        [[nodiscard]] encoding::specs::Format format() const;
        [[nodiscard]] encoding::specs::Confidence confidence() const;

        void setFormat( encoding::specs::Format format );
        void setConfidence( encoding::specs::Confidence confidence );

      private:
        std::istream              & _stream;
        const std::filesystem::path _path;
        encoding::specs::Format     _format;
        encoding::specs::Confidence _confidence;
        blogator::TextPos           _position;
    };
}

#endif //BLOGATOR_ENCODING_DTO_SOURCE_H