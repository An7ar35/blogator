#include "Generator.h"

#include <utility>
#include <iostream>
#include <fstream>
#include <stack>
#include <functional>

#include "page/Posts.h"
#include "page/Indices.h"
#include "../exception/failed_expectation.h"

/**
 * Constructor
 * @param master_index   Master index
 * @param templates      Templates
 * @param global_options Global blogator options
 */
blogator::output::Generator::Generator( std::shared_ptr<const dto::Index>     master_index,
                                        std::shared_ptr<const dto::Templates> templates,
                                        std::shared_ptr<const dto::Options>   global_options ) :
    _master_index( std::move( master_index ) ),
    _templates( std::move( templates ) ),
    _options( std::move( global_options ) )
{}

/**
 * Initialize HTML page generation
 * @param post_maker    PostMaker instance
 * @param index_maker   IndexMaker instance
 * @param landing_maker LandingMaker instance
 * @param rss_maker     RSS instance
 * @return Success
 * @throws exception::failed_expectation when a maker cannot generate its targets
 */
bool blogator::output::Generator::init( page::Posts   &post_maker,
                                        page::Indices   &index_maker,
                                        page::Landing &landing_maker,
                                        feed::RSS     &rss_maker ) const
{
    auto posts_ok   = post_maker.init(); //TODO maybe do an if( !.. ) then throw a fit/exception on each inits?
    auto index_ok   = index_maker.init();
    auto landing_ok = landing_maker.init();
    auto rss_ok     = rss_maker.init();

    return ( index_ok && posts_ok && rss_ok && landing_ok );
}