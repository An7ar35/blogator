#include "Generator.h"

#include <utility>
#include <iostream>
#include <fstream>
#include <stack>
#include <functional>

#include "PostMaker.h"
#include "IndexMaker.h"


/**
 * Constructor
 * @param master_index   Master index
 * @param templates      Templates
 * @param global_options Global blogator options
 */
blogator::generator::Generator::Generator( std::shared_ptr<const dto::Index>    master_index,
                                           std::shared_ptr<const dto::Template> templates,
                                           std::shared_ptr<const dto::Options>  global_options ) :
    _master_index( std::move( master_index ) ),
    _templates( std::move( templates ) ),
    _options( std::move( global_options ) )
{}

/**
 * Initialize HTML page generation
 * @param master_index  Master index of all posts/articles
 * @param post_maker    PostMaker instance
 * @param index_maker   IndexMaker instance
 * @param landing_maker LandingMaker instance
 * @param rss_maker     RSS instance
 * @return Success
 */
bool blogator::generator::Generator::init( const dto::Index &master_index,
                                           PostMaker        &post_maker,
                                           IndexMaker       &index_maker,
                                           LandingMaker     &landing_maker,
                                           RSS              &rss_maker )
{
    auto posts_ok = post_maker.init();

    //TODO landing init
    //TODO rss init
    for( auto &a : master_index._indices.byTag.tags ) {
        std::cout << a.first << ":\n"
                  << "\tprefix: " << a.second.tag_id << "\n"
                  << "\tindices  : ";
        for( auto i : a.second.article_indices )
            std::cout << i << ", ";
        std::cout << "\n\tpaths    :\n";
        for( auto p : a.second.file_names )
            std::cout << "\t\t" << p << "\n";
        std::cout << std::endl;
    }

    auto index_ok = index_maker.init();

    return ( index_ok && posts_ok ); //TODO landing_ok && rss_ok
}