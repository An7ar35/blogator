#include "ByAuthorList.h"
#include "../../html/html.h"

/**
 * Constructor
 * @param master_index   Master Index DTO
 * @param global_options Global blogator options
 * @param entry_template Index list Template to use
 */
blogator::output::page::ByAuthorList::ByAuthorList( std::shared_ptr<const dto::Index> master_index,
                                                    std::shared_ptr<const dto::Options> global_options,
                                                    const dto::Template & entry_template ) :
    IndexList( std::move( global_options ),
               entry_template,
               BreadCrumb_t( { { global_options->_breadcrumb.start, "../../index.html" } } ),
               global_options->_breadcrumb.by_author
    ),
    _index( std::move( master_index ) )
{}

/**
 * Writes a complete flat-level author list
 * @param page   Target file
 * @param indent Space to place before the output line (i.e.: html indent)
 */
void blogator::output::page::ByAuthorList::writeFlatList( dto::Page &page,
                                                          const std::string &indent ) const
{
    if( !_index->_indices.byAuthor.cats.empty() ) {
        auto tag_it = _index->_indices.byAuthor.cats.cbegin();
        page._out << indent << "<ul>\n";

        while( tag_it != _index->_indices.byAuthor.cats.cend() ) {
            page._out << indent << "\t<li>"
                      << html::createHyperlink( tag_it->second.file_names.front(), tag_it->first ) << "</li>\n";
            ++tag_it;
        }

        page._out << indent << "</ul>" << std::endl;
    }
}

/**
 * Writes complete author list categorised alphabetically
 * @param page   Target file
 * @param indent Space to place before the output line (i.e.: html indent)
 */
void blogator::output::page::ByAuthorList::writeHierarchyList( dto::Page & page,
                                                               const std::string & indent ) const
{
    if( !_index->_indices.byAuthor.cats.empty() ) {
        auto tag_it = _index->_indices.byAuthor.cats.cbegin();
        page._out << indent << "<ul>\n";

        while( tag_it != _index->_indices.byAuthor.cats.cend() ) {
            const char sublist_letter = std::tolower( tag_it->first.at( 0 ) );

            page._out << indent << "\t<li><h3>" << char( std::toupper( sublist_letter ) ) << "</h3>\n"
                      << indent << "\t\t<ul>\n";

            while( tag_it != _index->_indices.byAuthor.cats.cend() &&
                   char( std::tolower( sublist_letter ) ) == std::tolower( tag_it->first.at( 0 )  ) )
            {
                page._out << indent << "\t\t\t<li>"
                          << html::createHyperlink( tag_it->second.file_names.front(), tag_it->first ) << "</li>\n";
                ++tag_it;
            }

            page._out << indent << "\t\t</ul>\n"
                      << indent << "\t</li>\n";
        }

        page._out << indent << "</ul>" << std::endl;
    }
}
