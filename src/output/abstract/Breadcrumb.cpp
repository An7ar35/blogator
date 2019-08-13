#include "Breadcrumb.h"
#include "../../html/html.h"

/**
 * Write a breadcrumb list to a target file
 * @param page    Output page
 * @param indent  Space to place before the output lines (i.e.: html indent)
 * @param parents List of parent pages {name, path}
 * @param child   Name of child page
 */
void blogator::output::abstract::Breadcrumb::writeBreadcrumb( blogator::dto::Page &page,
                                                              const std::string   &indent,
                                                              const BreadCrumb_t  &parents,
                                                              const std::string   &child ) const
{
    page._out << indent << "<ul>\n";

    for( const auto &p : parents )
        page._out << indent << "\t<li>" << html::createHyperlink( p.second, p.first ) << "</li>\n";

    page._out << indent << "\t<li>" << child << "</li>\n"
              << indent << "</ul>\n";
}