#include "LandingMaker.h"

/**
 * Constructor
 * @param master_index   Master index
 * @param templates      Templates
 * @param global_options Global blogator options
 */
blogator::generator::LandingMaker::LandingMaker( std::shared_ptr<const dto::Index>    master_index,
                                                 std::shared_ptr<const dto::Template> templates,
                                                 std::shared_ptr<const dto::Options>  global_options ) :
    _master_index( std::move( master_index ) ),
    _templates( std::move( templates ) ),
    _options( std::move( global_options ) )
{}
