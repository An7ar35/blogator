#include <utility>

#include "ByYearList.h"

#include <utility>

#include "../../html/html.h"

/**
 * Constructor
 * @param master_index   Master Index DTO
 * @param global_options Global blogator options
 * @param entry_template Index list Template to use
 */
blogator::output::page::ByYearList::ByYearList( std::shared_ptr<const blogator::dto::Index>   master_index,
                                                std::shared_ptr<const blogator::dto::Options> global_options,
                                                const blogator::dto::Template &entry_template ) :
    IndexList( std::move( global_options ),
               entry_template,
               BreadCrumb_t( { { global_options->_breadcrumb.start, "../../index.html" } } ),
               global_options->_breadcrumb.by_year
    ),
    _index( std::move( master_index ) )
{}

/**
 * Writes a complete flat-level year list
 * @param page   Target file
 * @param indent Space to place before the output line (i.e.: html indent)
 */
void blogator::output::page::ByYearList::writeFlatList( dto::Page &page,
                                                        const std::string &indent ) const
{
    if( !_index->_indices.byYear.cats.empty() ) {
        auto year_it = _index->_indices.byYear.cats.cbegin();
        page._out << indent << "<ul>\n";

        while( year_it != _index->_indices.byYear.cats.cend() ) {
            page._out << indent << "\t<li>"
                      << html::createHyperlink( year_it->second.file_names.front(), year_it->first ) << "</li>\n";
            ++year_it;
        }

        page._out << indent << "</ul>" << std::endl;
    }
}

/**
 * Writes complete year list categorised by year
 * @param page   Target file
 * @param indent Space to place before the output line (i.e.: html indent)
 */
void blogator::output::page::ByYearList::writeHierarchyList( dto::Page &page,
                                                             const std::string &indent ) const
{
    if( !_index->_indices.byYear.cats.empty() ) {
        auto year_it = _index->_indices.byYear.cats.cbegin();
        page._out << indent << "<ul>\n";

        while( year_it != _index->_indices.byYear.cats.cend() ) {
            const std::string current = year_it->first;

            page._out << indent << "\t<li><h3>" << year_it->first << "</h3>\n"
                      << indent << "\t\t<ul>\n";

            for( size_t i : year_it->second.article_indices ) {

                try {
                    const auto &article = _index->_articles.at( i );
                    const auto abs_path = page._abs_path.parent_path();
                    const auto rel_path = ( _options->_paths.posts_dir / article._paths.out_html ).lexically_relative( abs_path );

                    page._out << indent << "\t\t\t<li>"
                              << html::createHyperlink( rel_path, article._heading )
                              << "</li>\n";

                } catch( std::out_of_range &e ) {
                    _display.error(
                        "[output::page::ByYearList::writeHierarchyList(..)] "
                        "Article index (i: " + std::to_string( i ) + ") is not valid: " + e.what()
                    );
                }
            }

            page._out << indent << "\t\t</ul>\n"
                      << indent << "\t</li>\n";

            ++year_it;
        }

        page._out << indent << "</ul>" << std::endl;
    }
}