#ifndef BLOGATOR_GENERATOR_H
#define BLOGATOR_GENERATOR_H

#include <unordered_map>

#include "../dto/Article.h"
#include "../dto/Index.h"
#include "../dto/HTML.h"
#include "../dto/IndexDateTree.h"
#include "../dto/IndexTagTree.h"
#include "../dto/Options.h"
#include "../dto/Templates.h"

#include "page/Posts.h"
#include "page/Indices.h"
#include "page/Landing.h"
#include "feed/RSS.h"

namespace blogator::output {
    class Generator {
      public:
        Generator( std::shared_ptr<const dto::Index>     master_index,
                   std::shared_ptr<const dto::Templates> templates,
                   std::shared_ptr<const dto::Options>   global_options );

        bool init( page::Posts   &post_maker,
                   page::Indices &index_maker,
                   page::Landing &landing_maker,
                   feed::RSS     &rss_maker ) const;

      private:
        std::shared_ptr<const dto::Index>     _master_index;
        std::shared_ptr<const dto::Templates> _templates;
        std::shared_ptr<const dto::Options>   _options;
    };
}

#endif //BLOGATOR_GENERATOR_H