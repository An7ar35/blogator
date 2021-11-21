#ifndef BLOGATOR_PARSER_DTO_U32TEXT_H
#define BLOGATOR_PARSER_DTO_U32TEXT_H

#include <string>
#include <vector>
#include <filesystem>

#include "TextPos.h"

namespace blogator::parser {
    typedef std::vector<uint32_t>::const_iterator TextIterator_t;

    class U32Text {
      public:
        explicit U32Text( std::u32string text );
        explicit U32Text( std::vector<uint32_t> text );
        U32Text( std::filesystem::path src_path, std::u32string text );
        U32Text( std::filesystem::path src_path, std::vector<uint32_t> text );

        void advanceColTracker( unsigned int n = 1 );
        void advanceLineTracker();
        size_t advanceCol( unsigned int n = 1 );
        void advanceLine();

        [[nodiscard]] uint32_t character() const;
        [[nodiscard]] std::pair<uint32_t, bool> character( std::u32string::iterator::difference_type fwd_n );
        [[nodiscard]] std::u32string characters( std::u32string::iterator::difference_type n );
        [[nodiscard]] TextPos position() const noexcept;
        [[nodiscard]] std::filesystem::path path() const noexcept;

        [[nodiscard]] bool reachedEnd() const;
        [[nodiscard]] bool reachedEnd( const TextIterator_t & it ) const;

        [[nodiscard]] TextIterator_t iteratorCurr() const;
        [[nodiscard]] TextIterator_t iteratorEnd() const;

        void reset();

      private:
        std::filesystem::path _path;
        std::vector<uint32_t> _src;
        TextPos               _position;
        TextIterator_t        _iterator;
    };
}

#endif //BLOGATOR_PARSER_DTO_U32TEXT_H
