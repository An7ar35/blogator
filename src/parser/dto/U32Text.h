#ifndef BLOGATOR_PARSER_DTO_U32TEXT_H
#define BLOGATOR_PARSER_DTO_U32TEXT_H

#include <string>
#include <vector>
#include <filesystem>

#include "TextPos.h"

namespace blogator::parser {
    typedef std::vector<char32_t>::const_iterator TextIterator_t;

    class U32Text {
      public:
        explicit U32Text( std::u32string text );
        explicit U32Text( std::vector<char32_t> text );
        U32Text( std::filesystem::path src_path, std::u32string text );
        U32Text( std::filesystem::path src_path, std::vector<char32_t> text );

        [[deprecated]] size_t advanceCol( unsigned int n = 1 );

        size_t advanceCaret( unsigned int n = 1 );
        size_t reverseCaret( unsigned int n = 1 );

        char32_t nextChar();
        void reconsume();

        [[nodiscard]] char32_t character() const;
        [[nodiscard]] std::pair<char32_t, bool> character( std::u32string::iterator::difference_type fwd_n );
        [[nodiscard]] std::u32string characters( std::u32string::iterator::difference_type n );
        [[nodiscard]] TextPos position() const noexcept;
        [[nodiscard]] std::filesystem::path path() const noexcept;

        [[nodiscard]] bool reachedEnd() const;

        void setMarker();
        void resetToMarker();
        void reset();

      private:
        struct State {
            State( TextPos pos, TextIterator_t it, bool nl );
            State( const State & state );
            State & operator =( const State & state );

            std::vector<size_t> line_sizes;
            TextPos             position;
            TextIterator_t      iterator;
            bool                newline;
            bool                reconsume;
        };

        std::filesystem::path _path;
        std::vector<char32_t> _src;
        State                 _state;
        State                 _marker;
    };
}

#endif //BLOGATOR_PARSER_DTO_U32TEXT_H
