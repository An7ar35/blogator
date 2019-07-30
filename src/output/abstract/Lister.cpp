#include "Lister.h"
#include "../../html/html.h"

/**
 * Constructor
 * @param global_options Global Options DTO
 * @param articles       Articles
 * @param page_template  Page Template DTO to use
 */
blogator::output::abstract::Lister::Lister( std::shared_ptr<const dto::Options> global_options,
                                            const dto::Index::Articles_t &articles,
                                            const dto::Template          &page_template ) :
    _display( cli::MsgInterface::getInstance() ),
    _options( std::move( global_options ) ),
    _articles( articles ),
    _template( page_template )
{}

/**
 * Writes the page navigation html to a file
 * @param page    Target file
 * @param indent  Space to place before the output line (i.e.: html indent)
 * @param paths   Collection of page paths
 * @param path_it Iterator to the current page path where the navigation is being written to
 */
void blogator::output::abstract::Lister::writePageNavDiv( dto::Page &page,
                                                          const std::string &indent,
                                                          const dto::Index::PagePaths_t &paths,
                                                          const dto::Index::PagePaths_t::const_iterator &path_it ) const
{
    const bool   is_first    = path_it == paths.cbegin();
    const bool   is_last     = path_it == std::prev( paths.cend() );
    const size_t page_number = ( path_it - paths.cbegin() ) + 1;

    page._out << indent << "<ul>\n"
              << indent << "\t<li class=\"first\">"
                        << ( is_first
                             ? html::createHyperlink( *paths.cbegin(), _options->_page_nav.first, "disabled" )
                             : html::createHyperlink( *paths.cbegin(), _options->_page_nav.first ) )
                        << "</li>\n"
              << indent << "\t<li class=\"prev\">"
                        << ( is_first
                             ? html::createHyperlink( *paths.cbegin(), _options->_page_nav.backward, "disabled", "prev" )
                             : html::createHyperlink( *std::prev( path_it ), _options->_page_nav.backward, "", "prev" ) )
                        << "</li>\n"
              << indent << "\t<li class=\"curr\">"
                        << std::to_string( page_number ) << _options->_page_nav.separator << std::to_string( paths.size() )
                        << "</li>\n"
              << indent << "\t<li class=\"next\">"
                        << ( is_last
                             ? html::createHyperlink( *std::prev( paths.cend() ), _options->_page_nav.forward, "disabled", "next" )
                             : html::createHyperlink( *std::next( path_it ), _options->_page_nav.forward, "", "next" ) )
                        << "</li>\n"
              << indent << "\t<li class=\"last\">"
                        << ( is_last
                             ? html::createHyperlink( *std::prev( paths.cend() ), _options->_page_nav.last, "disabled" )
                             : html::createHyperlink( *std::prev( paths.cend() ), _options->_page_nav.last ) )
                        << "</li>\n"
              << indent << "</ul>"
              << std::endl;
}