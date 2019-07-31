#ifndef BLOGATOR_INDEXER_FEATAGGREGATOR_H
#define BLOGATOR_INDEXER_FEATAGGREGATOR_H

#include <functional>
#include <queue>

#include "../dto/Index.h"
#include "../dto/Options.h"
#include "../cli/MsgInterface.h"

namespace blogator::indexer {
    class FeatAggregator {
      public:
        FeatAggregator( std::shared_ptr<const dto::Options> global_options );

        void addArticleIfFeatured( const dto::Article &article );
        std::vector<dto::Article> getFeaturedArticles();

      private:
        cli::MsgInterface                   &_display;
        std::shared_ptr<const dto::Options> _options;

        typedef std::pair<size_t, dto::Article> HeapNode_t;

        struct Comparator {
            bool operator()( const HeapNode_t &a, const HeapNode_t &b ) const {
                return a.first > b.first;
            }
        };

        std::priority_queue<HeapNode_t, std::vector<HeapNode_t>, Comparator> _max_heap;
    };
}

#endif //BLOGATOR_INDEXER_FEATAGGREGATOR_H
