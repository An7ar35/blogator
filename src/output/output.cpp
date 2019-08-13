#include "output.h"
#include "../exception/failed_expectation.h"

/**
 * Initialize HTML page generation
 * @param index     Master index
 * @param templates Templates
 * @param options   Global blogator options
 * @throws exception::failed_expectation when a maker cannot generate its targets
 */
void blogator::output::generate( const std::shared_ptr<const dto::Index>     &index,
                                 const std::shared_ptr<const dto::Templates> &templates,
                                 const std::shared_ptr<const dto::Options>   &options )
{
    auto post_maker    = output::page::Posts( index, templates, options );
    auto index_maker   = output::page::Indices( index, templates, options );
    auto landing_maker = output::page::Landing( index, templates, options );
    auto rss           = output::feed::RSS( index, templates, options );

    if( !post_maker.init() )
        throw exception::failed_expectation( "Failed creating the post targets." );
    if( !index_maker.init() )
        throw exception::failed_expectation( "Failed creating the indices targets." );
    if( !landing_maker.init() )
        throw exception::failed_expectation( "Failed creating the landing page target." );
    if( !rss.init() )
        throw exception::failed_expectation( "Failed creating the RSS feed target." );
}