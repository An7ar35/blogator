#ifndef BLOGATOR_ORDEREDPOSTINSERTION_H
#define BLOGATOR_ORDEREDPOSTINSERTION_H

#include <utility>
#include <queue>
#include <variant>

#include "../../dto/InsertPosition.h"
#include "../../dto/TableOfContents.h"

namespace blogator::output::helper {
    enum class InsertPositionType { PATH, TOC, HEADING_ID };

    class OrderedPostInsertion {
      public:
        typedef std::pair<dto::InsertPosition, std::filesystem::path>          PathPosition_t;
        typedef dto::InsertPosition                                            ToCPosition_t;
        typedef std::pair<dto::InsertPosition, dto::TableOfContents::Heading>  HeadingPosition_t;
        typedef std::variant<PathPosition_t, ToCPosition_t, HeadingPosition_t> VariantPosition_t;
        typedef std::pair<InsertPositionType, VariantPosition_t>               HeapNode_t;

      private:
        struct Comparator {
            bool operator()( const HeapNode_t &a, const HeapNode_t &b ) const {
                return OrderedPostInsertion::getInsertPosition( a ) >
                       OrderedPostInsertion::getInsertPosition( b );
            }
        };

        std::priority_queue<HeapNode_t, std::vector<HeapNode_t>, Comparator> _min_heap;

      public:
        void push( InsertPositionType type, VariantPosition_t value );
        void pushPath( const PathPosition_t &value );
        void pushToC( const ToCPosition_t &value );
        void pushHeading( const HeadingPosition_t &value );

        [[nodiscard]] std::priority_queue<HeapNode_t, std::vector<HeapNode_t>, Comparator>::const_reference top() const;
        [[nodiscard]] bool empty() const;
        void pop();

        static const dto::InsertPosition & getInsertPosition( const HeapNode_t &node );
    };
}

#endif //BLOGATOR_ORDEREDPOSTINSERTION_H
