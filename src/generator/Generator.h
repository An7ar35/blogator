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

namespace blogator::generator {
    class Generator {
      public:
        explicit Generator( dto::Options global_options );
        bool init( const dto::Index &master_index );

      private:
        dto::Options _options;

        std::unique_ptr<dto::Template> importTemplates( const dto::Index &master_index ) const;
    };
}

#endif //BLOGATOR_GENERATOR_H
