#include "Templates.h"

/**
 * Constructor
 */
blogator::dto::Templates::Templates() :
    _post( std::make_unique<Template>( Template::Type::POST ) ),
    _index( std::make_unique<Template>( Template::Type::INDEX ) ),
    _index_list( std::make_shared<Template>( Template::Type::INDEX_LIST ) ),
    _index_entry( std::make_shared<Template>( Template::Type::INDEX_ENTRY ) ),
    _landing( std::make_unique<Template>( Template::Type::LANDING ) ),
    _landing_entry( std::make_shared<Template>( Template::Type::LANDING_ENTRY ) )
{}