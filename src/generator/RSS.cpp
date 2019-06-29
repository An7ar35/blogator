#include <utility>

#include "RSS.h"

blogator::generator::RSS::RSS( std::shared_ptr<dto::Options> global_options ) :
    _options( std::move( global_options ) )
{}
