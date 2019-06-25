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
        explicit Generator( std::shared_ptr<dto::Options> global_options );
        bool init( const dto::Index &master_index,
                   PostMaker        &post_maker,
                   IndexMaker       &index_maker,
                   LandingMaker     &landing_maker,
                   RSS              &rss_maker );

      private:
        std::shared_ptr<dto::Options> _options;

        std::unique_ptr<dto::Template> importTemplates( const dto::Index &master_index ) const;
    };
}

#endif //BLOGATOR_GENERATOR_H
