#include "Generator.h"

#include <utility>
#include <iostream>
#include <fstream>
#include <stack>
#include <functional>

#include "PostMaker.h"
#include "IndexMaker.h"
#include "../fs/fs.h"
#include "../html/html.h"
#include "../exception/file_access_failure.h"


/**
 * Constructor
 * @param global_options Global blogator options
 */
blogator::generator::Generator::Generator( std::shared_ptr<dto::Options> global_options ) :
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
    auto templates = importTemplates( master_index );

    try {
        templates->_months = fs::importMonthNames( _options->_paths.month_file );
    } catch ( std::exception &e ) {
        std::cout << "Using default month strings (eng)." << std::endl;
    }

    auto posts_ok = post_maker.init( master_index, *templates );
    auto index_ok = index_maker.init( master_index, *templates );

    //TODO landing init
    //TODO rss init
    for( auto &a : master_index._indices.byTag.tag_directories )
        std::cout << a.first << ":\t" << a.second << std::endl;

    return ( index_ok && posts_ok ); //TODO landing_ok && rss_ok
}

/**
 * Imports the HTML templates to use
 * @param options Option DTO (with the paths to the html template files)
 * @return Template DTO loaded and analysed for insertion points
 */
std::unique_ptr<blogator::dto::Template>
    blogator::generator::Generator::importTemplates( const dto::Index &master_index ) const
{
    auto templates = std::make_unique<dto::Template>();

    templates->_post.html  = fs::importHTML( master_index._paths.templates.post );
    templates->_index.html = fs::importHTML( master_index._paths.templates.index );
    templates->_start.html = fs::importHTML( master_index._paths.templates.start );

    html::reader::findInsertPositions( *templates->_post.html, templates->_post.div_write_pos );
    html::reader::findInsertPositions( *templates->_index.html, templates->_index.div_write_pos );
    html::reader::findInsertPositions( *templates->_start.html, templates->_start.div_write_pos );

    return std::move( templates );
}