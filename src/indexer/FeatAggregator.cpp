#include "FeatAggregator.h"

#include <iostream>

/**
 * Constructor
 * @param global_options Global blogator options
 */
blogator::indexer::FeatAggregator::FeatAggregator( std::shared_ptr<const blogator::dto::Options> global_options ) :
    _options( std::move( global_options ) )
{}

/**
 * Adds an article to the aggregator if it is featured in the options for the landing page
 * @param article   Article DTO
 * @param order_pos Featured position
 */
void blogator::indexer::FeatAggregator::addArticleIfFeatured( const blogator::dto::Article &article )
{
    const auto it = _options->_landing_page.featured.find( article._paths.out_html );

    if( it != _options->_landing_page.featured.cend() )
        _max_heap.push( std::make_pair( it->second, article ) );
}

/**
 * Gets the ordered list of Articles to be featured on the landing page
 * @return Order list of Article DTOs
 */
std::vector<blogator::dto::Article> blogator::indexer::FeatAggregator::getFeaturedArticles() {
    auto v = std::vector<blogator::dto::Article>();

    while( !_max_heap.empty() ) {
        v.emplace_back( _max_heap.top().second );
        _max_heap.pop();
    }

    return v;
}
