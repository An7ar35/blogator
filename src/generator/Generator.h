#ifndef BLOGATOR_GENERATOR_H
#define BLOGATOR_GENERATOR_H

#include <unordered_map>

#include "../dto/Article.h"
#include "../dto/Index.h"
#include "../dto/HTML.h"
#include "../dto/IndexDateTree.h"
#include "../dto/IndexTagTree.h"
#include "../dto/Options.h"
#include "../dto/Template.h"

#include "PostMaker.h"
#include "IndexMaker.h"
#include "LandingMaker.h"
#include "RSS.h"

namespace blogator::generator {
    class Generator {
      public:
        Generator( std::shared_ptr<const dto::Index>    master_index,
                   std::shared_ptr<const dto::Template> templates,
                   std::shared_ptr<const dto::Options>  global_options );

        bool init( const dto::Index &master_index,
                   PostMaker        &post_maker,
                   IndexMaker       &index_maker,
                   LandingMaker     &landing_maker,
                   RSS              &rss_maker );

      private:
        std::shared_ptr<const dto::Index>    _master_index;
        std::shared_ptr<const dto::Template> _templates;
        std::shared_ptr<const dto::Options>  _options;
    };
}

#endif //BLOGATOR_GENERATOR_H
