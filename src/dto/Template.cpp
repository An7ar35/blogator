#include "Template.h"

/**
 * Constructor
 * @param type Template type
 */
blogator::dto::Template::Template( Template::Type template_type ) :
    type( template_type )
{
    switch( type ) {
        case Type::LANDING:
            block_classes = Template::BlockInsertClasses_t( {
                { "breadcrumb",     false },
                { "newest-posts",   false },
                { "top-tags",       false },
                { "top-authors",    false },
                { "featured-posts", false }
            } );
            break;

        case Type::LANDING_ENTRY: //If this ever differs from INDEX_ENTRY or vice-versa look in the custom index entry caching (indexer::cacheCustomIndexEntry())
            block_classes = Template::BlockInsertClasses_t( {
                { "post-number", false },
                { "heading",     false },
                { "authors",     false },
                { "tags",        false },
                { "date-stamp",  false },
                { "summary",     false }
            } );
            break;

        case Type::POST:
            block_classes = Template::BlockInsertClasses_t( {
                { "breadcrumb",       false },
                { "page-nav",         false },
                { "post-content",     false },
                { "index-pane-dates", false },
                { "index-pane-tags",  false }
            } );
            break;

        case Type::INDEX:
            block_classes = Template::BlockInsertClasses_t( {
                { "page-name",     false },
                { "breadcrumb",    false },
                { "page-nav",      false },
                { "index-entries", false }
            } );
            break;

        case Type::INDEX_ENTRY:
            block_classes = Template::BlockInsertClasses_t( {
                { "post-number", false },
                { "heading",     false },
                { "authors",     false },
                { "tags",        false },
                { "date-stamp",  false },
                { "summary",     false }
            } );
            break;

        case Type::INDEX_LIST:
            block_classes = Template::BlockInsertClasses_t( {
                { "page-name",            false },
                { "breadcrumb",           false },
                { "index-list-flat",      false },
                { "index-list-hierarchy", false }
            } );
            break;
    }
}