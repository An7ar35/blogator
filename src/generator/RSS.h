#ifndef BLOGATOR_GENERATOR_RSS_H
#define BLOGATOR_GENERATOR_RSS_H

#include <memory>

#include "../dto/Options.h"

namespace blogator::generator {
    class RSS { //TODO
      public:
        RSS( std::shared_ptr<dto::Options> global_options );

      private:
        std::shared_ptr<dto::Options> _options;
    };
}

#endif //BLOGATOR_GENERATOR_RSS_H
