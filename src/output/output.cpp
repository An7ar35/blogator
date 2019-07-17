#include "output.h"

/**
 * Initialize HTML page generation
 * @param index     Master index
 * @param templates Templates
 * @param options   Global blogator options
 * @return Success
 * @throws exception::failed_expectation when a maker cannot generate its targets
 */
bool blogator::output::generate( const std::shared_ptr<const dto::Index>     &index,
                                 const std::shared_ptr<const dto::Templates> &templates,
                                 const std::shared_ptr<const dto::Options>   &options )
{
    auto post_maker    = output::page::Posts( index, templates, options );
    auto index_maker   = output::page::Indices( index, templates, options );
    auto landing_maker = output::page::Landing( index, templates, options );
    auto rss           = output::feed::RSS( index, templates, options );

    auto posts_ok   = post_maker.init(); //TODO maybe do an if( !.. ) then throw a fit/exception on each inits?
    auto index_ok   = index_maker.init();
    auto landing_ok = landing_maker.init();
    auto rss_ok     = rss.init();

    return ( index_ok && posts_ok && rss_ok && landing_ok );
}