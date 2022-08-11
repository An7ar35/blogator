#ifndef BLOGATOR_DTO_U32TEXT_H
#define BLOGATOR_DTO_U32TEXT_H

#include <string>
#include <vector>
#include <filesystem>

#include "TextPos.h"

namespace blogator {
    typedef std::vector<char32_t>::const_iterator TextIterator_t;

    /**
     * Read-only UTF-32 buffered text container
     */
    class U32Text {
      public:
        class State; //pre-declaration
        typedef size_t ID_t;

        U32Text( std::filesystem::path src_path, std::u32string text );
        U32Text( std::filesystem::path src_path, std::vector<char32_t> text );

        size_t advanceCaret( unsigned int n = 1 );
        size_t reverseCaret( unsigned int n = 1 );

        char32_t nextChar();
        void reconsume();

        [[nodiscard]] ID_t id() const;
        [[nodiscard]] char32_t character() const;
        [[nodiscard]] std::pair<char32_t, bool> character( std::u32string::iterator::difference_type fwd_n );
        [[nodiscard]] std::u32string characters( std::u32string::iterator::difference_type n );
        [[nodiscard]] TextPos position() const noexcept;
        [[nodiscard]] std::filesystem::path path() const noexcept;
        [[nodiscard]] bool reachedEnd() const;

        void resetToMarker( const State & state );
        [[nodiscard]] State createMarker() const;
        void reset();

        /**
         * Internal tracker state for U32Text
         */
        class State {
            friend U32Text;

          public:
            [[nodiscard]] ID_t id() const;

          private:
            State( ID_t id, TextPos pos, TextIterator_t it, bool nl, bool rc );

            ID_t                text_id;
            TextPos             position;
            TextIterator_t      iterator;
            bool                newline;
            bool                reconsume;
        };

      private:
        ID_t                  _id;
        std::filesystem::path _path;
        std::vector<char32_t> _src;
        std::vector<size_t>   _line_sizes;
        TextPos               _position;
        TextIterator_t        _iterator;
        bool                  _newline;
        bool                  _reconsume;

        void cacheLineSize( const TextPos & pos );
    };
}

#endif //BLOGATOR_DTO_U32TEXT_H
