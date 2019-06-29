#ifndef BLOGATOR_GENERATOR_LANDINGMAKER_H
#define BLOGATOR_GENERATOR_LANDINGMAKER_H

#include <memory>

#include "../dto/Options.h"
#include "../dto/Index.h"
#include "../dto/Template.h"

namespace blogator::generator {
    class LandingMaker {
      public:
        LandingMaker( std::shared_ptr<const dto::Index>    master_index,
                      std::shared_ptr<const dto::Template> templates,
                      std::shared_ptr<const dto::Options>  global_options );
      private:
        std::shared_ptr<const dto::Index>    _master_index;
        std::shared_ptr<const dto::Template> _templates;
        std::shared_ptr<const dto::Options>  _options;
    };
}

#endif //BLOGATOR_GENERATOR_LANDINGMAKER_H
