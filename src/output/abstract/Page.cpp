#include "Page.h"

/**
 * Constructor
 * @param master_index   Master index
 * @param templates      Templates
 * @param global_options Global blogator options
 */
blogator::output::abstract::Page::Page( std::shared_ptr<const blogator::dto::Index> master_index,
                                        std::shared_ptr<const blogator::dto::Templates> templates,
                                        std::shared_ptr<const blogator::dto::Options> global_options ) :
    _index( std::move( master_index ) ),
    _templates( std::move( templates ) ),
    _options( std::move( global_options ) )
{}