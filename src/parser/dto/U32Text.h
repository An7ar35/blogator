#ifndef BLOGATOR_PARSER_DTO_U32TEXT_H
#define BLOGATOR_PARSER_DTO_U32TEXT_H

#include <string>

#include "TextPos.h"

namespace blogator::parser {
    typedef std::u32string::const_iterator TextIterator_t;

    class U32Text {
      public:
        explicit U32Text( const std::u32string &text );

        void advanceCol( size_t n = 1 );
        void advanceLine();
        void skipCol( size_t n = 1 );
        void skipLine();

        [[nodiscard]] uint32_t character() const;
        [[nodiscard]] TextPos position() const noexcept;

        [[nodiscard]] bool reachedEnd() const;
        [[nodiscard]] bool reachedEnd( const TextIterator_t & it ) const;

      private:
        const std::u32string & _src;
        TextPos                _position;
        TextIterator_t         _iterator;
    };
}

#endif //BLOGATOR_PARSER_DTO_U32TEXT_H
