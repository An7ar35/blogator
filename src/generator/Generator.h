#ifndef BLOGATOR_GENERATOR_H
#define BLOGATOR_GENERATOR_H

#include "../dto/Article.h"
#include "../dto/Index.h"
#include "../dto/HTML.h"
#include "../dto/Options.h"

namespace blogator {
    class Generator {
      public:
        explicit Generator( dto::Options global_options );

        bool init( const dto::Index &index );

      private:
        dto::Options               _options;
        std::unique_ptr<dto::HTML> _header;
        std::unique_ptr<dto::HTML> _footer;
    };
}

#endif //BLOGATOR_GENERATOR_H
