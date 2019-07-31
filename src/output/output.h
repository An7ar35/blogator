#ifndef BLOGATOR_OUTPUT_H
#define BLOGATOR_OUTPUT_H

#include "page/Posts.h"
#include "page/Indices.h"
#include "page/Landing.h"
#include "feed/RSS.h"

namespace blogator::output {
    void generate( const std::shared_ptr<const dto::Index>     &index,
                   const std::shared_ptr<const dto::Templates> &templates,
                   const std::shared_ptr<const dto::Options>   &options );
}

#endif //BLOGATOR_OUTPUT_H